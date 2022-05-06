#include "ecu.h"
/*
#include "libbmp/libbmp.c"
#include "core/reset_screen.c"
#include "core/set_pins.c"
#include "core/set_images.c"
#include "core/flowmeter.c"
#include "core/graphic_test.c"
#include "core/utilities.c"
*/

int main() {
	// Setting system vars //
	strcpy(sys.log_file, "/var/log/ecu.log");
	strcpy(sys.main_dir, "/var/ecu/");
	strcpy(sys.image_dir, "images/");
	strcpy(sys.bg_dir, "backgrounds/");
	strcpy(sys.symbol_dir, "symbols_50x50/");
	strcpy(sys.gauge_dir, "gauges/");
	strcpy(sys.label_dir, "labels/");
	strcpy(sys.number_dir, "number/");
	strcpy(sys.fb_device, "/dev/fb0");

	// Setting flow meter pulses per liter.
	sys.pulses_per_liter = 287;

	logger("Christine ECU initializing.", LOG_SYSTEM);


	screen_ok = screen_setup();
	printf("xres: %d - yres: %d\n", vinfo.xres, vinfo.yres);

	DEVICE *new = create_device(DEV_FLOWMETER, 0xFF, 5, "This is the flowmeter");

	if (new) add_device(new);

	//set_pins();
	
	set_images();
	
	reset_screen(0,0,0);
	
	draw_image(background);
	sleep(1);
	
	reset_screen(0,0,0);
	
	draw_image(christine);
	sleep(1);
	
	reset_screen(0,0,0);
	
	//graphic_test();
	/*
	// -------------------------------------- //
	// ------------ SPEEDOMETER ------------- //
	// -------------------------------------- //
	
	int shmid = shmget(1000,sizeof(int),0666|IPC_CREAT);
    int *speed = shmat(shmid,NULL,0);
	
    if(fork() != 0){
		speedometer();
	}
	
	// -------------------------------------- //
	// ---------- FUEL CONSUMPTION ---------- //
	// -------------------------------------- //
	
    int shmid = shmget(1984,sizeof(float),0666|IPC_CREAT);
    float *total_fuel = shmat(shmid,NULL,0);
	
    if(fork() != 0){
		flowmeter();
	}
	
	// -------------------------------------- //
	*/
	//wait(NULL);

	while (1) {
		draw();
		sleep(2);
	}
		
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
