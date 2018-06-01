#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MULCAST_PORT	6000
#define MULCAST_IP		"225.0.0.1"
#define BUF_LEN			128
#define LEN_IP			16

int swap_be32(int x)
{
    return ((x >> 24) & 0x000000ff) | 
        ((x << 24) & 0xff000000) |
        ((x >> 8) & 0x0000ff00) |
        ((x << 8) & 0x00ff0000);
}

int get_ip(char *iface)
{
    struct sockaddr_in *addr;
    struct ifreq req;
    int fd ,ret;

    if (iface == NULL)
        return 0;

    strncpy(req.ifr_name, iface, IFNAMSIZ);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return 0;
    ret = ioctl(fd, SIOCGIFADDR, &req);
    close(fd);
    if (ret < 0)
        return 0;

    addr = (struct sockaddr_in *)&(req.ifr_addr);
    return addr->sin_addr.s_addr;
}

int main(int argc, char* argv[])
{
	int fd = -1;
	int c = -1;
	socklen_t len = -1;
	int loop = 1;
	char buf[BUF_LEN] = {0};
	char tmpIp[LEN_IP] = {0};
	struct sockaddr_in local_addr;
	struct ip_mreq mreq;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket err:");
		return -1;
	}
	
	// init
	memset(&local_addr, 0, sizeof(struct sockaddr_in));	
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	local_addr.sin_port = htons(MULCAST_PORT);

	// bind
	cnt = bind(sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));
	if(cnt < 0){
		perror("bind err:");
		goto _out;
	}

	cnt = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if(cnt < 0){
		perror("multicast loop");
		goto _out;
	}
	
	mreq.imr_multiaddr.s_addr = inet_addr(tmpIp);
	//mreq.imr_interface.s_addr = htonl("10.0.2.15");
	mreq.imr_interface.s_addr = get_ip("eth0");
    printf("ip = %x\n", swap_be32(get_ip("eth0")));

	cnt = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if(0 > cnt){
		perror("ip add membership");
		goto _out;
	}
		
	while(1){
		len = sizeof(struct sockaddr_in);
		memset(buf, 0, sizeof(buf));
		cnt = recvfrom(sock, buf, BUF_LEN, 0, (struct sockaddr*)&local_addr, &len);
		if(0 > cnt) {
			perror("recv failed:");
			goto _out;
		} else if(BUF_LEN == cnt) {
			buf[BUF_LEN] = 0x00;
		}
		printf("client received len:%d %s", cnt, buf);
		
		if( !strncmp(buf, "quit", 4) ){
			printf("client exit.\n");
			break;
		}
	}
	
	cnt = setsockopt(sock,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
	if(cnt < 0){
		perror("IP_DROP_MEMBERSHIP");
		return -1;
	}

	//close
	close(sock);
	return 0;
_out:
	close(sock);
	return -1;
}
