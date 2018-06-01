#include <stdio.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <errno.h>
#include <sys/mman.h>

#define VGA_XRES 800
#define VGA_YRES 600
#define VGA_HFP 0x27
#define VGA_HBP 0x57
#define VGA_HSW 0x3F
#define VGA_VFP 0x01
#define VGA_VBP 0x17
#define VGA_VSW 0x03
#define VGA_PIXCLOCK 27777 // 36MHz

static void print_var(struct fb_var_screeninfo *var)
{
	printf("====================\n");
	printf("left_margin(HBP): \t%d\n", var->left_margin);
    printf("right_margin(HFP): \t%d\n", var->right_margin);
    printf("hsync_len(HSW): \t%d\n", var->hsync_len);

    printf("upper_margin(VBP): \t%d\n", var->upper_margin);
    printf("lower_margin(VFP): \t%d\n", var->lower_margin);
    printf("vsync_len(VSW): \t%d\n", var->vsync_len);
	printf("====================\n");
}

static void set_var(struct fb_var_screeninfo *var,
		int hbp, int hfp, int hsw,
		int vbp, int vfp, int vsw)
{
    var->left_margin = hbp;
    var->right_margin = hfp;
    var->hsync_len = hsw;

    var->upper_margin = vbp;
    var->lower_margin = vfp;
    var->vsync_len = vsw;

    var->activate = FB_ACTIVATE_NOW;
}

int main(int argc, char **argv)
{
	int fd = 0;
	struct fb_var_screeninfo vinfo;
    int ret = 0;
	int just_read = 0;

	int hbp, hfp, hsw;
	int vbp, vfp, vsw;

	if (argc == 1) {
		just_read = 1;
	} else if (argc != 7) {
        printf("usage: ./setfb HBP HFP HSW VBP VFP VSW\n");
        return 1;
    }

	fd = open("/dev/fb0", O_RDWR);
	if (fd < 0) {
		printf("Error: cannot open framebuffer device.\n");
		exit(3);
	}

	/* Get variable screen information */
	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
		printf("Error reading variable information.\n");
		close(fd);
		exit(4);
	}
	print_var(&vinfo);

	if (just_read)
		goto L_OUT;

	hbp = atoi(argv[1]);
	hfp = atoi(argv[2]);
	hsw = atoi(argv[3]);
	vbp = atoi(argv[4]);
	vfp = atoi(argv[5]);
	vsw = atoi(argv[6]);

	set_var(&vinfo, hbp, hfp, hsw, vbp, vfp, vsw);
	ret = ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo);
	if (ret) {
		perror("set to vga failed");
	} else
		print_var(&vinfo);

L_OUT:
	close(fd);
	return ret;
}
