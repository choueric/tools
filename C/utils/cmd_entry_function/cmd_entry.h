#ifndef _CMD_ENTRY_H
#define _CMD_ENTRY_H

#include <stdio.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#define PRINT_CMD_ENTRY(cmd, desc) printf("%10s: %s\n", cmd, desc)

struct cmd_entry;
struct cmd_args;

typedef int (*entry_func) (struct cmd_args *, struct cmd_entry *);

struct cmd_args {
	int fd;
	int argc;
	char **argv;
};

struct cmd_entry {
	char *cmd;
	entry_func func;
	char *desc;
};


int get_cmd(FILE *fp, char *line, int size);

/* 
 * build-in commands:
 * - quit: quit programe, return 1
 * - help: print usage information
 * @return: 1, means quit loop
 */
int run_cmd_entry(int fd, char *cmd, struct cmd_entry *entrys,
		int argc, char **argv);

void print_usage(struct cmd_entry *entrys);

#endif
