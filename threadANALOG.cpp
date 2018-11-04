#include "mbed.h"
#include "rtos.h"


AnalogIn soilSensor(PA_0); 
AnalogIn lightSensor(PA_4); 

float soilData=0.0;
float lightData=0.0;
extern int timeToWait;
extern Serial pc;
extern int mode;
bool init = true;


float lightDataAverage;
float getAverage(float, float, bool);
float lightDataMaximum;
float getMax(float , float, bool );
float lightDataMinimum;
float getMin(float , float, bool );



float soilDataAverage;
float soilDataMaximum;
float soilDataMinimum;

Thread threadANALOG(osPriorityNormal, 512); // 1K stack size

void ANALOG_thread(); 

void ANALOG_thread() {
	
	
							
		
		while (true) {

				soilData=soilSensor*100;
				lightData = lightSensor*100;
			
		
				lightDataAverage = getAverage(lightDataAverage, lightData, init);
				lightDataMaximum = getMax(lightDataMaximum, lightData, init);
				lightDataMinimum = getMin(lightDataMinimum, lightData, init);
			
				soilDataAverage = getAverage(soilDataAverage,soilData, init);
				soilDataMaximum = getMax(soilDataMaximum, soilData, init);
				soilDataMinimum = getMin(soilDataMinimum, soilData, init);
			
			
				init = false;
				
				wait(timeToWait);
				  
    }
}

float getMax(float a, float data, bool init){
	if(init){
		return data;
	}
	if(a > data){
		return a;
	}
	return data;
}

float getMin(float a, float data, bool init){
		if(init){
		return data;
	}
	if(a < data){
		return a;
	}
	return data;
}

float getAverage(float a, float data, bool init){
		if(init){
		return data;
	}
	return (a + data)/2;
}
