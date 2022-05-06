#include "../ecu.h"

void reset_screen(unsigned char red, unsigned char green, unsigned char blue){
	
	unsigned short		bytes_per_pixel	= vinfo.bits_per_pixel / 8;	
	int					line_length		= vinfo.xres * bytes_per_pixel;
	
	if(red < 0 || green < 0 || blue < 0 || red > 255 || green > 255 || blue > 255){
		red		= 0;
		green	= 0;
		blue	= 0;
	}
	
	for (size_t i = 0; i < vinfo.yres; i++)
	{		
		for(size_t k = 0; k < vinfo.xres; k++){
			*(fbp + k*bytes_per_pixel + i*line_length)		= blue;
			*(fbp + k*bytes_per_pixel + i*line_length + 1)	= green;
			*(fbp + k*bytes_per_pixel + i*line_length + 2)	= red;
			*(fbp + k*bytes_per_pixel + i*line_length + 3)	= 255;
		}
	}
}
