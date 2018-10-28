#include "mbed.h"
#include "rtos.h"

//DIEGO GARCIA DIAZ y ANDREAS BECK

DigitalOut myled1(LED1); //test mode
DigitalOut myled2(LED2); //normal mode
DigitalOut myled3(LED3); //advanced mode

Serial pc(USBTX,USBRX,19200);

//Analog stuff
extern Thread threadANALOG;
extern void ANALOG_thread();
extern float valueSS; 

//I2C stuff
extern Thread threadI2C;
extern void I2C_thread();
extern float rhData; //Humidity
extern float tData; //Temperature
extern double clear_value, red_value, green_value, blue_value; //Colors



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
    

    threadANALOG.start(ANALOG_thread);  
		threadI2C.start(I2C_thread);

	
   	
    while (true) {		  
			
					
     
			
			pc.printf("VALOR %.lf ", valueSS);
			
			switch ( mode )  
      {  
         case TEST:
					  wait(2);
						myled1 = 1;
						myled3 = 0;
						myled2 = 0;					 
            pc.printf("TEST \n\r");
						
						
				 
						pc.printf("Soil %f", valueSS);
						pc.printf("Hum %f", rhData);
						pc.printf("Temp %f", tData);
				 		pc.printf("Clear (%d), Red (%d), Green (%d), Blue (%d)\n", clear_value, red_value, green_value, blue_value);

				 
				
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