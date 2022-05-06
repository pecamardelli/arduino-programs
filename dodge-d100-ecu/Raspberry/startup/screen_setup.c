#include "../ecu.h"

void handle_error() {
	logger(strerror(errno), LOG_ERROR);
	perror(strerror(errno));
}

unsigned char screen_setup() {
    fbfd = open(sys.fb_device, O_RDWR);
	if (!fbfd) {
        handle_error();
        return 0x0;
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        handle_error();
        return 0x0;
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        handle_error();
        return 0x0;
	}

	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	if ((void *)fbp == MAP_FAILED) {
		handle_error();
		return 0x0;
	}

	return 0xFF;
}