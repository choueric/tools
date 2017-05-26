/*
 *      Real Time Clock Driver Test/Example Program
 *
 *      Compile with:
 *		     gcc -s -Wall -Wstrict-prototypes rtctest.c -o rtctest
 *
 *      Copyright (C) 1996, Paul Gortmaker.
 *
 *      Released under the GNU General Public License, version 2,
 *      included herein by reference.
 *
 */

#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Normally this offset is 1900. It minuses 100 because set_time and get_time 
// in the driver of rx-8010 minus and add 100.
#define YEAR_OFFSET (1900 - 100)

#include "args.c"

void real_time_2_tm(int year, int mon, int mday, int hour, int min, int sec,
		struct rtc_time *tm)
{
	tm->tm_year = year - YEAR_OFFSET;
	tm->tm_mon = mon;
	tm->tm_mday = mday;
	tm->tm_hour = hour;
	tm->tm_min = min;
	tm->tm_sec = sec;
}

void print_tm(struct rtc_time *tm)
{
	int year = tm->tm_year + YEAR_OFFSET;
	printf("\t%d-%d-%d %d:%d:%d\n", year, tm->tm_mon, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
}

static int test_set_time(int fd)
{
	int ret;
	struct rtc_time time;
	printf("==== set time ====\n");

	real_time_2_tm(1987, 2, 27, 12, 30, 15, &time);
	ret = ioctl(fd, RTC_SET_TIME, &time);
	if (ret) {
		printf("set time failed: %s\n", strerror(errno));
		return ret;
	}
	printf("\tset time OK.\n");
	return 0;
}

static int test_read_time(int fd)
{
	int ret;
	struct rtc_time time;
	printf("==== read time ====\n");

	ret = ioctl(fd, RTC_RD_TIME, &time);
	if (ret) {
		printf("get time failed: %s\n", strerror(errno));
		return ret;
	}
	printf("\tget time OK.\n");
	print_tm(&time);
	return 0;
}

static int test_low_voltage(int fd)
{
	int ret;
	int state;

	printf("==== voltage low operatoin ====\n");

	ret = ioctl(fd, RTC_VL_READ, &state);
	if (ret) {
		printf("read voltage low failed: %s\n", strerror(errno));
		return ret;
	}
	printf("\tvoltage low: %s\n", state ? "Low": "Normal");

	if (state) {
		printf("\tclear voltage low flag\n");
		ret = ioctl(fd, RTC_VL_CLR, NULL);
		if (ret) {
			printf("clear voltage low failed: %s\n", strerror(errno));
			return ret;
		}
		printf("\tOK.\n");
	}

	return 0;
}

static int auto_test(int fd)
{
	fprintf(stderr, "\nRTC Driver Auto Test.\n\n");

	test_set_time(fd);
	test_low_voltage(fd);
	test_read_time(fd);

	return 0;
}

int main(int argc, char **argv)
{
	int i, fd, retval, irqcount = 0;
	unsigned long tmp, data;
	struct rtc_time rtc_tm;

	parse_args(argc, argv);
	print_options();

	fd = open(rtcdev, O_RDONLY);

	if (fd == -1) {
		perror(rtcdev);
		exit(errno);
	}

	if (autotest) {
		auto_test(fd);
		goto done;
	}

	if (!strcmp(cmd, "read")) {
		test_read_time(fd);
	} else if (!strcmp(cmd, "set")) {
		test_set_time(fd);
	} else if (!strcmp(cmd, "vl")) {
		test_low_voltage(fd);
	} else {
		printf("invalid command %s\n", cmd);
	}
	goto done;

	/* Turn on update interrupts (one per second) */
	retval = ioctl(fd, RTC_UIE_ON, 0);
	if (retval == -1) {
		if (errno == ENOTTY) {
			fprintf(stderr,
				"\n...Update IRQs not supported.\n");
			goto test_READ;
		}
		perror("RTC_UIE_ON ioctl");
		exit(errno);
	}

	fprintf(stderr, "Counting 5 update (1/sec) interrupts from reading %s:", rtcdev);
	fflush(stderr);
	for (i=1; i<6; i++) {
		/* This read will block */
		retval = read(fd, &data, sizeof(unsigned long));
		if (retval == -1) {
			perror("read");
			exit(errno);
		}
		fprintf(stderr, " %d",i);
		fflush(stderr);
		irqcount++;
	}

	fprintf(stderr, "\nAgain, from using select(2) on /dev/rtc:");
	fflush(stderr);
	for (i=1; i<6; i++) {
		struct timeval tv = {5, 0};     /* 5 second timeout on select */
		fd_set readfds;

		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		/* The select will wait until an RTC interrupt happens. */
		retval = select(fd+1, &readfds, NULL, NULL, &tv);
		if (retval == -1) {
		        perror("select");
		        exit(errno);
		}
		/* This read won't block unlike the select-less case above. */
		retval = read(fd, &data, sizeof(unsigned long));
		if (retval == -1) {
		        perror("read");
		        exit(errno);
		}
		fprintf(stderr, " %d",i);
		fflush(stderr);
		irqcount++;
	}

	/* Turn off update interrupts */
	retval = ioctl(fd, RTC_UIE_OFF, 0);
	if (retval == -1) {
		perror("RTC_UIE_OFF ioctl");
		exit(errno);
	}

test_READ:
	/* Read the RTC time/date */
	retval = syscall(SYS_ioctl, fd, RTC_RD_TIME, &rtc_tm);
	if (retval < 0) {
		printf("ret = %d\n", retval);
		perror("RTC_RD_TIME ioctl");
		exit(errno);
	}

	fprintf(stderr, "\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
		rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	/* Set the alarm to 5 sec in the future, and check for rollover */
	rtc_tm.tm_sec += 5;
	if (rtc_tm.tm_sec >= 60) {
		rtc_tm.tm_sec %= 60;
		rtc_tm.tm_min++;
	}
	if (rtc_tm.tm_min == 60) {
		rtc_tm.tm_min = 0;
		rtc_tm.tm_hour++;
	}
	if (rtc_tm.tm_hour == 24)
		rtc_tm.tm_hour = 0;

	retval = ioctl(fd, RTC_ALM_SET, &rtc_tm);
	if (retval == -1) {
		if (errno == ENOTTY) {
			fprintf(stderr,
				"\n...Alarm IRQs not supported.\n");
			goto test_PIE;
		}
		perror("RTC_ALM_SET ioctl");
		exit(errno);
	}

	/* Read the current alarm settings */
	retval = ioctl(fd, RTC_ALM_READ, &rtc_tm);
	if (retval == -1) {
		perror("RTC_ALM_READ ioctl");
		exit(errno);
	}

	fprintf(stderr, "Alarm time now set to %02d:%02d:%02d.\n",
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	/* Enable alarm interrupts */
	retval = ioctl(fd, RTC_AIE_ON, 0);
	if (retval == -1) {
		perror("RTC_AIE_ON ioctl");
		exit(errno);
	}

	fprintf(stderr, "Waiting 5 seconds for alarm...");
	fflush(stderr);
	/* This blocks until the alarm ring causes an interrupt */
	retval = read(fd, &data, sizeof(unsigned long));
	if (retval == -1) {
		perror("read");
		exit(errno);
	}
	irqcount++;
	fprintf(stderr, " okay. Alarm rang.\n");

	/* Disable alarm interrupts */
	retval = ioctl(fd, RTC_AIE_OFF, 0);
	if (retval == -1) {
		perror("RTC_AIE_OFF ioctl");
		exit(errno);
	}

test_PIE:
	/* Read periodic IRQ rate */
	retval = ioctl(fd, RTC_IRQP_READ, &tmp);
	if (retval == -1) {
		/* not all RTCs support periodic IRQs */
		if (errno == ENOTTY) {
			fprintf(stderr, "\nNo periodic IRQ support\n");
			goto done;
		}
		perror("RTC_IRQP_READ ioctl");
		exit(errno);
	}
	fprintf(stderr, "\nPeriodic IRQ rate is %ldHz.\n", tmp);

	fprintf(stderr, "Counting 20 interrupts at:");
	fflush(stderr);

	/* The frequencies 128Hz, 256Hz, ... 8192Hz are only allowed for root. */
	for (tmp=2; tmp<=64; tmp*=2) {

		retval = ioctl(fd, RTC_IRQP_SET, tmp);
		if (retval == -1) {
			/* not all RTCs can change their periodic IRQ rate */
			if (errno == ENOTTY) {
				fprintf(stderr,
					"\n...Periodic IRQ rate is fixed\n");
				goto done;
			}
			perror("RTC_IRQP_SET ioctl");
			exit(errno);
		}

		fprintf(stderr, "\n%ldHz:\t", tmp);
		fflush(stderr);

		/* Enable periodic interrupts */
		retval = ioctl(fd, RTC_PIE_ON, 0);
		if (retval == -1) {
			perror("RTC_PIE_ON ioctl");
			exit(errno);
		}

		for (i=1; i<21; i++) {
			/* This blocks */
			retval = read(fd, &data, sizeof(unsigned long));
			if (retval == -1) {
				perror("read");
				exit(errno);
			}
			fprintf(stderr, " %d",i);
			fflush(stderr);
			irqcount++;
		}

		/* Disable periodic interrupts */
		retval = ioctl(fd, RTC_PIE_OFF, 0);
		if (retval == -1) {
			perror("RTC_PIE_OFF ioctl");
			exit(errno);
		}
	}

done:
	fprintf(stderr, "\n*** Test complete ***\n");

	close(fd);

	return 0;
}
