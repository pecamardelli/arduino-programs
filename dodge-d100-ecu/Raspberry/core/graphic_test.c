#include "../ecu.h"

void graphic_test(){
	draw_image(left_arrow);
	usleep(500000);
	
	draw_image(brakes);
	usleep(500000);
	
	draw_image(fog_lights);
	usleep(500000);
	
	draw_image(low_beams);
	usleep(500000);
	
	draw_image(high_beams);
	usleep(500000);
	
	draw_image(emergency_lights);
	usleep(500000);
	
	draw_image(right_arrow);
	usleep(500000);
	
	for(int i=1;i<=36;i++){
		sprintf(big_gauge.filename, "%s%s%sg%02d_big.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir,i);
		draw_image(big_gauge);
		
		sprintf(med_gauge_01.filename, "%s%s%sg%02d_medium.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir,i);
		draw_image(med_gauge_01);
		
		sprintf(med_gauge_02.filename, "%s%s%sg%02d_medium.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir,i);
		draw_image(med_gauge_02);
		
		draw_image(km_l_label);		
		draw_image(volts_label);		
		draw_image(temp_label);
		
		usleep(100000);
	}

	
	draw_image(heater);
	usleep(500000);
	
	draw_image(fan);
	usleep(500000);
	
	
	draw_image(seat_belts);
	usleep(500000);
	
	draw_image(arduino);
	usleep(500000);
	
	
	draw_image(wrench);
	usleep(500000);
	
	draw_image(fuel);
	usleep(500000);
	
	draw_image(choke);
	usleep(500000);
	
	
	draw_image(battery);
	usleep(500000);
	
	draw_image(oil);
	usleep(500000);	
	
	draw_image(engine);
	usleep(500000);
	
	draw_image(temp);
	usleep(500000);
	
	draw_image(alternator);
	usleep(500000);
}