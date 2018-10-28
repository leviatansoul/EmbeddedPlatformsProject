#include "mbed.h"
#include "rtos.h"


AnalogIn soilSensor(PA_0); 

float valueSS=0.0;

Thread threadANALOG(osPriorityNormal, 512); // 1K stack size

void ANALOG_thread(); 

void ANALOG_thread() {
		while (true) {

				Thread::wait(200);
				valueSS=soilSensor*100;
				//printf("%f", valueSS);
				  
    }
}