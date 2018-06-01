#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <linux/netlink.h>

#define NETLINK_MDRY 21
#define NL_GROUP 1

#define MAX_PAYLOAD 1024

int main(int argc, char **argv)
{
	int sk;
	struct sockaddr_nl src_addr, dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	struct msghdr msg;


	sk = socket(AF_NETLINK, SOCK_RAW, NETLINK_MDRY);
	if (sk < 0) {
		perror("socket failed");
		return -1;
	}

	memset(&msg, 0, sizeof(msg));
	memset(&src_addr, 0, sizeof(src_addr));
	memset(&dest_addr, 0, sizeof(dest_addr));

	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	src_addr.nl_groups = NL_GROUP;
	if (bind(sk, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
		perror("bind failed");
		close(sk);
		return -2;
	}

	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;
	dest_addr.nl_groups = 0;
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;
	strcpy(NLMSG_DATA(nlh), "hello you.");
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	printf("sending msg ...\n");
	sendmsg(sk, &msg, 0);
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	printf("waiting msg ...\n");
	recvmsg(sk, &msg, 0);
	printf("--> %s\n", (char *)NLMSG_DATA(nlh));
	close(sk);

	return 0;
}
