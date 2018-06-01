/*
 * console_ui.cpp
 *
 *  Created on: 2014-7-25
 *      Author: 50215238
 */

#include <stdio.h>
#include <stdlib.h>

#include "MM_Types.h"
#include "ui.h"

int get_line(char *buf, int len)
{
	char c;
	int count = len - 1;
	int i = 0;

	while ( (c = fgetc(stdin)) != '\n' && c != EOF && i < count)
		buf[i++] = c;
	buf[i] = '\0';

	return i;
}

int read_input_data(uint8 *buf, int len)
{
	char str[10];
	int n, i;
	unsigned char c;

	for (n = 0; n < len; n++) {
		i = 0;
		while ( (c = fgetc(stdin)) == '\n' || c == ' ' || c == '\t');
		do {
			str[i++] = c;
			c = fgetc(stdin);
			if (c == '\n' || c == ' ' || c == '\t')
				break;
		} while (i < 10);
		str[i] = '\0';
		buf[n] = strtol(str, NULL, 0);
	}

	return n;
}


/*
 * read one line, and parse data in this line.
 * return when meet '\n' or number of data equals @len
 */
int read_input_data_one_line(uint8 *buf, int len)
{
	char str[10];
	int n, i;
	unsigned char c;

	for (n = 0; n < len; n++) {
		i = 0;
		while ( (c = fgetc(stdin)) == '\t' || c == ' ');
		if (c == '\n')
			break;
		do {
			str[i++] = c;
			c = fgetc(stdin);
			if (c == '\n' || c == ' ' || c == '\t')
				break;
		} while (i < 10);
		str[i] = '\0';
		buf[n] = strtol(str, NULL, 0);
		if (c == '\n')
			break;
	}

	return n;
}
