#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define PORT	6000

int main(int argc, char* argv[])
{
	int fd = -1;
	int cnt = -1;
	int len = -1;
	char buf[BUF_LEN] = {0};
	char tmpIp[LEN_IP] = {0};
	struct sockaddr_in addr;

    if (argc != 2) {
        printf("usage: send ip\n");
        return 1;
    }

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket err:");
		return 2;
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(tmpIp);

	while(1){
		len = sizeof(struct sockaddr);
		memset(buf, 0, sizeof(buf));
		fprintf(stdout, "Input str: ");
		fgets(buf, sizeof(buf), stdin);
		
		// sendto
		cnt = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&addr, len);
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
	close(fd);
	return 0;
_out:
	close(fd);
	return -1;
}
