#include "../ecu.h"

void set_pins(){
	
	wiringPiSetup();
	
	// ------------------------------- //
	// ----------- RELAYS ------------ //
	// ------------------------------- //
	
	pinMode(RELAY1, OUTPUT);
	pinMode(RELAY2, OUTPUT);
	pinMode(RELAY3, OUTPUT);
	pinMode(RELAY4, OUTPUT);
	
	digitalWrite(RELAY1, HIGH);
	digitalWrite(RELAY2, HIGH);
	digitalWrite(RELAY3, HIGH);
	digitalWrite(RELAY4, HIGH);
	
	// ------------------------------- //	
	// -------- FUEL FLOWMETER ------- //
	// ------------------------------- //
	
	pinMode(FLOWMETER, INPUT);
	
	// ------------------------------- //	
	// --- DHT11 TEMP & HUM SENSOR --- //
	// ------------------------------- //
	
}