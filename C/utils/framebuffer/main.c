#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>
#include <stdio.h>  
#include <fcntl.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>  

static int Xres, Yres;

int test_color_1(char *fbp, int fbfd, int xres, int yres)
{
	int seg_len = 0;  
	int tmp_seg_len = 0;  
	int seg_num = 0;  
	unsigned int  rgb = 0;  
	unsigned int r = 0, g = 0, b = 0;  
	unsigned long location = 0;  
	int x = 0, y = 0;  

#if 1  
	seg_len = yres/6;  
	for (seg_num = 0; seg_num < 6; seg_num++) {  
		if (seg_num == 5)   
			tmp_seg_len = yres - seg_len*5;  
		else   
			tmp_seg_len = seg_len;  

		for (y = 0; y < tmp_seg_len; y++) {  
			for (x = 0; x < xres; x++) {  
				location = seg_num*seg_len*xres*4 + (y*xres+ x)*4;  
				switch (seg_num) {  
					case 0:  
						r = 0xff;  
						g = (0xff/seg_len)*y;  
						b = 0;  
						break;  
					case 1:  
						r = (0xff/seg_len)*(seg_len-y);  
						g = 0xff;  
						b = 0;  
						break;  
					case 2:  
						r = 0;  
						g = 0xff;  
						b = (0xff/seg_len)*y;   
						break;  
					case 3:  
						r = 0;  
						g = (0xff/seg_len)*(seg_len-y);  
						b = 0xff;  
						break;  
					case 4:  
						r =  (0xff/seg_len)*y;    
						g = 0;  
						b = 0xff;  
						break;  
					case 5:  
						r = 0xff;  
						b = (0xff/seg_len)*(seg_len-y);   
						g = 0;  
						break;  
					default:  
						printf("%s--%d:unknown seg_num %d\n", __FILE__, __LINE__, seg_num);  
						break;  
				}  

				r = (r*0x1f)/0xff;  
				g = (g*0x3f)/0xff;  
				b = (b*0x1f)/0xff;  
				rgb = (r << 11) | (g << 5) | b;  
				*((unsigned int*)(fbp + location)) = rgb;  
			}  
		}  
	}  

	sleep(2);  

	seg_len = yres/6;  
	for (seg_num = 0; seg_num < 6; seg_num++) {  
		if (seg_num == 5)   
			tmp_seg_len = yres - seg_len*5;  
		else   
			tmp_seg_len = seg_len;  

		for (y = 0; y < tmp_seg_len; y++) {  
			for (x = 0; x < xres; x++) {  
				location = seg_num*seg_len*xres*4 + (y*xres+ x)*4;  
				switch (seg_num) {  
					case 0://grey  
						r = 100;  
						g = 100;  
						b = 100;   
						break;  
					case 1: //black  
						r = 0x00;  
						g = 0x00;  
						b = 0x00;  
						break;  
					case 2://white  
						r = 0xff;  
						g = 0xff;  
						b = 0xff;  
						break;  
					case 3://red  
						r = 0xff;  
						g = 0;  
						b = 0;  
						break;  
					case 4: //green  
						r =  0;       
						g = 0xff;  
						b = 0;  
						break;  
					case 5: //blue  
						r = 0;  
						g = 0;  
						b = 0xff;   
						break;  
					default:  
						printf("%s--%d:unknown seg_num %d\n", __FILE__, __LINE__, seg_num);  
						break;    
				}  

				r = (r*0x1f)/0xff;  
				g = (g*0x3f)/0xff;  
				b = (b*0x1f)/0xff;  
				rgb = (r << 11) | (g << 5) | b;  
				*((unsigned int*)(fbp + location)) = rgb;  
			}  
		}  
	}  
#endif  

#if 1  
	sleep(2);  

	seg_len = xres/6;  
	for (seg_num = 0; seg_num < 6; seg_num++) {    
		if (seg_num == 5)   
			tmp_seg_len = xres - seg_len*5;  
		else   
			tmp_seg_len = seg_len;  

		for (x = 0; x < tmp_seg_len; x++) {  
			for (y = 0; y < yres; y++) {  
				location = y*xres*4 + (seg_num*seg_len + x)*4;  

				switch (seg_num) {  
					case 0:  
						r = 0xff;  
						g = (0xff/seg_len)*x;  
						b = 0;  
						break;  
					case 1:  
						r = (0xff/seg_len)*(seg_len-x);  
						g = 0xff;  
						b = 0;  
						break;  
					case 2:  
						r = 0;  
						g = 0xff;  
						b = (0xff/seg_len)*x;   
						break;  
					case 3:  
						r = 0;  
						g = (0xff/seg_len)*(seg_len-x);  
						b = 0xff;  
						break;  
					case 4:  
						r =  (0xff/seg_len)*x;    
						g = 0;  
						b = 0xff;  
						break;  
					case 5:  
						r = 0xff;  
						g = 0;  
						b = (0xff/seg_len)*(seg_len-x);   
						break;  
					default:  
						printf("%s--%d:unknown seg_num %d\n", __FILE__, __LINE__, seg_num);  
						break;  
				}  

				r = (r*0x1f)/0xff;  
				g = (g*0x3f)/0xff;  
				b = (b*0x1f)/0xff;  
				rgb = (r << 11) | (g << 5) | b;  
				*((unsigned int*)(fbp + location)) = rgb;  
			}  
		}  
	}  

	sleep(2);  

	seg_len = xres/6;  
	/* white black gray red green blue */  
	for (seg_num = 0; seg_num < 6; seg_num++) {  
		if (seg_num == 5)   
			tmp_seg_len = xres - seg_len*5;  
		else   
			tmp_seg_len = seg_len;  

		for (x = 0; x < tmp_seg_len; x++) {  
			for (y = 0; y < yres; y++) {  
				location = y*xres*4 + (seg_num*seg_len + x)*4;  

				switch (seg_num) {  
					case 0://grey  
						r = 100;  
						g = 100;  
						b = 100;   
						break;  
					case 1://black  
						r = 0;  
						g = 0;  
						b = 0;  
						break;  
					case 2: //white  
						r = 0xff;  
						g = 0xff;  
						b = 0xff;  
						break;  
					case 3://red  
						r = 0xff;  
						g = 0;  
						b = 0;  
						break;  
					case 4: //green  
						r =  0;       
						g = 0xff;  
						b = 0;  
						break;  
					case 5: //blue  
						r = 0;  
						g = 0;  
						b = 0xff;   
						break;  
					default:  
						printf("%s--%d:unknown seg_num %d\n", __FILE__, __LINE__, seg_num);  
						break;  
				}  

				r = (r*0x1f)/0xff;  
				g = (g*0x3f)/0xff;  
				b = (b*0x1f)/0xff;  
				rgb = (r << 11) | (g << 5) | b;  
				*((unsigned int*)(fbp + location)) = rgb;  
			}  
		}  
	}  
#endif  

	return 0;
}

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

int full_color_test(char *fbp, int size, int fbfd, int xres, int yres)
{
	printf("size = %d\n", size);
	memset(fbp, 0xff, size);
	sleep(1);
	full_color(fbp, fbfd, xres, yres, 0x000000ff);
	sleep(1);
	full_color(fbp, fbfd, xres, yres, 0x0000ff00);
	sleep(1);
	full_color(fbp, fbfd, xres, yres, 0x00ff0000);
	sleep(1);

	return 0;
}

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

static void draw_line(char *fbp, int x1, int y1, int x2, int y2)
{
	x2--; x1--; y1--; y2--;
	if (x1 == x2)
		draw_line_v(fbp, x1, y1, x2, y2);
	else if (y1 == y2)
		draw_line_h(fbp, x1, y1, x2, y2);
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


	// test 1
	full_color_test(fbp, screensize, fbfd, vinfo.xres, vinfo.yres);

	// test 2
	rect_test(fbp, screensize, fbfd, vinfo.xres, vinfo.yres);

	sleep(50);

	munmap(fbp, screensize);  
	close(fbfd);  
	return 0;  
}  

