#include "../ecu.h"

void draw() {
    int index = 0;

    while (all_images[index]) {
        if (all_images[index]->last_state != all_images[index]->alpha) {
            draw_image(*all_images[index]);
            all_images[index]->last_state = all_images[index]->alpha;
        }
        else {
            if (all_images[index]->alpha == TRANSPARENT) all_images[index]->alpha = NODRAW;
            else all_images[index]->alpha = TRANSPARENT;
        }
        usleep(100000);
        index++;
    }


    /*
	draw_image(brakes);
	draw_image(fog_lights);
	draw_image(low_beams);
	draw_image(high_beams);
	draw_image(emergency_lights);
	draw_image(right_arrow);

	draw_image(heater);
	draw_image(fan);
	draw_image(seat_belts);
	draw_image(arduino);

	draw_image(wrench);
	draw_image(fuel);
	draw_image(choke);

	draw_image(battery);
	draw_image(oil);
	draw_image(engine);
	draw_image(temp);
	draw_image(alternator);
    */
}