
// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C
#include "mbed.h"
#include "rtos.h"

Thread threadColorSensor(osPriorityNormal, 512); // 1K stack size

void ColorSensor_thread(); 
 
I2C i2c(PB_9, PB_8); //pins for I2C communication (SDA, SCL)


int sensor_addr = 41 << 1;
int clear_value, red_value, green_value, blue_value;



void ColorSensor_thread() {
    
    i2c.frequency(100000);
	
		char id_regval[1] = {146};
    char data[1] = {0};
    i2c.write(sensor_addr,id_regval,1, true);
    i2c.read(sensor_addr,data,1,false);
     
		
		// Initialize color sensor
    
    char timing_register[2] = {129,0};
    i2c.write(sensor_addr,timing_register,2,false);
    
    char control_register[2] = {143,0};
    i2c.write(sensor_addr,control_register,2,false);
    
    char enable_register[2] = {128,3};
    i2c.write(sensor_addr,enable_register,2,false);
				
		
	while (true) {

		// Read data from color sensor (Clear/Red/Green/Blue)
		
		char clear_reg[1] = {148};
		char clear_data[2] = {0,0};
		i2c.write(sensor_addr,clear_reg,1, true);
		i2c.read(sensor_addr,clear_data,2, false);
		
		clear_value = ((int)clear_data[1] << 8) | clear_data[0];
		
		char red_reg[1] = {150};
		char red_data[2] = {0,0};
		i2c.write(sensor_addr,red_reg,1, true);
		i2c.read(sensor_addr,red_data,2, false);
		
		red_value = ((int)red_data[1] << 8) | red_data[0];
		
		char green_reg[1] = {152};
		char green_data[2] = {0,0};
		i2c.write(sensor_addr,green_reg,1, true);
		i2c.read(sensor_addr,green_data,2, false);
		
		green_value = ((int)green_data[1] << 8) | green_data[0];
		
		char blue_reg[1] = {154};
		char blue_data[2] = {0,0};
		i2c.write(sensor_addr,blue_reg,1, true);
		i2c.read(sensor_addr,blue_data,2, false);
		
		blue_value = ((int)blue_data[1] << 8) | blue_data[0];
		
		// print sensor readings
		
		wait(2.0);
	}
}   
