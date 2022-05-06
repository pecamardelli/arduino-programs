#ifndef _HEADER_H_
#define _HEADER_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <errno.h>

#define BMP_MAGIC 19778

#define BMP_GET_PADDING(a) ((a) % 4)

#define		MAX_DIR_LEN		128
#define		MAX_SUBDIR_LEN	64
#define		MAX_NAME_LEN	64
#define		MAX_PATH_LEN	320

#define		MAX_DESC_LEN	64
#define		MAX_LINE_LEN	255

#define LOG_INFO        "INFO"
#define LOG_WARNING     "WARN"
#define LOG_ERROR       "ERROR"
#define LOG_SYSTEM      "SYSTEM"

#define	RED		0
#define	GREEN	1
#define YELLOW	2
#define BLUE	3
#define	DEFAULT	9

#define	TRANSPARENT	0
#define	OPAQUE		1
#define	NODRAW		2

#define SMALL		0
#define	MEDIUM		1
#define BIG			2

// ------------------------- //
// ----- SYMBOL LAYOUT ----- //
// ------------------------- //

#define SYMBOL_UPPER_NUM	7
#define	SYMBOL_WIDTH		100
#define	SYMBOL_HEIGHT		70

#define	BIG_GAUGE_WIDTH		200
#define BIG_GAUGE_HEIGHT	173
#define	BIG_LABEL_WIDTH		80
#define	BIG_LABEL_HEIGHT	40

#define MED_GAUGE_WIDTH		183
#define MED_GAUGE_HEIGHT	121
#define	MED_LABEL_WIDTH		80
#define	MED_LABEL_HEIGHT	20

#define BIG_NUMBER_WIDTH	30
#define	BIG_NUMBER_HEIGHT	42
#define	BIG_DOT_WIDTH		10
#define BIG_DOT_HEIGHT		42

// ------------------------- //
// ------ SCREEN VARS ------ //
// ------------------------- //

#define CENTER_SCREEN	9999
#define	MIDDLE_SCREEN	9998

// -------------------------- //
// ---- SENSOR VARIABLES ---- //
// -------------------------- //

#define FLOWMETER			15

#define DEV_FLOWMETER		0xA
#define DEV_RELAY			0xB

#define RELAY1				0
#define RELAY2				2
#define RELAY3				3
#define RELAY4				12

typedef struct _bmp_header {
	//unsigned short	bfType;
	unsigned int   bfSize;
	unsigned int   bfReserved;
	unsigned int   bfOffBits;
	
	unsigned int   biSize;
	int            biWidth;
	int            biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	int            biXPelsPerMeter;
	int            biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} bmp_header;

typedef struct _bmp_pixel {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} bmp_pixel;

typedef struct _image {
	char			filename[MAX_PATH_LEN];
	int 			x;
	int 			y;
	unsigned char	color;
	unsigned char	alpha;
	unsigned char	last_state;
}image;

// This is faster than a function call
#define BMP_PIXEL(r,g,b) ((bmp_pixel){(b),(g),(r)})

typedef struct _bmp_img {
	bmp_header   *img_header;
	bmp_pixel  **img_pixels;
} bmp_img;

// BMP_HEADER

bmp_header		*bmp_header_read	(FILE*);
void			show_header			(bmp_header *);

// BMP_IMG
void            bmp_img_alloc		(bmp_img*);
void            bmp_img_free		(bmp_img*);
bmp_img			*bmp_img_read		(const char*);
void 			draw_image			(image);
void			graphic_test		();

struct 			fb_var_screeninfo 	vinfo;
struct 			fb_fix_screeninfo 	finfo;
int 			fbfd;
long int 		screensize;
char 			*fbp;
unsigned char   screen_ok;

typedef struct _DEVICE {
	unsigned char	type;
	unsigned char	enabled;
	u_int8_t		pin;
	char			desc[MAX_LINE_LEN];
	struct _DEVICE	*next;
}DEVICE;

typedef struct {
	// System
	char log_file		[MAX_NAME_LEN];
	// Assets path and names
	char main_dir		[MAX_DIR_LEN];
	char image_dir		[MAX_SUBDIR_LEN];
	char bg_dir			[MAX_SUBDIR_LEN];
	char symbol_dir		[MAX_SUBDIR_LEN];
	char gauge_dir		[MAX_SUBDIR_LEN];
	char label_dir		[MAX_SUBDIR_LEN];
	char number_dir		[MAX_SUBDIR_LEN];

	char fb_device		[MAX_NAME_LEN];
	char bg_file		[MAX_NAME_LEN];
	double				pulses_per_liter;

	DEVICE 				*first_device;
	DEVICE 				*last_device;
	//const double	pulses_per_liter	= 287;
}SYS_CONFIG;

SYS_CONFIG sys;

typedef struct {
	float				volts;
	float				temp;
	float				oil;
	float				amps;
}READINGS;

READINGS data;

// --------------------------------------------- //
// ------------ FUNCTION DEFINITION ------------ //
// --------------------------------------------- //

// MISC
void 			logger				(char *str, char *type);
unsigned char 	screen_setup		();
void 			reset_screen		(unsigned char, unsigned char, unsigned char);
void			set_pins			();
void			set_images			();
void			flowmeter			();
void 			draw_number			(float, int, int, unsigned char, unsigned char, unsigned char);
double			microsec			();
int 			device_count		();
void			add_device			(DEVICE*);
unsigned char 	valid_device_type	(unsigned char dev_type);
DEVICE * 		create_device		(unsigned char type, unsigned char enabled, u_int8_t pin, char *desc);
unsigned char	pin_available		(u_int8_t pin);
char 			*print_type			(unsigned char type);
void			draw				();

// utils
void* 			create_shmem		(size_t);

// ------------------------------------- //
// ------------ BACKGROUNDS ------------ //
// ------------------------------------- //

image 	background;
image	christine;

// ------------------------------------- //
// -------------- SYMBOLS -------------- //
// ------------------------------------- //

image	left_arrow;
image	right_arrow;
image	low_beams;
image	high_beams;
image	fog_lights;
image	emergency_lights;
image	heater;
image	fan;
image	seat_belts;
image	arduino;
image	wrench;
image	fuel;
image	choke;
image	battery;
image	oil;
image	brakes;
image	engine;
image	temp;
image	alternator;
image	choke;

image	big_gauge;
image	km_l_label;
image	volts_label;
image	temp_label;
image	amp_label;
image	med_gauge_01;
image	med_gauge_02;

image	digit;

image	*all_images[32];

// ------------------------------------- //
// ------------ GLOBAL VARS ------------ //
// ------------------------------------- //

//void *total_fuel;

#endif /* _HEADER_H_ */
