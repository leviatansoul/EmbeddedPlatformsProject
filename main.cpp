#include "mbed.h"
#include "rtos.h"

//DIEGO GARCIA DIAZ y ANDREAS BECK

DigitalOut myled1(LED1); //test mode
DigitalOut myled2(LED2); //normal mode
DigitalOut myled3(LED3); //advanced mode

Serial pc(USBTX,USBRX,19200);

//RGB LED
DigitalOut blue(PB_13);
DigitalOut green(PB_14);
DigitalOut red(PB_15);
void maxValueTEST( int , int , int ); //Color decision in TEST Mode
enum color {
	RED,
	GREEN,
	BLUE
};
int color;

//Analog stuff
extern Thread threadANALOG;
extern void ANALOG_thread();
extern float soilData; 
extern float lightData;

//I2C stuff
extern Thread threadI2C;
extern void I2C_thread();
extern float rhData; //Humidity
extern float tData; //Temperature
extern int clear_value, red_value, green_value, blue_value; //Colors



enum modes 
{   TEST, 
    NORMAL, 
    ADVANCED
};

int mode;



// main() runs in its own thread in the OS
int main() {

		myled1 = 1;  
    myled2 = 0;
	  myled3 = 0;
		mode = TEST;
	
		red = 0;
		green = 0;
		blue = 0;
	
    

    threadANALOG.start(ANALOG_thread);  
		threadI2C.start(I2C_thread);
   	
    while (true) {		  
			
					
     
			
			pc.printf("VALOR %.lf ", soilData);
			
			switch ( mode )  
      {  
         case TEST:
					  wait(2);
						myled1 = 1;
						myled3 = 0;
						myled2 = 0;					 
            pc.printf("TEST \n\r");
												
				 
						pc.printf("Soil %f", soilData);
						pc.printf("Lihgt %f", lightData);
						pc.printf("Hum %f", rhData);
						pc.printf("Temp %f", tData);
				 		pc.printf("Clear (%i), Red (%i), Green (%i), Blue (%i)\n", clear_value, red_value, green_value, blue_value);
						maxValueTEST( red_value, green_value, blue_value);
				 
            break;  
				 
         case NORMAL:  
					 
						myled1 = 0;
						myled3 = 1;
						myled2 = 0;		
						pc.printf("NORMAL \n\r");
				 
           			 
            break;  
				 
				 case ADVANCED:  
					 
						myled1 = 0;
						myled3 = 0;
						myled2 = 1;		
						pc.printf("ADVANCED \n\r");
												
           
            break;  
				 
         default:  
					 mode = TEST;
            
      } 
			
    }
}

void maxValueTEST( int redv, int greenv, int bluev){
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
			break;
		case GREEN:
			green = 0;
			red = 1;
			blue = 1;
			break;
		case BLUE:
			blue = 0;
			red = 1;
			green = 1;
			break;
		default:
			break;
	}
	
	
}