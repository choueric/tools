#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/kthread.h>

#define NETLINK_MDRY 23
#define MAX_GROUP 10
#define SPECI_GROUP 1

#define MODULE_NAME "nl_test"
#define dbg_print(fmt, arg...) printk(KERN_DEBUG "%s: " fmt, MODULE_NAME, ##arg)

struct sock *nltest_sk = NULL;

static struct task_struct *nltest_task = NULL;

void nltest_rcv(struct sk_buff *__skb)
{
}


int nltest_send_event(char *str)
{
	int skblen, len;
	struct nlmsghdr *nlh;
	struct sk_buff *skb;
	char *data;
	int ret = 0;

	int pid = 0;
	int seq = 0;
	int type = 0;

	if (!nltest_sk)
		return -1;

	len = strlen(str);
	skblen = NLMSG_SPACE(len);
	skb = nlmsg_new(skblen, GFP_ATOMIC);
	if (!skb) {
		dbg_print("allocate failed.\n");
		return -2;
	}

	nlh = nlmsg_put(skb, pid, seq, type, len, 0);
	if (!nlh) {
		dbg_print("get nhl failed.\n");
		kfree_skb(skb);
		return -3;
	}
	data = nlmsg_data(nlh);
	memcpy(data, str, len);
	nlmsg_end(skb, nlh);

	ret = nlmsg_multicast(nltest_sk, skb, pid, SPECI_GROUP, GFP_ATOMIC);
	if (ret) {
		dbg_print("send error. %d\n", ret);
		return -4;
	}
	dbg_print("send is ok.\n");
	return 0;
}

static int nltest_thread(void *data)
{
	static int count = 0;
	char buf[100];

	do {
		msleep(5000);
		snprintf(buf, 100, "test: %d", count++);
		nltest_send_event(buf);
	} while (!kthread_should_stop());

	return 0;
}

static int nltest_init(void)
{
	dbg_print("init\n");

	nltest_sk = netlink_kernel_create(&init_net, NETLINK_MDRY, MAX_GROUP,
			nltest_rcv, NULL, THIS_MODULE);
	if (!nltest_sk) {
		dbg_print("cannot create netlink socket.\n");
		return -ENOMEM;
	}
	dbg_print("create netlink socket ok.\n");

	nltest_task = kthread_run(nltest_thread, NULL, "nltest");
	if (IS_ERR(nltest_task)) {
		dbg_print("kthtread_run failed.\n");
		netlink_kernel_release(nltest_sk);
		return -1;
	}

	return 0;
}

static void nltest_exit(void)
{
	dbg_print("exit\n");
	if (nltest_task) {
		kthread_stop(nltest_task);
		nltest_task = NULL;
	}
	netlink_kernel_release(nltest_sk);
	nltest_sk = NULL;
}

module_init(nltest_init);
module_exit(nltest_exit);

MODULE_LICENSE("GPL");
