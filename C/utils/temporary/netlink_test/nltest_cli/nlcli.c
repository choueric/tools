#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include "nlevent.h"

#define MODULE_NAME "nlcli"
#define dbg_print(fmt, arg...) printk(KERN_DEBUG "%s: " fmt, MODULE_NAME, ##arg)
#define err_print(fmt, arg...) printk(KERN_ERR "%s: " fmt, MODULE_NAME, ##arg)
#define warning_print(fmt, args...) printk(KERN_WARNIG "%s: " fmt, MODULE_NAME, ##arg)

#define NETLINK_MDRY 23
#define MAX_GROUP 10
#define SPECI_GROUP 1

static struct task_struct *nlcli_task = NULL;

static int nlcli_thread(void *data)
{
	static int count = 0;
	char buf[100];

	do {
		msleep(5000);
        if (kthread_should_stop())
            break;

        /* send event message to userspace via netlink */
		snprintf(buf, 100, "test: %d", count++);
		nlevent_send(buf);
	} while (!kthread_should_stop());

	return 0;
}

static int nlcli_init(void)
{
	dbg_print("init\n");

	nlcli_task = kthread_run(nlcli_thread, NULL, "nlcli");
	if (IS_ERR(nlcli_task)) {
		dbg_print("kthtread_run failed.\n");
		return -1;
	}

	return 0;
}

static void nlcli_exit(void)
{
	dbg_print("exit\n");
	if (nlcli_task) {
		kthread_stop(nlcli_task);
		nlcli_task = NULL;
	}
}

module_init(nlcli_init);
module_exit(nlcli_exit);

MODULE_LICENSE("GPL");
