#include "SerialGPS.h"

//Inicia GPS
SerialGPS GPS (PA_10, PA_9, 9600);

//Thread
Thread threadGPS(osPriorityNormal, 512); // 1K stack size


//Data Fields

float longitude, latitude, gpsTime, alt, geoid;
int sats;

bool noanswer = false;

extern Serial pc;


void GPS_thread(); 

void GPS_thread() { 
	
	gpsTime = 0.0;
  longitude = 0.0;
  latitude = 0.0;
  sats = 0;
  alt = 0.0;
	geoid = 0.0;   
	
		while (true) {
			
			wait(2);
			
			if (GPS.sample()) {
				gpsTime = GPS.time; 
				longitude = GPS.longitude;
				latitude = GPS.latitude;
				sats = GPS.sats;
				alt = GPS.alt;
				geoid = GPS.geoid;
				
				
				
			}	else {
				noanswer = true;
			}				
    }
}