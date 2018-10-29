
// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C
#include "mbed.h"
#include "rtos.h"

Thread threadI2C(osPriorityNormal, 512); // 1K stack size

void I2C_thread(); 
 
I2C i2c(PB_9, PB_8); //pins for I2C communication (SDA, SCL)
extern Serial pc;
extern int timeToWait;
extern int mode;

char tx_buff[2] = {0,0};
char rx_buff[2] = {0,0};


//COLOR SENSOR VALUES
int TSC34725_ADDR = 41 << 1; // Color sensor address
int clear_value, red_value, green_value, blue_value;
int CLEAR_REG = 148;
int BLUE_REG = 154;
int RED_REG = 150;
int GREEN_REG = 152;

//SI7021 VALUES
int SI7021_ADDR = 64 << 1 ; // Temperature and humidity sensor address
int READ_ID2_1 = 252;
int READ_ID2_2 = 201;
int READ_RH = 229;
int READ_TEMP = 224;

int DEVICE_ID = 0;
float rhData = 0;
float tData = 0;



void I2C_thread() {
    
    i2c.frequency(100000);
	
	// Initialize hum/temp sensor
	
		tx_buff[0] = READ_ID2_1;
	  tx_buff[1] = READ_ID2_2;
		
    i2c.write( SI7021_ADDR ,(char*)tx_buff,2, true);
    i2c.read(SI7021_ADDR,(char*)rx_buff,1,false);
			
		
		// Initialize color sensor
    
    char id_regval[1] = {146};
    char data[1] = {0};
    i2c.write(TSC34725_ADDR,id_regval,1, true);
    i2c.read(TSC34725_ADDR,data,1,false);
     
    
    char timing_register[2] = {129,0};//0x81
    i2c.write(TSC34725_ADDR,timing_register,2,false);
    
    char control_register[2] = {143,0}; //0x8F
    i2c.write(TSC34725_ADDR,control_register,2,false);
    
    char enable_register[2] = {128,3}; //0x80
    i2c.write(TSC34725_ADDR,enable_register,2,false);
  
				
		
	while (true) {

		//READ HUMIDITY
		tx_buff[0] = READ_RH;
		
		i2c.write(SI7021_ADDR, (char*)tx_buff, 1);
		i2c.read(SI7021_ADDR, (char*)rx_buff, 2);

		rhData = (float) ((uint32_t)rx_buff[0] << 8) + rx_buff[1];
		rhData = (rhData*125/65536)-6;
		//printf("Hum %f", rhData);
		
		
		//READ TEMPERATURE
		tx_buff[0] = READ_TEMP;
		
		i2c.write(SI7021_ADDR, (char*)tx_buff, 1);
		i2c.read(SI7021_ADDR, (char*)rx_buff, 2);

		tData = (float)((uint32_t) rx_buff[0] << 8) + (rx_buff[1]);
		tData = (tData*175.72/65536)-46.85;
		
		
		/*
		//READ COLORS
		
		tx_buff[1] = CLEAR_REG;
		i2c.write(TSC34725_ADDR,(char*)tx_buff,1);
		i2c.read(TSC34725_ADDR,(char*)rx_buff,2);
		
		clear_value = ((int)rx_buff[1] << 8) | rx_buff[0];
		
		tx_buff[1] = RED_REG;
		i2c.write(TSC34725_ADDR,(char*)tx_buff,1, true);
		i2c.read(TSC34725_ADDR,(char*)rx_buff,2, false);
		
		red_value = ((int)rx_buff[1] << 8) | rx_buff[0];
		
		tx_buff[1] = GREEN_REG;
		i2c.write(TSC34725_ADDR,(char*)tx_buff,1, true);
		i2c.read(TSC34725_ADDR,(char*)rx_buff,2, false);
		
		green_value = ((int)rx_buff[1] << 8) | rx_buff[0];
		
		tx_buff[1] = BLUE_REG;
		i2c.write(TSC34725_ADDR,(char*)tx_buff,1, true);
		i2c.read(TSC34725_ADDR,(char*)rx_buff,2, false);
		
		blue_value = ((int)rx_buff[1] << 8) | rx_buff[0];
		*/
		
		//READ COLORS
		
		char clear_reg[1] = {148};
		char clear_data[2] = {0,0};
		i2c.write(TSC34725_ADDR,clear_reg,1, true);
		i2c.read(TSC34725_ADDR,clear_data,2, false);
		
		clear_value = ((int)clear_data[1] << 8) | clear_data[0];
		
		char red_reg[1] = {150};
		char red_data[2] = {0,0};
		i2c.write(TSC34725_ADDR,red_reg,1, true);
		i2c.read(TSC34725_ADDR,red_data,2, false);
		
		red_value = ((int)red_data[1] << 8) | red_data[0];
		
		char green_reg[1] = {152};
		char green_data[2] = {0,0};
		i2c.write(TSC34725_ADDR,green_reg,1, true);
		i2c.read(TSC34725_ADDR,green_data,2, false);
		
		green_value = ((int)green_data[1] << 8) | green_data[0];
		
		char blue_reg[1] = {154};
		char blue_data[2] = {0,0};
		i2c.write(TSC34725_ADDR,blue_reg,1, true);
		i2c.read(TSC34725_ADDR,blue_data,2, false);
		
		blue_value = ((int)blue_data[1] << 8) | blue_data[0];
		
		
		wait(timeToWait);
	}
}  
