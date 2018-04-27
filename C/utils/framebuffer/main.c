#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>
#include <stdio.h>  
#include <fcntl.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>  

static int Xres, Yres;

static int rgb2yuv(double r, double g, double b)
{
    double y = 0.299 * r + 0.587 * g + 0.114 * b;
    double u = -0.147 * r - 0.289 * g + 0.436 * b;
    double v = 0.615 * r - 0.515 * g - 0.100 * b;

    return 0;
}

/* y 16-235, u,v 16-240 */
static int yuv2rgb(double y, double u, double v)
{
    int ret = 0;
#if 1
    double dr = y + 1.14 * v;
    double dg = y - 0.39 * u - 0.58 * v;
    double db = y + 2.03 * u;
#else
    double dr = y + 1.4075 * (v - 128);  
    double dg = y - 0.3455 * (u - 128) - 0.7169 * (v - 128);  
    double db = y + 1.779 * (u - 128); 
#endif

    int r = (int)dr;
    int g = (int)dg;
    int b = (int)db;

    printf("(0x%02x, 0x%02x, 0x%02x) --> (%d(0x%02x), %d(0x%02x), %d(0x%02x))\n",
            (int)y, (int)u, (int)v, r, r, g, g, b, b);

    ret = b & 0xff;
    ret |= (g & 0xff) << 8;
    ret |= (r & 0xff) << 16;

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
int full_color(char *fbp, int fbfd, int xres, int yres, int value)
{
	int index = 0;
	unsigned int *buf = (unsigned int *)fbp;
	int x, y;

	for (y = 0; y < yres; y++) {
		for (x = 0; x < xres; x++) {
			index = y * xres +x;
			buf[index] = value;
		}
	}

	return 0;
}

// RGB888
int full_color_test(char *fbp, int size, int fbfd, int xres, int yres)
{
	printf("size = %d\n", size);
    // white
	memset(fbp, 0xff, size);
	sleep(1);
    // blue
	full_color(fbp, fbfd, xres, yres, 0x000000ff);
	sleep(1);
    // green
	full_color(fbp, fbfd, xres, yres, 0x0000ff00);
	sleep(1);
    // red
	full_color(fbp, fbfd, xres, yres, 0x00ff0000);
	sleep(1);


	return 0;
}

////////////////////////////////////////////////////////////////////////////////

static void draw_line_h(char *fbp, int x1, int y1, int x2, int y2)
{
	int index = 0;
	unsigned int *buf = (unsigned int *)fbp;
	int x, y;

	for (x = x1, y = y1; x < (x2-1); x++) {
		index = y * Xres + x;
		buf[index] = 0xff00ff;
	}
}

static void draw_line_v(char *fbp, int x1, int y1, int x2, int y2)
{
	int index = 0;
	unsigned int *buf = (unsigned int *)fbp;
	int x, y;

	for (x = x1, y = y1; y < y2-1; y++) {
		index = y * Xres + x;
		buf[index] = 0x00ff00;
	}
}

// TODO
static void draw_line_oblique_down(char *fbp, int x1, int y1, int x2, int y2)
{
	int i, j;
	int t = 0;

	for (i = x1, j = y1; i <= x2; i++, j++) {
		printf("    %d.%d -> %d.%d\n", i, j, i+1, j+1);
		draw_line_v(fbp, i, j, i, j+1);
		draw_line_h(fbp, i, j, i+1, j);
	}
	printf("-------\n");
}

static void draw_line_oblique_up(char *fbp, int x1, int y1, int x2, int y2)
{
	int i, j;
	int t = 0;

	for (i = x1, j = y1; i <= x2, j > 0; i++, j--) {
		printf("    %d.%d -> %d.%d\n", i, j, i+1, j+1);
		draw_line_v(fbp, i, j, i, j-1);
		draw_line_h(fbp, i, j, i+1, j);
	}
	printf("-------\n");
}

static void draw_line_oblique(char *fbp, int x1, int y1, int x2, int y2)
{
	int i, j;

	printf("draw: %d.%d -> %d.%d\n", x1, y1, x2, y2);

	if (x1 > x2) {
		i = x1;
		j = y1;
		x1 = x2;
		x2 = i;
		y1 = y2;
		y2 = j;
	}
	if (y1 > y2) {
		draw_line_oblique_down(fbp, x1, y1, x2, y2);
	} else {
		draw_line_oblique_up(fbp, x1, y1, x2, y2);
	}
}

static void draw_line(char *fbp, int x1, int y1, int x2, int y2)
{ x2--; x1--; y1--; y2--;
	if (x1 == x2)
		draw_line_v(fbp, x1, y1, x2, y2);
	else if (y1 == y2)
		draw_line_h(fbp, x1, y1, x2, y2);
	else
		draw_line_oblique(fbp, x1, y1, x2, y2);
}

static void draw_rect(char *fbp, int x, int y, int w, int h)
{
	int i = 0;
	for (i = 0; i < w; i++) {
		draw_line(fbp, x + i, y, x + i, y + h);
	}
}

int rect_test(char *fbp, int size, int fbfd, int xres, int yres)
{
	memset(fbp, 0x00, size);

	draw_rect(fbp, 40, 40, 50, 50);

	draw_line(fbp, 100, 5, 300, 5);

	draw_line(fbp, 1, 1, 1, yres - 1);
	draw_line(fbp, 1, 1, xres - 1, 1);

	draw_line(fbp, xres - 200, yres - 1, xres, yres - 1);
	draw_line(fbp, xres , yres - 200, xres, yres);

	return 0;
}

int line_test(char *fbp, int size, int fbfd, int xres, int yres)
{
	int x0, y0;
	int x1, y1;
	int t = 0;

	while (1) {
		int i, j;
		for (j = 10; j < (yres - 20); j += 11) {
			for (i = 10; i < xres; i += 10) {
				x0 = i;
				y0 = j;
				x1 = i;
				y1 = j + 9;
				sleep(1);
				draw_line(fbp, x0, y0, x1, y1);
			}
		}
		memset(fbp, 0x00, size);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

#define T1_XRES 480
#define T1_YRES 272
#define T1_HFP 0x05
#define T1_HBP 0x28
#define T1_HSW 0x02
#define T1_VFP 0x08
#define T1_VBP 0x08
#define T1_VSW 0x02
#define T1_PIXCLOCK 111111 // 9MHz
static void set_var_info(int fd, struct fb_var_screeninfo *var)
{
    int ret = 0;

    var->xres = T1_XRES;
    var->yres = T1_YRES;
    var->xres_virtual = T1_XRES;
    var->yres_virtual = T1_YRES;

    var->left_margin = T1_HBP;
    var->right_margin = T1_HFP;
    var->hsync_len = T1_HSW;

    var->upper_margin = T1_VFP;
    var->lower_margin = T1_VBP;
    var->vsync_len = T1_VSW;

	var->pixclock = T1_PIXCLOCK;
    var->activate = FB_ACTIVATE_NOW;

    ret = ioctl(fd, FBIOPUT_VSCREENINFO, var);
    if (ret) {
        perror("set to t1 failed");
    }
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])  
{  
	int fbfd = 0;  
	struct fb_var_screeninfo vinfo;  
	struct fb_fix_screeninfo finfo;
	unsigned long screensize = 0;  
	char *fbp = 0;  
	char *dev = NULL, *cmd = NULL;

	if (argc == 2) {
		dev = argv[1];
	} else if (argc == 3) {
		dev = argv[1];
		cmd = argv[2];
	} else if (argc == 1) {
		dev = "/dev/fb0";
	} else {
		printf("invalid options\n");
		return -1;
	}

	// Open the file for reading and writing  
	fbfd = open(dev, O_RDWR);  
	if (!fbfd) {  
		printf("Error: cannot open framebuffer device.\n");  
		exit(1);  
	}  
	printf("--> %s opened successfully.\n", dev);  

	// Get variable screen information  
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {  
		printf("--> Error reading variable information.\n");  
		exit(1);  
	}  
	/* Get fixed screen information */
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)<0) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	printf("#### fix info ####\n");
	printf("  Line length = %d\n", finfo.line_length);
	printf("  Physical Address = %p\n", (void *)finfo.smem_start);
	printf("  Buffer Length = %d\n", finfo.smem_len);

	printf("#### var info ####\n");
	printf("  %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);  

	Xres = vinfo.xres;
	Yres = vinfo.yres;

	screensize = vinfo.xres * vinfo.yres * (vinfo.bits_per_pixel/8);
	// Map the device to memory  
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,  
			fbfd, 0);  
	if ((int)fbp == -1) {  
		printf("Error: failed to map framebuffer device to memory.\n");  
		perror("  ");
		exit(4);  
	}  
	printf("--> The framebuffer device was mapped to memory successfully.\n");  


#if 1
	// test 1
	full_color_test(fbp, screensize, fbfd, vinfo.xres, vinfo.yres);

	// test 2
	rect_test(fbp, screensize, fbfd, vinfo.xres, vinfo.yres);
#endif

	line_test(fbp, screensize, fbfd, vinfo.xres, vinfo.yres);
	

	sleep(50);

	munmap(fbp, screensize);  
	close(fbfd);  
	return 0;  
}  

