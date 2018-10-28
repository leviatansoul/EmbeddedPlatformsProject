/*
// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C
#include "mbed.h"
#include "rtos.h"

Thread threadI2C(osPriorityNormal, 512); // 1K stack size

void I2C_thread(); 
 
I2C i2c(PB_9, PB_8); //pins for I2C communication (SDA, SCL)

char tx_buff[2] = {0,0};
char rx_buff[2] = {0,0};


//COLOR SENSOR VALUES
int TSC34725_ADDR = 41 << 1; // Color sensor address
double clear_value, red_value, green_value, blue_value;
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
	
		tx_buff[0] = READ_ID2_1;
	  tx_buff[1] = READ_ID2_2;
		
    i2c.write( SI7021_ADDR ,(char*)tx_buff,2, true);
    i2c.read(SI7021_ADDR,(char*)rx_buff,1,false);
		//printf("Id %i", (int)rx_buff[0]);
	
		

		char id_regval[1] = {146};
    char data[1] = {0};
    i2c.write(TSC34725_ADDR,id_regval,1, true);
    i2c.read(TSC34725_ADDR,data,1,false);
     
		
		// Initialize color sensor
    
    char timing_register[2] = {129,0};
    i2c.write(TSC34725_ADDR,timing_register,2);
    
    char control_register[2] = {143,0};
    i2c.write(TSC34725_ADDR,control_register,2);
    
    char enable_register[2] = {128,3};
    i2c.write(TSC34725_ADDR,enable_register,2);
  
				
		
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
		
		
		wait(2);
	}
}  
*/