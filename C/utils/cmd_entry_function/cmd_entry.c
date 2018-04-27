#include <string.h>
#include "cmd_entry.h"

int get_cmd(FILE *fp, char *line, int size)
{
    int c, length, copied;
    copied = 0;

    for(length = 0; (c = fgetc(fp)) != EOF && c != '\0' && c != '\n'; length++) {
        if(copied < (size - 1)) {
            line[copied++] = c;
        } else 
            break;
    }

    line[length] = '\0';

    if(c == EOF && length == 0) {
        return -1;
    } else {
        return length;
    }
}

void print_usage(struct cmd_entry *entrys)
{
	int i;
	struct cmd_entry *p = NULL;

	printf("==== usage: ====\n");
	for (i = 0; 1; i++) {
		p = entrys + i;
		if (p->cmd == NULL)
			break;
		PRINT_CMD_ENTRY(p->cmd, p->desc);
	}
	PRINT_CMD_ENTRY("help", "print help message");
	PRINT_CMD_ENTRY("quit", "quit programe");
}

int run_cmd_entry(int fd, char *cmd, struct cmd_entry *entrys,
		int argc, char **argv)
{
	int ret = 0, i;
	struct cmd_entry *p;
	struct cmd_args args;

	if (!strcmp(cmd, "quit"))
		return 1;
	if (!strcmp(cmd, "help")) {
		print_usage(entrys);
		return 0;
	}

	args.fd = fd;
	args.argc = argc - 2;
	args.argv = argv + 2;

	for (i = 0; 1; i++) {
		p = entrys + i;
		if (p->cmd == NULL) {
			printf("==== unknow command entry '%s' ====\n", cmd);
			print_usage(entrys);
			break;
		}
		if (strcmp(cmd, p->cmd))
			continue;
		printf("==== run cmd '%s' ====\n", cmd);
		ret = p->func(&args, p);
		printf("--> '%s' result: %d\n", cmd, ret);
		break;
	}

	return 0;
}
