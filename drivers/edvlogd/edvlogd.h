#ifndef _LINUX_EDVLOGD_H
#define _LINUX_EDVLOGD_H

#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/usb.h>

#ifdef CONFIG_EDVLOGD
#define EDV_LOG_LEN 200
#define EDV_BUFFER_SIZE 128

////////////////////////////////////// debug for echi_irq /////////////////////////////////
#define EHCI_MODULE_OVERLOAD 0
//#define EDV_EHCI_IRQLOCK

//#define is_match_module(urb) (!strcmp(urb->dev->serial, "100062"))
//#define is_match_module(urb) (urb->dev->portnum == 6)
#define is_match_module(urb) (0)  // always false to get rid of debug infomation

#if EHCI_MODULE_OVERLOAD
#define edv_timestamp(format, arg...) \
    do {\
        struct timeval tv;\
        do_gettimeofday(&tv);\
        printk(KERN_DEBUG "[%ld:%06ld]:" format "\n",\
                tv.tv_sec, tv.tv_usec, ##arg);\
    } while (0)


#define edv_print_buf(urb) do {\
    unsigned char *buf = (unsigned char *)urb->transfer_buffer;\
    int seq = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];\
    printk("portnum[%d]: %02x %d[%02x %02x %02x %02x]:\n",\
            urb->dev->portnum, buf[0], seq,\
            buf[4], buf[5], buf[6], buf[7]);\
    } while(0)

#define edv_irq_print(format, arg...) printk(format, ##arg)

#else
#define edv_timestamp(format, arg...) do {} while (0)
#define edv_print_buf(urb) do {} while (0)
#define edv_irq_print(format, arg...)

#endif

#if 0
#define edv_timestamp_irq(format, arg...) \
    do {\
        struct timeval tv;\
        do_gettimeofday(&tv);\
        printk(KERN_DEBUG "[%ld:%06ld]:" format "\n",\
                tv.tv_sec, tv.tv_usec, ##arg);\
    } while (0)
#else
#define edv_timestamp_irq(format, arg...) do {} while (0)
#endif
////////////////////////////////////// debug for echi_irq end /////////////////////////////

struct edvlogd_item {
    char str[EDV_LOG_LEN];
    struct timeval tv;
    int is_logged;
};

/* just one reader, multi writer */
struct edvlogd_t {
    struct edvlogd_item buffer[EDV_BUFFER_SIZE];
    int size;
    int write;
    int read;

    int is_init;
    spinlock_t buf_lock;
    wait_queue_head_t wq;
};

struct edvlogd_t *edvlogd_get_data(void);

int edvlogd_get_count(struct edvlogd_t *p);

void do_edvlog(int line, const char *func, const char *file, int level, const char *format, ...);

#define edvlogd_get_space(p) ((p)->size - edvlogd_get_count((p)) - 1)
#define edvlogd_is_empty(p) (edvlogd_get_count((p)) == 0)
#define edvlogd_is_full(p) (edvlogd_get_space((p)) == 0)

/* used in edvlog() */
#define EDV_ERR 3
#define EDV_WAR 2
#define EDV_INFO 1
#define EDV_NO_NOTIFY 0

#define edvlog(notify, format, arg...) \
    do {\
        printk(KERN_DEBUG "edvlogd: %d(%s)@%s: " format "\n",\
                __LINE__, __func__, __FILE__, ##arg);\
        if (notify) \
            do_edvlog(__LINE__, __func__, __FILE__, notify, format, ##arg);\
    } while (0)

/* precondition: check the buffer is not empty */
int edvlogd_read(char *buf);

#define edvlogd_inc(index) ((index) = (index + 1) & (EDV_BUFFER_SIZE - 1))

#else
#define edvlog(notify, format, arg...) \
    do {\
    } while (0)
#endif

#endif
