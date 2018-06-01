#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <errno.h>
#include <poll.h>
#include <linux/soundcard.h>
#include <linux/input.h>
#include <linux/spi/spidev.h>
#include <termios.h>
#include <linux/serial.h>
#include <linux/i2c.h>
#include <stdarg.h>
#include <signal.h>
#include <ctype.h>
#include <sys/uio.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <linux/tcp.h>
#include <linux/ethtool.h>
#include <signal.h>
#include <bits/signum.h>
#include <execinfo.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/mount.h>

//在debug中定义_M_PRINTF_宏，让printf打印信息；release中不定义_M_PRINTF_宏
#ifdef _NO_PRINTF_

#define printf
#define fprintf
#define perror

#endif

#endif
