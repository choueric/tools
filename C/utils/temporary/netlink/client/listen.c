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

#define NETLINK_MDRY 23
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

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	iov.iov_base = (void *)nlh;
	iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	while (1) {
		recvmsg(sk, &msg, 0);
		printf("%s\n", (char *)NLMSG_DATA(nlh));
	}
	close(sk);

	return 0;
}
