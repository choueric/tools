
/* #define DEBUG_APDU_RECORD */

#ifdef DEBUG_APDU_RECORD
struct apdu_record {
	struct timeval tv;
	int type;
	uint32_t data;
};

static char *apdu_type_strs[] = {
    "OUT", " IN", "RXD", "INT",
    "RXF", "STS", "PNS", "PNE",
    "PNR", "HRS", "HRE", "BWS",
    "BWE",
};

static char *interrupt_str[] = {
	"PAR", "WT", "CT", "TC",
	"RXE", "RXT", "RXF", "TXE",
};

static char *interrupt_to_str(uint8_t intstatus)
{
	static char str[128];
	int i = 0, index = 0;

	memset(str, 0, 128);
	for (i = 0; i < 8; i++, intstatus = intstatus >> 1) {
		if (!(intstatus & 1))
			continue;
		index += sprintf(str + index, "%s ", interrupt_str[i]);
	}
	return str;
}

#define APDU_RECORD_MAX_NUM 512
static struct apdu_record apdu_record_array[APDU_RECORD_MAX_NUM];
static int apdu_record_count = 0;
static int apdu_record_num = 0;
static bool apdu_record_dumped = false;

void apdu_record_add(uint32_t data, int type)
{
	struct apdu_record *prec = NULL;
	static bool start_recording = false;

	// SELECT command response
	if (type == TYPE_IN && data == 0xa4)
		start_recording = true;

	if (!start_recording)
		return;

	if (apdu_record_count >= APDU_RECORD_MAX_NUM)
		return;

	prec = &apdu_record_array[apdu_record_count++];
	do_gettimeofday(&prec->tv);
	prec->type = type;
	prec->data = data;
}

void apdu_record_dump(void)
{
	int i = 0;
	struct apdu_record *prec = NULL;

#if 0
	static bool dir_in = true;
	printk("-------------- %d ----------------------", apdu_record_num);
	for (i = 0; i < apdu_record_count; i++) {
		prec = &apdu_record_array[i];
		if (i == 0)
			dir_in = !prec->dir_in;
		if (prec->dir_in != dir_in) {
			dir_in = prec->dir_in;
			printk("\n[%ld:%06ld]: %s\n", prec->tv.tv_sec, prec->tv.tv_usec, prec->dir_in ? " in": "out");
		}
		printk("%02x ", prec->data);
	}
	printk("\n");
#else
	static struct timeval t;
	int usec;

	if (apdu_record_dumped) {
		printk(">>> apdu dumped\n");
		return;
	}
	apdu_record_dumped = true;
	printk("-------------- %d ----------------------\n", apdu_record_num);
	for (i = 0; i < apdu_record_count; i++) {
		prec = &apdu_record_array[i];
		usec = (prec->tv.tv_sec - t.tv_sec)*1000000;
		usec += (prec->tv.tv_usec - t.tv_usec);
		t = prec->tv;
		if (prec->type == TYPE_INTERRUPT) {
			char *data = interrupt_to_str(prec->data);
			printk("[%4d][%ld:%06ld]: %s: %4dus: %02x %s\n", i,
			       prec->tv.tv_sec, prec->tv.tv_usec,
			       apdu_type_strs[prec->type], usec, prec->data, data);
		} else {
			printk("[%4d][%ld:%06ld]: %s: %4dus: %02x\n", i,
			       prec->tv.tv_sec, prec->tv.tv_usec,
			       apdu_type_strs[prec->type], usec, prec->data);
		}
	}
#endif
}

void apdu_record_reset(void)
{
	apdu_record_count = 0;
	apdu_record_dumped = false;
	apdu_record_num++;
}
#else
void apdu_record_add(uint32_t data, int type){}
void apdu_record_dump(void){}
void apdu_record_reset(void){}
#endif

void iso_dev_outb(struct sc_dev *dev, unsigned long value)
{
#ifdef DEBUG_APDU_RECORD
	apdu_record_add(value, 0);
#endif
	iso_dev_write_reg(dev, SCIB_SCTXB, value);
}

unsigned long iso_dev_inb(struct sc_dev *dev)
{
	uint8_t val = iso_dev_read_reg(dev, SCIB_SCRXB);
#ifdef DEBUG_APDU_RECORD
	apdu_record_add(val, TYPE_IN);
#endif
	return val;
}

