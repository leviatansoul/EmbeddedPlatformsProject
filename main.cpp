#include "mbed.h"
#include "rtos.h"

//DIEGO GARCIA DIAZ y ANDREAS BECK

DigitalOut myled1(LED1); //test mode
DigitalOut myled2(LED2); //normal mode
DigitalOut myled3(LED3); //advanced mode

InterruptIn btnMode(PB_2);

Serial pc(USBTX,USBRX,19200);

Ticker t;
int timerCounter;
void sendCalculations(void);
void btnMode_handler(void);
bool ticker = false;

//RGB LED
DigitalOut blue(PB_13);
DigitalOut green(PH_1);
DigitalOut red(PH_0);
void maxValueColorTEST( int , int , int ); //Color decision in TEST Mode
void maxValueColorNORMAL( int , int , int ); //Color decision in TEST Mode
void outOfRangesNORMAL(); //Color decision in NORMAL mode based in values out of ranges
enum color {
	RED,
	GREEN,
	BLUE
};
int color;

//Analog stuff
extern Thread threadANALOG;
extern void ANALOG_thread();

extern float soilData; //Soil Measure
extern float soilDataAverage;
extern float soilDataMaximum;
extern float soilDataMinimum;

extern float lightData; //Light Measure
extern float lightDataAverage;
extern float lightDataMaximum;
extern float lightDataMinimum;

//GPS Stuff
extern Thread threadGPS;
extern void GPS_thread();
extern float longitudeGPS, latitudeGPS, timeGPS, altGPS, geoidGPS;
extern float hour, minute, seconds;
extern int satsGPS;


//I2C stuff
extern Thread threadI2C;
extern void I2C_thread();

extern float rhData; //Humidity
extern float rhDataAverage;
extern float rhDataMaximum;
extern float rhDataMinimum;
bool hum_out_of_range = false;

extern float tData; //Temperature
extern float tDataAverage;
extern float tDataMaximum;
extern float tDataMinimum;
bool temp_out_of_range = false;

extern int clear_value, red_value, green_value, blue_value; //Colors
extern int clearAverage, redAverage, greenAverage, blueAverage;
bool checkTemperature(float); //Check if temperature is out of range and print the result
bool checkHumidity(float); //Check if humidity is out of range and print the result

//Accel stuff
extern Thread threadAccel;
extern void Accel_thread();
extern float xAcc;
extern float yAcc;
extern float zAcc;
extern float xAccMAX;
extern float yAccMAX;
extern float zAccMAX;
extern float xAccMIN;
extern float yAccMIN;
extern float zAccMIN;



enum modes 
{   TEST, 
    NORMAL, 
    ADVANCED
};

int mode;
int timeToWait;



