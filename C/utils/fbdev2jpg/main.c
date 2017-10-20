#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <jpeglib.h>

static void savetojpeg(uint8_t *img, int width, int height)
{
   unsigned char *buffer, *src, *dest;
   unsigned char *line;
   int length;
   int i, line_length;
   FILE *fp;
   struct jpeg_compress_struct jpeg;
   struct jpeg_error_mgr jerr;

   /* setting error output */
   jpeg.err = jpeg_std_error(&jerr);

   /* create jpeg */
   jpeg_create_compress(&jpeg);

   /* setting image */
   jpeg.image_width = width;
   jpeg.image_height = height;
   jpeg.input_components = 3;
   jpeg.in_color_space = JCS_RGB;

   /* other settings by default */
   jpeg_set_defaults(&jpeg);

   /* output */
   fp = fopen("screenshot.jpg","w");
   jpeg_stdio_dest(&jpeg, fp);

   /* processing */
   jpeg_start_compress(&jpeg, TRUE);

   /* transform 4-byte(framebuffer raw data) to 3-byte */
   length = width * height;
   buffer = (unsigned char *)malloc(sizeof(unsigned char)*length*3);
   for (i=0, src=img, dest=buffer; i<length; i+=3,src+=4,dest+=3) {
       *dest = *src;
       *(dest+1) = *(src+1);
       *(dest+2) = *(src+2);
   }

   line_length = width * 3;
   /* write image */
   for (i=0,line=buffer;i<height;i++,line+=line_length)
       jpeg_write_scanlines(&jpeg, &line, 1);

   jpeg_finish_compress(&jpeg);
   jpeg_destroy_compress(&jpeg);

   /* close file */
   fclose(fp);

   /* release */
   free(buffer);
}

int main(int argc, char* argv[])
{
   int fb;
   caddr_t mem_map;
   struct fb_var_screeninfo vinfo;
   struct fb_fix_screeninfo finfo;

   /* open framebuffer device */
   if ((fb = open("/dev/fb0", O_RDWR))==-1) {
       printf("Cannot open framebuffer device.\n");
       exit(1);
   }

   /* get screen information */
   if (ioctl(fb, FBIOGET_VSCREENINFO,&vinfo)==-1) {
       printf("VSCREENINFO Error.\n");
       exit(1);
   }

   /* get memory information of framebuffer */
   if (ioctl(fb, FBIOGET_FSCREENINFO,&finfo)==-1) {
       printf("FSCREENINFO Error.\n");
       exit(1);
   }

   /* map physics address to virtual address */
   mem_map = (char *)mmap(NULL, vinfo.yres*finfo.line_length,
                                              PROT_READ, MAP_SHARED, fb, 0);
   if (mem_map==MAP_FAILED) {
       printf("mmap() Error. %s\n", strerror(errno));
       exit(1);
   }

   savetojpeg((uint8_t *)mem_map, vinfo.xres, vinfo.yres);

   close(fb);

   return 0;
}
