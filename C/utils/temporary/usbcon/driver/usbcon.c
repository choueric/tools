#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/poll.h>
#include <linux/usb.h>
#include <linux/usbdevice_fs.h>
#include <linux/usb/hcd.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define MODULE_NAME "usbcon"
#define dbg_print(fmt, arg...) printk(KERN_DEBUG "%s: " fmt, MODULE_NAME, ##arg)
#define err_print(fmt, arg...) printk(KERN_ERR "%s: " fmt, MODULE_NAME, ##arg)


////////////////////////////////////////////////////////////////////////////////
/* copy from drivers/usb/core/hub.c */
struct usb_hub {
	struct device		*intfdev;	/* the "interface" device */
	struct usb_device	*hdev;
	struct kref		kref;
	struct urb		*urb;		/* for interrupt polling pipe */

	/* buffer for urb ... with extra space in case of babble */
	char			(*buffer)[8];
	union {
		struct usb_hub_status	hub;
		struct usb_port_status	port;
	}			*status;	/* buffer for status reports */
	struct mutex		status_mutex;	/* for the status buffer */

	int			error;		/* last reported error */
	int			nerrors;	/* track consecutive errors */

	struct list_head	event_list;	/* hubs w/data or errs ready */
	unsigned long		event_bits[1];	/* status change bitmask */
	unsigned long		change_bits[1];	/* ports with logical connect
							status change */
	unsigned long		busy_bits[1];	/* ports being reset or
							resumed */
	unsigned long		removed_bits[1]; /* ports with a "removed"
							device present */
	unsigned long		wakeup_bits[1];	/* ports that have signaled
							remote wakeup */
#if USB_MAXCHILDREN > 31 /* 8*sizeof(unsigned long) - 1 */
#error event_bits[] is too short!
#endif

	struct usb_hub_descriptor *descriptor;	/* class descriptor */
	struct usb_tt		tt;		/* Transaction Translator */

	unsigned		mA_per_port;	/* current for each child */

	unsigned		limited_power:1;
	unsigned		quiescing:1;
	unsigned		disconnected:1;

	unsigned		quirk_check_port_auto_suspend:1;

	unsigned		has_indicators:1;
	u8			indicator[USB_MAXCHILDREN];
	struct delayed_work	leds;
	struct delayed_work	init_work;
	struct usb_port		**ports;
};
#define	USB_STS_TIMEOUT		1000
#define	USB_STS_RETRIES		5

static int get_port_status(struct usb_device *hdev, int port1,
		struct usb_port_status *data)
{
	int i, status = -ETIMEDOUT;

	for (i = 0; i < USB_STS_RETRIES &&
			(status == -ETIMEDOUT || status == -EPIPE); i++) {
		status = usb_control_msg(hdev, usb_rcvctrlpipe(hdev, 0),
			USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_PORT, 0, port1,
			data, sizeof(*data), USB_STS_TIMEOUT);
	}
	return status;
}

static int hub_port_status(struct usb_hub *hub, int port1,
		u16 *status, u16 *change)
{
	int ret;
    struct usb_port_status st;

	ret = get_port_status(hub->hdev, port1, &st);
	if (ret < 4) {
        dbg_print("get port status failed\n");
		if (ret >= 0)
			ret = -EIO;
	} else {
		*status = le16_to_cpu(st.wPortStatus);
		*change = le16_to_cpu(st.wPortChange);
		ret = 0;
	}
	return ret;
}

static int set_port_feature(struct usb_device *hdev, int port1, int feature)
{
    return usb_control_msg(hdev, usb_sndctrlpipe(hdev, 0), USB_REQ_SET_FEATURE, 
            USB_RT_PORT, feature, port1, NULL, 0, 1000);
}

static struct usb_hub *hdev_to_hub(struct usb_device *hdev)
{
	if (!hdev || !hdev->actconfig || !hdev->maxchild)
		return NULL;
	return usb_get_intfdata(hdev->actconfig->interface[0]);
}

////////////////////////////////////////////////////////////////////////////////

static int hub_poweron_ports(struct usb_hub *hub)
{
    int port;
    int ret;
    u16 st, change;

    for (port = 1; port <= hub->descriptor->bNbrPorts; port++) {

        hub_port_status(hub, port, &st, &change);
        dbg_print("status %04x change %04x\n", st, change);

        msleep(100);

        ret = set_port_feature(hub->hdev, port, USB_PORT_FEAT_POWER);
		if (ret) {
			dev_err(hub->intfdev, "cannot power port %d (err = %d)\n", port, ret);
        } else {
            printk("   power on port %d ok.\n", port);
        }

        msleep(100);

        ret = set_port_feature(hub->hdev, port, USB_PORT_FEAT_ENABLE);
		if (ret) {
			dev_err(hub->intfdev, "cannot power port %d (err = %d)\n", port, ret);
        } else {
            printk("   enable on port %d ok.\n", port);
        }

        msleep(100);

        hub_port_status(hub, port, &st, &change);
        dbg_print("status %04x change %04x\n", st, change);
    }

    return 0;
}

