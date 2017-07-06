#include <linux/edvlogd.h>

#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/wait.h>

#define DEVICE_NAME "edvlog"
#define EDVLOGD_VERSION "0.1.3"

static int debug = 0;
	
static char log[EDV_LOG_LEN];
struct cdev *pcdev;
static dev_t devno;
static struct class *edvlog_class;

static struct edvlogd_t Data;

struct edvlogd_t *edvlogd_get_data(void)
{
	struct edvlogd_t *p = &Data;

	return p;
}
EXPORT_SYMBOL_GPL(edvlogd_get_data);

int edvlogd_get_count(struct edvlogd_t *p)
{
	int i = 0;

	if (p->read == p->write)
		return 0;

	i = p->write - p->read;
	if (i < 0)
		i += EDV_BUFFER_SIZE;

	return i;
}
EXPORT_SYMBOL_GPL(edvlogd_get_count);

void do_edvlog(int line, const char *func, const char *file, int level, const char *format, ...)
{
	char str[EDV_LOG_LEN] = {0};
	char substr[EDV_LOG_LEN] = {0};
	va_list args;
	struct edvlogd_t *p = edvlogd_get_data();
	struct edvlogd_item *pitem = NULL;
	struct timeval tv;
	unsigned long flags;

	if (p->is_init == 0)
		return;

	va_start(args, format);
	do_gettimeofday(&tv);
	vsnprintf(substr, EDV_LOG_LEN, format, args);
	snprintf(str, EDV_LOG_LEN, "%01d%s|[%ld.%06ld][%d(%s)@%s]", level, substr, tv.tv_sec, tv.tv_usec, 
		line, func, file);
	va_end(args);

	spin_lock_irqsave(&p->buf_lock, flags);
	if (edvlogd_is_full(p)) {
		edvlogd_inc(p->read);
	}
	pitem = p->buffer + p->write;
	strncpy(pitem->str, str, EDV_LOG_LEN);
	pitem->tv = tv;
	pitem->is_logged = 1;
	if (debug)
		printk("---> edvlogd: write = %d\n", p->write);
	edvlogd_inc(p->write);
	spin_unlock_irqrestore(&p->buf_lock, flags);
			
	wake_up_interruptible(&p->wq);
}
EXPORT_SYMBOL_GPL(do_edvlog);

int edvlogd_read(char *buf)
{
	struct edvlogd_t *p = edvlogd_get_data();
	struct edvlogd_item *pitem = NULL;
	unsigned long flags;

	spin_lock_irqsave(&p->buf_lock, flags);
	pitem = p->buffer + p->read;
	if (pitem->is_logged == 0) {
		printk("edvlog:read a non-logged item\n");
		return 1;
	}
	strncpy(buf, pitem->str, EDV_LOG_LEN);
	pitem->is_logged = 0;
	if (debug)
		printk("---> edvlogd: read = %d\n", p->read);
	edvlogd_inc(p->read);
	spin_unlock_irqrestore(&p->buf_lock, flags);

	return 0;
}
EXPORT_SYMBOL_GPL(edvlogd_read);

static ssize_t edvlog_read(struct file *filp, char __user *buff, size_t size, loff_t *ppos)
{
	unsigned long ret = 0;
	struct edvlogd_t *p = filp->private_data;

	if (size < EDV_LOG_LEN) {
		printk("edvlog: read buffer size %d/%d\n", size, EDV_LOG_LEN);
		return -EAGAIN;
	}

	while (edvlogd_is_empty(p)) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;

		if (wait_event_interruptible(p->wq, !edvlogd_is_empty(p)))
			return -ERESTARTSYS;
	}

	edvlogd_read(log);
	ret = copy_to_user(buff, log, EDV_LOG_LEN);

	return ret;
}


static ssize_t edvlog_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long ret = 0;

	return ret;
}

static int edvlog_open(struct inode *inode, struct file *filp)
{
	filp->private_data = edvlogd_get_data();

	return 0;
}

static int edvlog_close(struct inode *inode, struct file *filp)
{
	filp->private_data = NULL;

	return 0;
}

static int edvlog_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static struct file_operations edvlog_fops = 
{
	.owner = THIS_MODULE,
	.read  = edvlog_read,
	.write = edvlog_write,
	.open  = edvlog_open,
	.release = edvlog_close,
	.ioctl  = edvlog_ioctl,
};

static int __init edvlogd_init(void)
{
	int err,result;

	result = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
	if(result < 0){
		printk(DEVICE_NAME "edvlog: can't get alloc register major number.\n");
		return result;
	}
	
	edvlog_class = class_create(THIS_MODULE, DEVICE_NAME);
	if( IS_ERR(edvlog_class) ){
		printk ("edvlog: failed in keyboard_class create.\n");
		return -1;
	}
	device_create(edvlog_class, NULL, devno, NULL, DEVICE_NAME);
	
	pcdev = cdev_alloc();
	pcdev->ops = &edvlog_fops;
	cdev_init(pcdev, &edvlog_fops);
	pcdev->owner = THIS_MODULE;
	err = cdev_add(pcdev, devno, 1);
	if (err) {
		printk("edvlog: adding err\r\n");
		unregister_chrdev_region(devno,1);
		return err;
	}	

	memset(log, 0, EDV_LOG_LEN);
	memset(&Data, 0, sizeof(struct edvlogd_t));
	spin_lock_init(&Data.buf_lock);
	Data.is_init = 1;
	Data.size = EDV_BUFFER_SIZE;
	init_waitqueue_head(&(Data.wq));

	pr_info("edvlogd: service started, version %s\n", EDVLOGD_VERSION);

	return 0; 
}

static void __exit edvlogd_exit(void)
{
	cdev_del(pcdev);
	unregister_chrdev_region(devno, 1);
	device_destroy(edvlog_class, devno);
	class_destroy(edvlog_class);

	Data.is_init = 0;
	pr_info("edvlogd: exit\n");
}

fs_initcall(edvlogd_init);
module_exit(edvlogd_exit);

MODULE_VERSION(EDVLOGD_VERSION);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("zhssmail@gmail.com");

