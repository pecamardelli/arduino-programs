#include "../ecu.h"

void set_images(){
	const int xres	= vinfo.xres;
	const int yres	= vinfo.yres;
	int index		= 0;
	//int	dist	= (xres - SYMBOL_WIDTH * SYMBOL_UPPER_NUM) / (SYMBOL_UPPER_NUM + 1);
	//int upper_y	= 20;
	
	// ----------------------------- //
	// -------- BACKGROUNDS -------- //
	// ----------------------------- //
	
	sprintf(background.filename, "%s%s%sbg04.bmp",sys.main_dir,sys.image_dir,sys.bg_dir);
	background.x			= CENTER_SCREEN;
	background.y			= MIDDLE_SCREEN;
	background.color		= RED;
	background.alpha		= TRANSPARENT;
	background.last_state	= NODRAW;
	
	sprintf(christine.filename, "%s%s%schristine.bmp",sys.main_dir,sys.image_dir,sys.bg_dir);
	christine.x		= CENTER_SCREEN;
	christine.y		= MIDDLE_SCREEN;
	christine.color	= RED;
	christine.alpha	= TRANSPARENT;
	
	// ----------------------------- //
	// ------- UPPER SYMBOLS ------- //
	// ----------------------------- //
	
	// --------- FIRST BLOCK --------- //

	sprintf(left_arrow.filename, "%s%s%slarrow.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//left_arrow.x		= xres / 2 - SYMBOL_WIDTH * 3.5 - dist * 3;
	//left_arrow.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	left_arrow.x			= xres / 5 - SYMBOL_WIDTH / 2;
	left_arrow.y			= 0;
	left_arrow.color		= GREEN;
	left_arrow.alpha		= NODRAW;
	left_arrow.last_state	= NODRAW;
	all_images[index] = &left_arrow;
	index++;
	
	sprintf(brakes.filename, "%s%s%sbrakes.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//brakes.x		= xres / 2 - SYMBOL_WIDTH * 2.5 - dist * 2;
	//brakes.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	brakes.x			= (xres  - SYMBOL_WIDTH) / 2;
	brakes.y			= 0;
	brakes.color		= RED;
	brakes.alpha		= NODRAW;
	brakes.last_state	= NODRAW;
	all_images[index] = &brakes;
	index++;
	
	sprintf(right_arrow.filename, "%s%s%srarrow.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//right_arrow.x		= xres / 2 + SYMBOL_WIDTH * 2.5 + dist * 3;
	//right_arrow.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	right_arrow.x			= xres * 4 / 5 - SYMBOL_WIDTH / 2;
	right_arrow.y			= 0;
	right_arrow.color		= GREEN;
	right_arrow.alpha		= NODRAW;
	right_arrow.last_state	= NODRAW;
	all_images[index] = &right_arrow;
	index++;

	// --------- SECOND BLOCK --------- //
	
	sprintf(fog_lights.filename, "%s%s%sfaux.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//fog_lights.x		= xres / 2 - SYMBOL_WIDTH * 1.5 - dist;
	//fog_lights.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	fog_lights.x			= xres / 5 - SYMBOL_WIDTH / 2 - 30;
	fog_lights.y			= SYMBOL_HEIGHT;
	fog_lights.color		= YELLOW;
	fog_lights.alpha		= NODRAW;
	fog_lights.last_state	= NODRAW;
	all_images[index] = &fog_lights;
	index++;
	
	sprintf(low_beams.filename, "%s%s%slbeams.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//low_beams.x		= xres / 2 - SYMBOL_WIDTH * 0.5;
	//low_beams.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	low_beams.x				= xres * 2 / 5 - SYMBOL_WIDTH / 2 - 10;
	low_beams.y				= SYMBOL_HEIGHT;
	low_beams.color			= GREEN;
	low_beams.alpha			= NODRAW;
	low_beams.last_state	= NODRAW;
	all_images[index] = &low_beams;
	index++;
	
	sprintf(high_beams.filename, "%s%s%shbeams.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//high_beams.x		= xres / 2 + SYMBOL_WIDTH * 0.5 + dist;
	//high_beams.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	high_beams.x			= xres * 3 / 5 - SYMBOL_WIDTH / 2 + 10;
	high_beams.y			= SYMBOL_HEIGHT;
	high_beams.color		= BLUE;
	high_beams.alpha		= NODRAW;
	high_beams.last_state	= NODRAW;
	all_images[index] = &high_beams;
	index++;
	
	sprintf(emergency_lights.filename, "%s%s%selights.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	//emergency_lights.x		= xres / 2 + SYMBOL_WIDTH * 1.5 + dist * 2;
	//emergency_lights.y		= yres * 0.15 - SYMBOL_HEIGHT / 2;
	emergency_lights.x			= xres * 4 / 5 - SYMBOL_WIDTH / 2 + 30;
	emergency_lights.y			= SYMBOL_HEIGHT;
	emergency_lights.color		= RED;
	emergency_lights.alpha		= NODRAW;
	emergency_lights.last_state	= NODRAW;
	all_images[index] = &emergency_lights;
	index++;
	
	// ---------- MIDDLE-UP SYMBOLS ---------- //
	
	sprintf(heater.filename, "%s%s%sheater.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	heater.x			= xres * 0.2 - SYMBOL_WIDTH / 2;
	heater.y			= yres * 0.4 - SYMBOL_HEIGHT / 2;
	heater.color		= YELLOW;
	heater.alpha		= NODRAW;
	heater.last_state	= NODRAW;
	all_images[index] = &heater;
	index++;
	
	sprintf(fan.filename, "%s%s%sfan.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	fan.x			= xres * 0.3 - SYMBOL_WIDTH / 2;
	fan.y			= yres * 0.4 - SYMBOL_HEIGHT / 2;
	fan.color		= GREEN;
	fan.alpha		= NODRAW;
	fan.last_state	= NODRAW;
	all_images[index] = &fan;
	index++;
	
	sprintf(seat_belts.filename, "%s%s%sbelts.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	seat_belts.x			= xres * 0.7 - SYMBOL_WIDTH / 2;
	seat_belts.y			= yres * 0.4 - SYMBOL_HEIGHT / 2;
	seat_belts.color		= YELLOW;
	seat_belts.alpha		= NODRAW;
	seat_belts.last_state	= NODRAW;
	all_images[index] = &seat_belts;
	index++;
	
	// --------- MIDDLE-DOWN SYMBOLS --------- //
	
	sprintf(engine.filename, "%s%s%sengine.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	engine.x			= (xres - SYMBOL_WIDTH) / 2;
	engine.y			= yres - SYMBOL_HEIGHT * 3;
	engine.color		= YELLOW;
	engine.alpha		= NODRAW;
	engine.last_state	= NODRAW;
	all_images[index] = &engine;
	index++;
	
	// ----------------------------- //
	// ------- LOWER SYMBOLS ------- //
	// ----------------------------- //
	
	// ---------- UPPER BLOCK ---------- //

	sprintf(battery.filename, "%s%s%sbattery.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	battery.x			= xres / 5 - SYMBOL_WIDTH / 2 - 30;
	battery.y			= yres - SYMBOL_HEIGHT * 2;
	battery.color		= RED;
	battery.alpha		= NODRAW;
	battery.last_state	= NODRAW;
	all_images[index] = &battery;
	index++;
	
	sprintf(oil.filename, "%s%s%soil.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	oil.x			= xres * 2 / 5 - SYMBOL_WIDTH / 2 - 10;
	oil.y			= yres - SYMBOL_HEIGHT * 2;
	oil.color		= RED;
	oil.alpha		= NODRAW;
	oil.last_state	= NODRAW;
	all_images[index] = &oil;
	index++;
	
	sprintf(temp.filename, "%s%s%stemp.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	temp.x			= xres * 3 / 5 - SYMBOL_WIDTH / 2 + 10;
	temp.y			= yres - SYMBOL_HEIGHT * 2;
	temp.color		= RED;
	temp.alpha		= NODRAW;
	temp.last_state	= NODRAW;
	all_images[index] = &temp;
	index++;
	
	sprintf(alternator.filename, "%s%s%salt.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	alternator.x			= xres * 4 / 5 - SYMBOL_WIDTH / 2 + 30;
	alternator.y			= yres - SYMBOL_HEIGHT * 2;
	alternator.color		= RED;
	alternator.alpha		= NODRAW;
	alternator.last_state	= NODRAW;
	all_images[index] = &alternator;
	index++;

	// ---------- LOWER BLOCK ---------- //
	
	sprintf(arduino.filename, "%s%s%sarduino.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	arduino.x			= xres / 5 - SYMBOL_WIDTH / 2 - 30;
	arduino.y			= yres - SYMBOL_HEIGHT - 1;
	arduino.color		= RED;
	arduino.alpha		= NODRAW;
	arduino.last_state	= NODRAW;
	all_images[index] = &arduino;
	index++;
	
	sprintf(wrench.filename, "%s%s%swrench.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	wrench.x			= xres * 2 / 5 - SYMBOL_WIDTH / 2 - 10;
	wrench.y			= yres - SYMBOL_HEIGHT - 1;
	wrench.color		= YELLOW;
	wrench.alpha		= NODRAW;
	wrench.last_state	= NODRAW;
	all_images[index] = &wrench;
	index++;
	
	sprintf(fuel.filename, "%s%s%sfuel.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	fuel.x			= xres * 3 / 5 - SYMBOL_WIDTH / 2 + 10;
	fuel.y			= yres - SYMBOL_HEIGHT - 1;
	fuel.color		= YELLOW;
	fuel.alpha		= NODRAW;
	fuel.last_state	= NODRAW;
	all_images[index] = &fuel;
	index++;
	
	sprintf(choke.filename, "%s%s%schoke.bmp",sys.main_dir,sys.image_dir,sys.symbol_dir);
	choke.x				= xres * 4 / 5 - SYMBOL_WIDTH / 2 + 30;
	choke.y				= yres - SYMBOL_HEIGHT - 1;
	choke.color			= YELLOW;
	choke.alpha			= NODRAW;
	choke.last_state	= NODRAW;
	all_images[index] = &choke;
	index++;
	
	// ----------------------------- //
	// ---------- GAUGES ----------- //
	// ----------------------------- //
	
	sprintf(big_gauge.filename, "%s%s%sg00_big.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir);
	big_gauge.x				= CENTER_SCREEN;
	big_gauge.y				= yres * 0.4 - BIG_GAUGE_HEIGHT / 2; 
	big_gauge.color			= GREEN;
	big_gauge.alpha			= NODRAW;
	big_gauge.last_state	= NODRAW;
	all_images[index] = &big_gauge;
	index++;
	
	sprintf(km_l_label.filename, "%s%s%skm_l.bmp",sys.main_dir,sys.image_dir,sys.label_dir);
	km_l_label.x			= CENTER_SCREEN;
	km_l_label.y			= yres * 0.4 + BIG_GAUGE_HEIGHT / 2 - BIG_LABEL_HEIGHT;
	km_l_label.color		= DEFAULT;
	km_l_label.alpha		= NODRAW;
	km_l_label.last_state	= NODRAW;
	all_images[index] = &km_l_label;
	index++;
	
	sprintf(med_gauge_01.filename, "%s%s%sg00_medium.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir);
	med_gauge_01.x			= xres * 0.25 - MED_GAUGE_WIDTH / 2;
	med_gauge_01.y			= yres * 0.6 - MED_GAUGE_HEIGHT / 2;
	med_gauge_01.color		= RED;
	med_gauge_01.alpha		= NODRAW;
	med_gauge_01.last_state	= NODRAW;
	all_images[index] = &med_gauge_01;
	index++;
	
	sprintf(volts_label.filename, "%s%s%svolts.bmp",sys.main_dir,sys.image_dir,sys.label_dir);
	volts_label.x			= xres * 0.25 - MED_LABEL_WIDTH / 2;
	volts_label.y			= yres * 0.6 + MED_GAUGE_HEIGHT / 2 - MED_LABEL_HEIGHT;
	volts_label.color		= DEFAULT;
	volts_label.alpha		= NODRAW;
	volts_label.last_state	= NODRAW;
	all_images[index] = &volts_label;
	index++;
	
	sprintf(med_gauge_02.filename, "%s%s%sg00_medium.bmp",sys.main_dir,sys.image_dir,sys.gauge_dir);
	med_gauge_02.x			= xres * 0.75 - MED_GAUGE_WIDTH / 2;
	med_gauge_02.y			= yres * 0.6 - MED_GAUGE_HEIGHT / 2;
	med_gauge_02.color		= YELLOW;
	med_gauge_02.alpha		= NODRAW;
	med_gauge_02.last_state	= NODRAW;
	all_images[index] = &med_gauge_02;
	index++;
	
	sprintf(temp_label.filename, "%s%s%stemp.bmp",sys.main_dir,sys.image_dir,sys.label_dir);
	temp_label.x			= xres * 0.75 - MED_LABEL_WIDTH / 2;
	temp_label.y			= yres * 0.6 + MED_GAUGE_HEIGHT /2 - MED_LABEL_HEIGHT;
	temp_label.color		= YELLOW;
	temp_label.alpha		= NODRAW;
	temp_label.last_state	= NODRAW;
	all_images[index] = &temp_label;
	index++;
}