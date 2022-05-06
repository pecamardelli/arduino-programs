#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "libbmp.h"
#include "libbmp.c"
//#include "bitmaps.c"

int main()
{
        int fbfd = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        long int screensize = 0;
        char *fbp = 0;
		
		bmp_img *image;

        fbfd = open("/dev/fb0", O_RDWR);
        if (!fbfd) {
                printf("Error: cannot open framebuffer device.\n");
                exit(1);
        }
        printf("The framebuffer device was opened successfully.\n");

        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
                printf("Error reading fixed information.\n");
                exit(2);
        }

        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
                printf("Error reading variable information.\n");
                exit(3);
        }

	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
        //screensize = vinfo.xres * vinfo.yres;
	printf("xres: %d - yres: %d - bpp: %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if ((int)fbp == -1) {
                printf("Error: failed to map framebuffer device to memory.\n");
                exit(4);
        }
        printf("The framebuffer device was mapped to memory successfully. %d\n", (int)fbp);
printf("Reading image...\n");
	if(bmp_img_read(image,"bg01.bmp")){
		printf("sizeof bmp_pixel: %d.\n", sizeof(bmp_pixel));
	}
//	for(int i=0;i < bitmapInfoHeader->biHeight;i++){
//		for(int k = 0;k < bitmapInfoHeader->biWidth;k++){
//			for(int l = 0;l < 4;l++){
//				*(fbp + (bitmapInfoHeader->biWidth*i*4) + k*4 + l) = bitmapData[(bitmapInfoHeader->biWidth*i*4) + k*4 + l];
//			}
//			*(fbp + k*4 + 1) = 255;
//			*(fbp + k*4 + 2) = 255;
//			*(fbp + k*4 + 3) = 255;
//		}
//	}

        printf("Screen blacked out.\n");

        munmap(fbp, screensize);
        close(fbfd);
        return 0;
}
