#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/bcd.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <mach/mux.h>
#include <mach/devno.h>
#include <linux/kdev_t.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/syscalls.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <asm-generic/ioctl.h>
#include <linux/keyboard.h>

#define KBD_DATA_SIZE 		100
#define DEV_NAME			"kbd"

struct kbd_data{
	unsigned int    scan_code;
	unsigned int    value;
	unsigned int    down_flag;
	unsigned int    led_value;
};

struct usbkbd_dev {
	struct kbd_data 	data[KBD_DATA_SIZE];
	struct kbd_data*	read;
	struct kbd_data*	write;
	wait_queue_head_t	waitq;
	spinlock_t			usbkbd_lock;
	int 				f_open;
};

static struct usbkbd_dev* usbkbd = NULL;
static int kbd_devNo = 0;
static int major = USB_KBD_MAJOR;

static int kbd_nb_call(struct notifier_block *blk, unsigned long code, void *_param);

static struct notifier_block kbd_nb = {
	.notifier_call = kbd_nb_call,
};

static int usbkbd_open(struct inode *inode, struct file *file)
{
	usbkbd->f_open = 1;
	register_keyboard_notifier(&kbd_nb);

	return nonseekable_open(inode, file);
}

static int usbkbd_release(struct inode *inode, struct file *file)
{
	usbkbd->f_open = 0;
	unregister_keyboard_notifier(&kbd_nb);

	return 0;
}

static ssize_t usbkbd_read(struct file *file, __user char *buffer, size_t count,
			loff_t *ppos)
{
    unsigned int c = 0, ret = 0;
	unsigned long flags;
    int size = sizeof(struct kbd_data);
	DECLARE_WAITQUEUE(wait, current);

    if(NULL == buffer || count < size) {
        return -EINVAL;
    }

    spin_lock_irqsave(&usbkbd->usbkbd_lock, flags);
	while(0 == ret){
		if(usbkbd->write == usbkbd->read){
			add_wait_queue(&usbkbd->waitq, &wait);
			set_current_state(TASK_INTERRUPTIBLE);

			while(usbkbd->write == usbkbd->read){
				if(file->f_flags & O_NONBLOCK){
					ret = -EAGAIN;
					break;
				}
				if (signal_pending(current)) {
					ret = -ERESTARTSYS;
					break;
				}
				if(0 == usbkbd->f_open){
					ret = -EINVAL;
					break;
				}
				spin_unlock_irqrestore(&usbkbd->usbkbd_lock, flags);
				schedule();			
				set_current_state(TASK_INTERRUPTIBLE);
				spin_lock_irqsave(&usbkbd->usbkbd_lock, flags);
			}

			set_current_state(TASK_RUNNING);
			remove_wait_queue(&usbkbd->waitq, &wait);
		}

		if(ret)
			goto out;
	
	    if (usbkbd->write >= usbkbd->read)
	        c = usbkbd->write - usbkbd->read;
	    else
	        c =  (KBD_DATA_SIZE) - (usbkbd->read - usbkbd->write);
	
	    while (count >= size && c) {
			spin_unlock_irqrestore(&usbkbd->usbkbd_lock, flags);
			if (copy_to_user(buffer, (char*)(usbkbd->read), size)) {
				return -EFAULT;
			}

			spin_lock_irqsave(&usbkbd->usbkbd_lock, flags);
			if (usbkbd->read == (usbkbd->data + KBD_DATA_SIZE - 1)) {
				usbkbd->read = usbkbd->data;
			}
			else {
				usbkbd->read++;
			}

			buffer += size;		 	
			c--;
			count -= size;
			ret += size;
    	}
	}

out:
	spin_unlock_irqrestore(&usbkbd->usbkbd_lock, flags);
    return ret;
}

static ssize_t usbkbd_write (struct file *file, const char __user *set_bit, size_t count, loff_t *ppos)
{
	unsigned char s_bit = *(unsigned char*)set_bit;	

	if(0x01 != s_bit && 0x02 != s_bit && 0x03 != s_bit){
		return -EINVAL;
	}

	kbd_set_lock_led(s_bit);
	return count;
}

static struct file_operations usbkbd_fops = {
    .owner   = THIS_MODULE,
    .open    = usbkbd_open,
    .read    = usbkbd_read,
	.write   = usbkbd_write,
    .release = usbkbd_release,
};

bool write_usbkeycode(unsigned int scan_code, unsigned int value, unsigned int down_flag, unsigned int led_value)
{
	struct kbd_data* p_write = usbkbd->write;
	unsigned long flags;

	if(p_write == usbkbd->data + KBD_DATA_SIZE - 1) {
		p_write = usbkbd->data;
	}
	else {
		p_write++;
	}

	spin_lock_irqsave(&usbkbd->usbkbd_lock, flags);
	if(p_write == usbkbd->read) {
		spin_unlock_irqrestore(&usbkbd->usbkbd_lock, flags);
		return false;
	}

	usbkbd->write->scan_code = scan_code;
	usbkbd->write->value = value;
	usbkbd->write->down_flag = down_flag;
	usbkbd->write->led_value = led_value;
	usbkbd->write = p_write;
	spin_unlock_irqrestore(&usbkbd->usbkbd_lock, flags);

	wake_up_interruptible(&usbkbd->waitq);	
	return true;
}

static int kbd_nb_call(struct notifier_block *blk, unsigned long code, void *_param)
{
	struct keyboard_notifier_param *param = (struct keyboard_notifier_param *)_param;
	int ret = NOTIFY_OK;

	static unsigned int keycode = -1;
	static unsigned short value = -1;

	switch(code){
	case KBD_KEYCODE:
	case KBD_UNBOUND_KEYCODE:
		keycode = param->value;
		break;

	case KBD_UNICODE:
		value = param->value;
		break;

	case KBD_KEYSYM:
		value = param->value & 0x000000FF;
		break;

	case KBD_POST_KEYSYM:
		write_usbkeycode(keycode, value, param->down, param->ledstate);
		break;

	default:
		break;
	}

	return ret;
}

static int __init usbkbd_init(void)
{
	int ret = 0;
	kbd_devNo = register_chrdev(major, DEV_NAME, &usbkbd_fops);
	if(0 != kbd_devNo){
		printk(KERN_ALERT "kbd register device no error\n");
		goto fail;
	}

	usbkbd = kmalloc(sizeof(struct usbkbd_dev),GFP_KERNEL);	
	if (!usbkbd){		
		ret = -ENOMEM;		
		goto fail;	
	}	
	memset(usbkbd, 0, sizeof(struct usbkbd_dev));
	usbkbd->read = usbkbd->data;
	usbkbd->write = usbkbd->data;

	init_waitqueue_head(&usbkbd->waitq);
	spin_lock_init(&usbkbd->usbkbd_lock);
fail:	
	return ret;
}

static void __exit usbkbd_exit(void)
{
	unregister_chrdev(kbd_devNo, DEV_NAME);
	kfree(usbkbd);
}

module_init(usbkbd_init);
module_exit(usbkbd_exit);

MODULE_AUTHOR("zhssmail@gmail.com");
MODULE_DESCRIPTION("USBKEYBOARD");
MODULE_LICENSE("GPL");

