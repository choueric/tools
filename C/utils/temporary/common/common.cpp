/*
 * common routines for utility
 */

#include "common.h"

#define BLACK "\E[30m"
#define RED "\E[31m"
#define GREEN "\E[32m"
#define YELLOW "\E[33m"
#define BLUE "\E[34m"
#define MAGENTA "\E[35m"
#define CYAN "\E[36m"
#define WHITE "\E[37m"
#define END "\E[0m"

#define LOG_LEN 100

int GetTestcaseId(const char *opt, struct testcase *p, int num)
{
	int i = -1;

	for (i = 0; i < num; i++) {
		if (!strncmp(opt, p[i].opt, strlen(opt)) &&
				!strcmp(opt, p[i].opt)) {
			return i;
		}
	}

	return -1;
}

void print_result(int res)
{
	if (0 == res) {
		goodlog("====================== success =====================\n");
	} else if (2 == res) {
		infolog("====================== not test ====================\n");
	} else {
		faillog("====================== failed ======================\n");
	}
}

void print_usage(struct testcase *p, int num)
{
	int i;
	printf(CYAN "*********************************************************************\n");
	printf("\tOptions\t\t\tDescription\n");
	printf("---------------------------------------------------------\n");
	for(i = 0; i < num; i++){
		printf("\t%s\t\t\t%s\n", p[i].opt, p[i].dscrp);
	}
	printf("*********************************************************************\n\n" END);
}

void dump_mem2file(const char *name, const unsigned char *buf, int len)
{
	FILE *fp = NULL;

	fp = fopen(name, "w+");
	if (fp == NULL) {
		perror("fopen failed.");
		return;
	}

	dump_mem(fp, buf, len, "dump to file");
	fclose(fp);
}

void dump_mem(FILE *fp, const unsigned char *buf, int len, const char *desc)
{
	if (desc)
		fprintf(fp, "%s:\n", desc);

	fprintf(fp, "--------------------------------------------------------\n");
	for (int i = 0; i < len; i++) {
		fprintf(fp, "0x%02x ", (unsigned char)buf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(fp, "\n");
	}

	fprintf(fp, "\n--------------------------------------------------------\n");
}

void signal_handler(int sig)
{
	printf("signal_handler, signal: %d\n", sig);
	exit(0);
}

void print_title(const char* name)
{
	printf(CYAN "\n================== %s ===================\n" END, name);
}

void faillog(const char *format, ...)
{
	static char str[LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);

	gettimeofday(&tv, NULL);
	snprintf(str, LOG_LEN, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, LOG_LEN, format, args);

	va_end(args);

	printf(RED "%s\n" END, str);
}

void goodlog(const char *format, ...)
{
	static char str[LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);

	gettimeofday(&tv, NULL);
	snprintf(str, LOG_LEN, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, LOG_LEN, format, args);

	va_end(args);

	printf(GREEN "%s\n" END, str);
}


void infolog(const char *format, ...)
{
	static char str[LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);

	gettimeofday(&tv, NULL);
	snprintf(str, LOG_LEN, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, LOG_LEN, format, args);

	va_end(args);

	printf(YELLOW "%s\n" END, str);
}

void errlog(const char *format, ...)
{
	static char str[LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);

	gettimeofday(&tv, NULL);
	snprintf(str, LOG_LEN, "++ %ld.%06ld ++ ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, LOG_LEN, format, args);

	va_end(args);

	printf(RED "%s\n" END, str);
}
