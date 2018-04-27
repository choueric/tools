#include <unistd.h>
#include <stdio.h>

char *lsCmd[] = {
	"/bin/ls",
};

char *sortCmd[] = {
	"/usr/bin/wc",
};

int main(int argc, char **argv)
{
	int fd[2];
	if (pipe(fd) < 0) {
		printf("pipe failed\n");
		return -1;
	}

	// left
	if(fork() == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("ls", "ls", NULL);
	} 

	// right
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	execlp("wc", "wc", NULL);
	return 0;
}
