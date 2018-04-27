/*
 * 用以计算时间间隔的宏
 * 第一次调用时因为tv1的值为0，间隔时间即为调用的当前时间
 * kernel space
 */
#include <linux/time.h>

static struct timeval tv1 = {0}, tv2 = {0};

#define GET_TIME(str) do {\
	do_gettimeofday(&tv2);\
	printk("--> %s [%ld]\n", str, 1000000 * (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec));\
	tv1 = tv2;\
} while (0)

void do_edvlog(const char *format, ...)
{
	char str[EDV_LOG_LEN] = {0};
	char *substr = NULL;
	va_list args;
	struct timeval tv;

	va_start(args, format);
	
	do_gettimeofday(&tv);
	snprintf(str, EDV_LOG_LEN, "[%ld.%06ld] ", tv.tv_sec, tv.tv_usec);
	substr = str + strlen(str);
	vsnprintf(substr, EDV_LOG_LEN, format, args);
	
	va_end(args);
	
    printf("%s\n", str);
}

