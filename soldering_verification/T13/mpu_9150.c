#include "mpu_9150.h"
#include "mpu_i2c.h"

void MPU9150_setupCompass() {
	write_i2c(0x0A, 0x00); //PowerDownMode
	write_i2c(0x0A, 0x0F); //SelfTest
	write_i2c(0x0A, 0x00); //PowerDownMode
	write_i2c(0x24, 0x40); //Wait for Data at Slave0
	write_i2c(0x25, 0x8C); //Set i2c address at slave0 at 0x0C
	write_i2c(0x26, 0x02); //Set where reading at slave 0 starts
	write_i2c(0x27, 0x88); //set offset at start reading and enable
	write_i2c(0x28, 0x0C); //set i2c address at slv1 at 0x0C
	write_i2c(0x29, 0x0A); //Set where reading at slave 1 starts
	write_i2c(0x2A, 0x81); //Enable at set length to 1
	write_i2c(0x64, 0x01); //overvride register
	write_i2c(0x67, 0x03); //set delay rate
	write_i2c(0x01, 0x80);
	write_i2c(0x34, 0x04); //set i2c slv4 delay
	write_i2c(0x64, 0x00); //override register
	write_i2c(0x6A, 0x00); //clear usr setting
	write_i2c(0x64, 0x01); //override register
	write_i2c(0x6A, 0x20); //enable master i2c mode
	write_i2c(0x34, 0x13); //disable slv4
}
/*
int MPU9150_readSensor(int addrH, int addrL) {
	int H, L;
	H = read_i2c(addrH);
	clear();
	L = read_i2c(addrL);
	return (int) ((H << 8) + L);
}
*/
