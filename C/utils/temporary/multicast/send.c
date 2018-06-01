#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define MULCAST_PORT	6000
#define MULCAST_IP		"225.0.0.1"
#define BUF_LEN			128
#define LEN_IP			16

int main(int argc, char* argv[])
{
	int sock = -1;
	int cnt = -1;
	int len = -1;
	char buf[BUF_LEN] = {0};
	char tmpIp[LEN_IP] = {0};
	struct sockaddr_in mulcast_addr;

	if(1 == argc){
		strncpy(tmpIp, MULCAST_IP, LEN_IP);
	}else if(2 == argc){
		strncpy(tmpIp, argv[1], LEN_IP);
	}else{
		fprintf(stdout, "Usage: cmd [multi ip]\n");
		return -1;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > sock){
		perror("socket err:");
		return -1;
	}

	// reuseraddr
	{
		int val = 1;
		cnt = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(val));
	//	cnt = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&val, sizeof(val));
		if(0 > cnt){
			perror("set reuser addr err:");
			goto _out;
		}
	}

	// init
	memset(&mulcast_addr, 0, sizeof(struct sockaddr_in));
	mulcast_addr.sin_family = AF_INET;
	mulcast_addr.sin_port = htons(MULCAST_PORT);
	mulcast_addr.sin_addr.s_addr = inet_addr(tmpIp);

	while(1){
		len = sizeof(struct sockaddr);
		memset(buf, 0, sizeof(buf));
		fprintf(stdout, "Input str: ");
		fgets(buf, sizeof(buf), stdin);
		
		// sendto
		cnt = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&mulcast_addr, len);
		if(0 > cnt){
			perror("send to err:");
			goto _out;
		}
		fprintf(stdout, "server sended len:%d.\n", cnt);	

		if( !strncmp(buf, "quit", 4) ){
			printf("server exit.\n");
			break;
		}
	}

	//close
	close(sock);
	return 0;
_out:
	close(sock);
	return -1;
}
