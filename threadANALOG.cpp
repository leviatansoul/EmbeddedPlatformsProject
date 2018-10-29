#include "mbed.h"
#include "rtos.h"


AnalogIn soilSensor(PA_0); 
AnalogIn lightSensor(PA_4); 

float soilData=0.0;
float lightData=0.0;
extern int timeToWait;
extern Serial pc;
extern int mode;

Thread threadANALOG(osPriorityNormal, 512); // 1K stack size

void ANALOG_thread(); 

void ANALOG_thread() {
		while (true) {

				soilData=soilSensor*100;
				lightData = lightSensor*100;
				wait(timeToWait);
				  
    }
}