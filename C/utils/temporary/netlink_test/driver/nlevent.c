#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>

#define MODULE_NAME "nlevent"
#define dbg_print(fmt, arg...) printk(KERN_DEBUG "%s: " fmt, MODULE_NAME, ##arg)
#define err_print(fmt, arg...) printk(KERN_ERR "%s: " fmt, MODULE_NAME, ##arg)
#define warning_print(fmt, args...) printk(KERN_WARNIG "%s: " fmt, MODULE_NAME, ##arg)

#define NETLINK_MDRY 23
#define MAX_GROUP 10
#define SPECI_GROUP 1

struct sock *nlevent_sk = NULL;

void nlevent_rcv(struct sk_buff *__skb)
{
}

int nlevent_send(char *str)
{
	int skblen, len;
	struct nlmsghdr *nlh;
	struct sk_buff *skb;
	char *data;
	int ret = 0;

	int pid = 0;
	int seq = 0;
	int type = 0;

	if (!nlevent_sk)
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

	ret = nlmsg_multicast(nlevent_sk, skb, pid, SPECI_GROUP, GFP_ATOMIC);
	if (ret) {
		dbg_print("send error. %d\n", ret);
		return -4;
	}
	dbg_print("send is ok.\n");
	return 0;
}
EXPORT_SYMBOL(nlevent_send);

static int nlevent_init(void)
{
	dbg_print("init\n");

	nlevent_sk = netlink_kernel_create(&init_net, NETLINK_MDRY, MAX_GROUP,
			nlevent_rcv, NULL, THIS_MODULE);
	if (!nlevent_sk) {
		dbg_print("cannot create netlink socket.\n");
		return -ENOMEM;
	}
	dbg_print("create netlink socket ok.\n");

	return 0;
}

static void nlevent_exit(void)
{
	dbg_print("exit\n");
	netlink_kernel_release(nlevent_sk);
	nlevent_sk = NULL;
}

module_init(nlevent_init);
module_exit(nlevent_exit);

MODULE_LICENSE("GPL");