/* This is a recursive function. Parameters:
 * The caller must own the device lock.
 */
static ssize_t usbcon_device_iterate(struct usb_device *usbdev, struct usb_bus *bus, int level, int index, char *buf)
{
	int port;
	int ret;
	ssize_t total_written = 0;
	struct usb_device *childdev = NULL;
    __le16 vid, pid;

        
    vid = usbdev->descriptor.idVendor;
    pid = usbdev->descriptor.idProduct;

    dbg_print("bus %d, dev %d, path %s, level %d, port %d, vid:pid = %04X:%04X\n", usbdev->bus->busnum, usbdev->devnum, usbdev->devpath, 
            usbdev->level, usbdev->portnum, vid, pid);
    total_written = sprintf(buf, "bus %d, dev %d, path %s, level %d, port %d, vid:pid = %04X:%04X\n", usbdev->bus->busnum, usbdev->devnum, usbdev->devpath, 
            usbdev->level, usbdev->portnum, vid, pid);

	/* Now look at all of this device's children. */
#if 1
    usb_hub_for_each_child(usbdev, port, childdev) {
#else
    for (port = 0; port < usbdev->maxchild; port++) {
        childdev = usbdev->children[port];
        if (childdev) {
#endif
            usb_lock_device(childdev);
            ret = usbcon_device_iterate(childdev, bus, level + 1, port, buf + total_written);
            usb_unlock_device(childdev);
            if (ret == -EFAULT)
                return total_written;
            total_written += ret;
#if 0
        }
#endif
    }
	return total_written;
}

static ssize_t usbcon_all_iterate(char *buf)
{
	struct usb_bus *bus;
	ssize_t ret, total_written = 0;

	mutex_lock(&usb_bus_list_lock);
	/* print devices for all busses */
	list_for_each_entry(bus, &usb_bus_list, bus_list) {
		/* recurse through all children of the root hub */
		if (!bus_to_hcd(bus)->rh_registered) {
			continue;
        }

		usb_lock_device(bus->root_hub);
		ret = usbcon_device_iterate(bus->root_hub, bus, 0, 0, buf + total_written);
		usb_unlock_device(bus->root_hub);
		if (ret < 0) {
			mutex_unlock(&usb_bus_list_lock);
			return ret;
		}
		total_written += ret;
	}
	mutex_unlock(&usb_bus_list_lock);
	return total_written;
}

static int do_hub_poweron_ports(int devnum)
{
    dbg_print("%s\n", __func__);
    return 0;
}


/*--------------------------------------------------------------------------------*/
static int Selected = 0;

static ssize_t devices_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return usbcon_all_iterate(buf);
}

static ssize_t select_store(struct device *dev, struct class_attribute *attr, const char *buf, size_t len)
{
    int ret;
    int val;

    ret = strict_strtol(buf, 0, &val);
    if (ret < 0)
        return ret;

    if (val <= 0) {
        return -1;
    } else {
        Selected = val;
    }
	return len;
}

static ssize_t select_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", Selected);
}

#define ARRAY_SIZE(array) (sizeof((array)) / sizeof(*(array)))

#define CMD_PORT_POWERON 0

struct cmd {
    char *str;
    int cmd;
};

static struct cmd cmds[] = {
    {"set power of hub's all port", CMD_PORT_POWERON},
};

static ssize_t control_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct cmd *p = NULL;
    int n = ARRAY_SIZE(cmds);
    int i;
    int count = 0;

    for (i = 0; i < n; i++) {
        p = &cmds[i];
        count += sprintf(buf, "%d: %s\n", p->cmd, p->str);
    }
    return count;
}

static ssize_t control_store(struct device *dev, struct class_attribute *attr, const char *buf, size_t len)
{
    int ret;
    int val;

    ret = strict_strtol(buf, 0, &val);
    if (ret < 0)
        return ret;

    switch (val) {
        case CMD_PORT_POWERON:
            ret = do_hub_poweron_ports(Selected);
            break;
        default:
            ret = 0;
            break;
    }

	return len;
}

static struct class_attribute usbcon_class_attrs[] = {
	__ATTR(devices, 0755, devices_show, NULL),
	__ATTR(select, 0755, select_show, select_store),
    __ATTR(control, 0755, control_show, control_store),
	__ATTR_NULL,
};

static struct class usbcon_class = {
	.name = "usbcon",
	.owner = THIS_MODULE,
	.class_attrs = usbcon_class_attrs,
};

static int usbcon_init_sysfs()
{
	int status;

	status = class_register(&usbcon_class);
	if (status < 0)
		return status;

	return status;
}


static int __init usbcon_init(void)
{
	dbg_print("init\n");

	if (usbcon_init_sysfs()) {
        dbg_print("init sysfs failed\n");
		return 0;
    }

	return 0;
}

static void usbcon_exit(void)
{
	dbg_print("exit\n");
    class_unregister(&usbcon_class);
}

module_init(usbcon_init);
module_exit(usbcon_exit);

MODULE_LICENSE("GPL");