// main() runs in its own thread in the OS
int main() {

		myled1 = 1;  
    myled2 = 0;
	  myled3 = 0;
		timeToWait = 10;
		mode = TEST;
	
		red = 0;
		green = 0;
		blue = 0;
	    
		timerCounter = 0;
		t.attach(sendCalculations, 4);
	
		btnMode.mode(PullUp);
		btnMode.fall(btnMode_handler);

    threadANALOG.start(ANALOG_thread);  
		threadI2C.start(I2C_thread);
		threadGPS.start(GPS_thread);
		threadAccel.start(Accel_thread);

   	
    while (true) {		  
								
     
			switch ( mode )  
      {  
         case TEST:
						timeToWait = 2;
						myled1 = 1;
						myled3 = 0;
						myled2 = 0;	

				 
						pc.printf(" \n\r");
						pc.printf(" \n\r");
				 pc.printf(" **********************\n\r");
            pc.printf("        TEST MODE \n\r");				 
				 pc.printf(" **********************\n\r");
						pc.printf(" \n\r");							
				 
						pc.printf("SOIL MOISTURE: %.2f %% \n\r", soilData);
						pc.printf("LIGHT: %.2f %% \n\r", lightData);
						pc.printf("TEMPERATURE: %.2f C \n\r", tData);
						pc.printf("RELATIVE HUMIDITY: %.2f %% \n\r", rhData);
						pc.printf("COLOR SENSOR: Clear (%i), Red (%i), Green (%i), Blue (%i) \n\r", clear_value, red_value, green_value, blue_value);						 
						pc.printf("GPS: ");
								pc.printf(" Longitude: %.2f", longitudeGPS);
								pc.printf(", Latitude: %.2f", latitudeGPS);
								pc.printf(", Time: %.2f", timeGPS);
								pc.printf(", Altitude: %.2f", altGPS);
								pc.printf(", Geo ID: %.2f", geoidGPS);
								pc.printf(", Satelites: %d", satsGPS);
						pc.printf("\n\rTIME: %.2f, %.2f, %.2f", hour, minute, seconds);
						pc.printf("\n\rACCELEROMETER VALUES: X:%.2f Y:%.2f  Z:%.2f \n\r", xAcc, yAcc, zAcc);
				
				 
			 		 maxValueColorTEST( red_value, green_value, blue_value);
				 
				 
						pc.printf(" \n\r");
					 pc.printf("COUNTER: %i  \n\r", timerCounter);
				 
					  wait(timeToWait);
				 
            break;  
				 
         case NORMAL:  
						timeToWait = 10;
						myled1 = 0;
						myled3 = 1;
						myled2 = 0;

						pc.printf(" \n\r");
						pc.printf(" \n\r");

				 pc.printf(" **********************\n\r");				 
						pc.printf("        NORMAL MODE \n\r");				 
				 pc.printf(" **********************\n\r");
						pc.printf(" \n\r");
				 
						pc.printf("SOIL MOISTURE: %.2f %% \n\r", soilData);
						pc.printf("LIGHT: %.2f %% \n\r", lightData);
						
				 if(checkTemperature(tData)){
						 pc.printf("TEMPERATURE: OUT OF RANGE \n\r");
					 }else{
					 pc.printf("TEMPERATURE: %.2f C \n\r", tData);						 
					 }
					 
					 if(checkHumidity(rhData)){
						 pc.printf("RELATIVE HUMIDITY: OUT OF RANGE \n\r");
					 }else{
					 pc.printf("RELATIVE HUMIDITY: %.2f %% \n\r", rhData);						 
					 }
					 
					 pc.printf("COLOR SENSOR: Clear (%i), Red (%i), Green (%i), Blue (%i)", clear_value, red_value, green_value, blue_value);
			 		 outOfRangesNORMAL();
					 
					 pc.printf("\n\rACCELEROMETER VALUES: X:%.2f Y:%.2f  Z:%.2f \n\r", xAcc, yAcc, zAcc);
					 
					 pc.printf("GPS: ");
								pc.printf(" Longitude: %.2f", longitudeGPS);
								pc.printf(", Latitude: %.2f", latitudeGPS);
								pc.printf(", Time: %.2f", timeGPS);
								pc.printf(", Altitude: %.2f", altGPS);
								pc.printf(", Geo ID: %.2f", geoidGPS);
								pc.printf(", Satelites: %d \n\r", satsGPS);
					 pc.printf("TIME: %.2f, %.2f, %.2f \n\r", hour, minute, seconds);
					 
					 if(ticker){
						 pc.printf("HUMIDITY AVERAGE : %.2f %% \n\r", rhDataAverage);
						 pc.printf("TEMPERATURE AVERAGE : %.2f C \n\r", tDataAverage);
						 pc.printf("SOIL AVERAGE : %.2f %% \n\r", soilDataAverage);
						 pc.printf("LIGHT AVERAGE : %.2f %% \n\r", lightDataAverage);
						 pc.printf("ACCELEROMETER MAXIMUM VALUES: X:%.2f Y:%.2f  Z:%.2f \n\r", xAccMAX, yAccMAX, zAccMAX);
						 pc.printf("ACCELEROMETER MINIMUM VALUES: X:%.2f Y:%.2f  Z:%.2f \n\r", xAccMIN, yAccMIN, zAccMIN);
						 
						 maxValueColorNORMAL( red_value, green_value, blue_value);
						 
						 ticker = false;
					 }
					 
					 
				 
				 
					  wait(timeToWait);
				 
           			 
            break;  
				 
				 case ADVANCED:  
					 
						myled1 = 0;
						myled3 = 0;
						myled2 = 1;		
						pc.printf("ADVANCED MODE\n\r");
												
           
            break;  
				 
         default:  
					 mode = TEST;
            
      } 
			
    }
}

void maxValueColorTEST( int redv, int greenv, int bluev){
	int color = 0; //Var that indicates the max color
	
	//We obtain the max value between the colors
	if (redv > greenv){ 
		if(redv > bluev){
			color = RED;
		} else {
			color = BLUE;
		}
	} else {
		if (greenv > bluev){
			color = GREEN;
		} else {
			color = BLUE;
		}
	}
	
	switch (color){
		case RED:
			red = 0;
			green = 1;
			blue = 1;
		pc.printf("DOMINANT COLOR: RED\n\r");
			break;
		case GREEN:
			green = 0;
			red = 1;
			blue = 1;
		pc.printf("DOMINANT COLOR: GREEN\n\r");
			break;
		case BLUE:
			blue = 0;
			red = 1;
			green = 1;
		pc.printf("DOMINANT COLOR: BLUE\n\r");
			break;
		default:
			break;
	}
		
}

void maxValueColorNORMAL( int redv, int greenv, int bluev){
	
	//We obtain the max value between the colors
	if (redv > greenv){ 
		if(redv > bluev){
			pc.printf("DOMINANT COLOR IN THE LAST HOUR: RED\n\r");
		} else {
			pc.printf("DOMINANT COLOR IN THE LAST HOUR: BLUE\n\r");
		}
	} else {
		if (greenv > bluev){
				pc.printf("DOMINANT COLOR IN THE LAST HOUR: GREEN\n\r");
		} else {
			pc.printf("DOMINANT COLOR IN THE LAST HOUR: BLUE\n\r");
		}
	}
	
}

bool checkTemperature(float t){
	if( t > 20 || t < -10){
		temp_out_of_range = true;
		return true;
	}
	temp_out_of_range = false;
	return false;
}

bool checkHumidity(float h){
	if( h > 75 || h < 25){
		hum_out_of_range = true;
		
		return true;
	}
	hum_out_of_range = false;
	return false;
}

void outOfRangesNORMAL(){
	
	if(temp_out_of_range){
			red = 0;
			green = 1;
			blue = 1;
	}else	if(hum_out_of_range){
			red = 1;
			green = 1;
			blue = 0;
	} else {
			red = 1;
			green = 1;
			blue = 1;
	}
	
}

void sendCalculations(void){
	timerCounter = timerCounter + 1;
	if(timerCounter == 2){
		ticker = true;
		timerCounter = 0;
	}
}

void btnMode_handler(void){
	if(mode == TEST){
		mode = NORMAL;
	}else {
		mode = TEST;
	}
	
}