#include "mbed.h"
#include "rtos.h"
#include "SerialGPS.h"
Thread threadGPS(osPriorityNormal, 512); // 1K stack 

 
SerialGPS gps(PA_9,PA_10,9600);


float longitudeGPS, latitudeGPS, timeGPS, hdopGPS, altGPS, geoidGPS;
int satsGPS;

int working;

extern Serial pc;

void GPS_thread() {

	
	
    while(true) {        

			pc.printf("asdfasdfasdf");
			gps.sample();
			pc.printf("inside");
			
		/*	if (working) {
				pc.printf("GPS message found");
			} else {
				pc.printf("GPS message Not found");
			}*/
			
			longitudeGPS = gps.longitude;
			latitudeGPS = gps.latitude;
			timeGPS = gps.time;
			hdopGPS = gps.hdop;
			altGPS = gps.alt;
			geoidGPS = gps.geoid;
			satsGPS = gps.sats;
			
			
			wait (2);
			
		}
}
