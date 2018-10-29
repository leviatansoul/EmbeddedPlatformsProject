#include "mbed.h"
#include "rtos.h"


//DIEGO GARCIA DIAZ y ANDREAS BECK

DigitalOut myled1(LED1); //test mode
DigitalOut myled2(LED2); //normal mode
DigitalOut myled3(LED3); //advanced mode

Serial pc(USBTX,USBRX,9600);




extern Thread threadANALOG;
extern void ANALOG_thread();
extern float valueLS; 

//ThreadGPS
extern Thread threadGPS;
extern void GPS_thread();
extern float gpsTime, longitude;

extern bool noanswer;

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
    
		pc.printf("Inicializing Threads");
    //threadANALOG.start(ANALOG_thread);  
		threadGPS.start(GPS_thread);
		pc.printf("DONE");
	
   	
    while (true) {		  
		
			
			switch ( mode )  
      {  
         case TEST:
					  wait(2);
						myled1 = 1;
						myled3 = 0;
						myled2 = 0;					 
            pc.printf("TEST \n\r");
				 
						pc.printf("Time: %f\n\r Longitude: %f", gpsTime, longitude);
				 
					if (noanswer) {
						pc.printf("NO SAMPLE()");
					}
						

				 
				
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