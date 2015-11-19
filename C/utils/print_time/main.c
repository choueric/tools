static struct timeval tv1 = {0}, tv2 = {0};

#define GET_TIME(str) do {\
	gettimeofday(&tv2, NULL);\
	printf("--> %s [%ld]\n", str, 1000000 * (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec));\
	tv1 = tv2;\
} while (0)

#define edvlog(notify, format, arg...) \
	do {\
		printk(KERN_DEBUG "edvlogd: %d(%s)@%s: " format "\n",\
				__LINE__, __func__, __FILE__, ##arg);\
		if (notify) \
			do_edvlog(__LINE__, __func__, __FILE__, notify, format, ##arg);\
	} while (0)



#define LOG_LEN 200
static  do_log(int line, const char *func, const char *file, const char *format, ...)
{
	char str[LOG_LEN] = {0};
	char substr[LOG_LEN] = {0};
	va_list args;
	struct timeval tv;
	unsigned long flags;

	va_start(args, format);
	gettimeofday(&tv, NULL);
	vsnprintf(substr, LOG_LEN, format, args);
	snprintf(str, LOG_LEN, "[%ld.%06ld] [%d(%s)@%s]: %s", tv.tv_sec, tv.tv_usec, line, func, file, substr);
	va_end(args);
	
    printf("%s", str);
}

#define log(format, arg...) \
	do {\
		do_edvlog(__LINE__, __func__, __FILE__, format, ##arg);\
	} while (0)

