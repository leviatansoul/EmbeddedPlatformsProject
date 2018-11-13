#include "mbed.h"
#include "rtos.h"
#include "MMA8451Q.h"

#define MMA8451Q_I2C_ADDRESS (0x1d<<1)

Thread threadAccel(osPriorityNormal, 1024); // 1K stack 

MMA8451Q *acc = 0;

extern Serial pc;
extern int timeToWait;
bool initAccel = true;

extern float getMax(float , float, bool );
extern float getMin(float , float, bool );

//Function

// Declare timer interrupt
Ticker timerAcc;

// Declare pointer variables
float xAcc;
float yAcc;
float zAcc;

float xAccMAX;
float yAccMAX;
float zAccMAX;
float xAccMIN;
float yAccMIN;
float zAccMIN;



// Sampling rates
float accRate = 0.1;

void Accel_thread() {
		acc = new MMA8451Q(PB_9,PB_8, MMA8451Q_I2C_ADDRESS);
	
	//pc.printf("Starting...");
	
	while(true){
		
		
		
		  xAcc = acc->getAccX();
    yAcc = acc->getAccY();
    zAcc = acc->getAccZ();
	//	pc.printf("Accelerometer Values:");
	//	pc.printf("\nX:%f Y:%f  Z:%f", xAcc, yAcc, zAcc);
		
		xAccMAX = getMax( xAccMAX, xAcc, initAccel);
		yAccMAX = getMax( yAccMAX, yAcc, initAccel);
		zAccMAX = getMax( zAccMAX, zAcc, initAccel);
		
		xAccMIN = getMin( xAccMIN, xAcc, initAccel);
		yAccMIN = getMin( yAccMIN, yAcc, initAccel);
		zAccMIN = getMin( zAccMIN, zAcc, initAccel);
		
		initAccel = false;
		
		wait(1);
	}
	
	
}


	
	
	
