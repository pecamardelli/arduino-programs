#include "../ecu.h"

void flowmeter(){
	int 			state 			= HIGH;
	int 			laststate		= HIGH;
	int				x				= 0;
	//int				y				= 0;
	unsigned char	color			= GREEN;
	int				pulses			= 0;
	int				total_pulses	= 0;
	double			lasttstamp 		= microsec();
	double			pulses_per_sec	= 0;
	double			drawn_number	= -1;
	int				refresh			= 1000000;
	//float var;
	
	draw_image(big_gauge);
	draw_image(km_l_label);
	/*
	// ---- SPEED ---- //	
	int shmid = shmget(1000,sizeof(int),0666|IPC_CREAT);
    int *speed = shmat(shmid,NULL,0);
	// --------------- //
	
    int shmid = shmget(1984,sizeof(float),0666|IPC_CREAT);
    float *total_fuel = shmat(shmid,NULL,0);
	*/

	float total_fuel;
	total_fuel = 0;

	while(1){
		state	= digitalRead(FLOWMETER);
		
		if(state != laststate){
			laststate 		= state;
			pulses++;
			total_pulses++;
		}
		
		if(microsec() - lasttstamp >= refresh){
			pulses_per_sec	= 1000000 * pulses / (microsec() - lasttstamp);
	
			if(pulses_per_sec <= 30){
				color	= GREEN;				
			}
			else if (pulses_per_sec <= 36){
				color	= RED;
			}
			
			if(pulses_per_sec < 0){
				sprintf(big_gauge.filename, "%s%s%sg00_big.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir);
			}
			else if(pulses_per_sec > 36){
				sprintf(big_gauge.filename, "%s%s%sg36_big.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir);
			}
			else{
				sprintf(big_gauge.filename, "%s%s%sg%02d_big.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir,(int)pulses_per_sec);
			}
			
			big_gauge.color	= color;
			
			if(pulses_per_sec != drawn_number){				
				if(pulses_per_sec < 10){
					x	= vinfo.xres / 2 - (BIG_NUMBER_WIDTH * 2 + BIG_DOT_WIDTH) / 2 - BIG_NUMBER_WIDTH * 3;
				}
				else{
					x	= vinfo.xres / 2 - (BIG_NUMBER_WIDTH * 3 + BIG_DOT_WIDTH) / 2 - BIG_NUMBER_WIDTH * 2;
				}
				
				draw_number(pulses_per_sec, x, big_gauge.y + BIG_GAUGE_HEIGHT * 0.45, color, OPAQUE, BIG);
				drawn_number	= pulses_per_sec;
				draw_image(big_gauge);
			}
			
			lasttstamp	= microsec();
			total_fuel	= total_pulses /sys.pulses_per_liter;
			pulses		= 0;
		}
		
		usleep(3000);
	}
	exit(0);
}