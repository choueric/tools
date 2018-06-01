/*
 * enter.cpp
 *
 *  Created on: 2014-11-25
 *      Author: 50215238
 */

#include "cmd_entry.h"
#include <string.h>
#include <stdio.h>

void cmd_entry_usage(struct cmd_entry *entrys, int n)
{
	int i;
	struct cmd_entry *p = NULL;

	printf("usage: \n");
	for (i = 0; i < n; i++) {
		p = entrys + i;
		printf("    %s\n", p->cmd);
	}
}

int cmd_entry_match(char *cmd, struct cmd_entry *entrys, int n)
{
	int i = 0;
	int ret = -1;
	struct cmd_entry *entry = NULL;

	for (i = 0; i < n; i++) {
		entry = entrys + i;
		if (!strcmp(cmd, entry->cmd)) {
			ret = i;
			break;
		}
	}
	if (i == n) {
		ret = -1;
	}

	return ret;
}
