#include "mbed.h"
#include "rtos.h"


AnalogIn soilSensor(PA_0); 
AnalogIn lightSensor(PA_4); 

float soilData=0.0;
float lightData=0.0;

Thread threadANALOG(osPriorityNormal, 512); // 1K stack size

void ANALOG_thread(); 

void ANALOG_thread() {
		while (true) {

				Thread::wait(200);
				soilData=soilSensor*100;
				lightData = lightSensor*100;
				//printf("%f", soilData);
				  
    }
}