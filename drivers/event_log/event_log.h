
#define TYPE_OUT 0
#define TYPE_IN  1
#define TYPE_RX_FIFO_DEPTH 2
#define TYPE_INTERRUPT 3
#define TYPE_RX_FULL 4
#define TYPE_STATUS 5
#define TYPE_PN_INTERUPT_S 6
#define TYPE_PN_INTERUPT_E 7
#define TYPE_PN_AFTER_READ 8
#define TYPE_HRTIMER_S 9
#define TYPE_HRTIMER_E 10
#define TYPE_BUSYWAIT_S 11
#define TYPE_BUSYWAIT_E 12

extern void apdu_record_dump(void);
extern void apdu_record_reset(void);
extern void apdu_record_add(uint32_t data, int type);
