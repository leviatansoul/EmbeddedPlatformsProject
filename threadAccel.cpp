#include "mbed.h"
#include "rtos.h"
#include "MMA8451Q.h"

#define MMA8451Q_I2C_ADDRESS (0x1d<<1)

Thread threadAccel(osPriorityNormal, 1024); // 1K stack 

MMA8451Q *acc = 0;

extern Serial pc;

//Function
void accRead();

// Declare timer interrupt
Ticker timerAcc;

// Declare pointer variables
float xAcc;
float yAcc;
float zAcc;

// Sampling rates
float accRate = 0.1;

void Accel_thread() {
	acc = new MMA8451Q mma(PB_9,PB_8, MMA8451Q_I2C_ADDRESS);
	
	pc.printf("Starting...");
	
	threadAccel.start(accRead);
	pc.printf("Accelerometer Values:");
	pc.printf("\nX:%f Y:%f  Z:%f", xAcc, yAcc, zAcc);
	
}


void accRead() 
{
    xAcc = acc->getAccX();
    yAcc = acc->getAccY();
    zAcc = acc->getAccZ();
}