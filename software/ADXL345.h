#ifndef ADXL345_2_H
#define ADXL345_2_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

//Register Addresses
#define ADXL345_DEVID				0x00
#define ADXL345_RESERVED1			0x01
#define ADXL345_THRESH_TAP			0x1D
#define ADXL345_OFSX				0x1E
#define ADXL345_OFSY				0x1F
#define ADXL345_OFSZ				0x20
#define ADXL345_DUR					0x21
#define ADXL345_LATENT				0x22
#define ADXL345_WINDOW				0x23
#define ADXL345_THRESH_ACT			0x24
#define ADXL345_THRESH_INACT		0x25
#define ADXL345_TIME_INACT			0x26
#define ADXL345_ACT_INACT_CTL		0x27
#define ADXL345_THRESH_FF			0x28
#define ADXL345_TIME_FF				0x29
#define ADXL345_TAP_AXES			0x2A
#define ADXL345_ACT_TAP_STATUS		0x2B
#define ADXL345_BW_RATE				0x2C
#define ADXL345_POWER_CTL			0x2D
#define ADXL345_INT_ENABLE			0x2E
#define ADXL345_INT_MAP				0x2F
#define ADXL345_INT_SOURCE			0x30
#define ADXL345_DATA_FORMAT			0x31
#define ADXL345_DATAX0				0x32
#define ADXL345_DATAX1				0x33
#define ADXL345_DATAY0				0x34
#define ADXL345_DATAY1				0x35
#define ADXL345_DATAZ0				0x36
#define ADXL345_DATAZ1				0x37
#define ADXL345_FIFO_CTL			0x38
#define ADXL345_FIFO_STATUS			0x39

//Interrupts
#define ADXL345_INT1_PIN			0x00
#define ADXL345_INT2_PIN			0x01

//Interrupt Bit Positions
#define ADXL345_INT_DATA_READY_BIT	0x07
#define ADXL345_INT_SINGLE_TAP_BIT	0x06
#define ADXL345_INT_DOUBLE_TAP_BIT	0x05
#define ADXL345_INT_ACTIVITY_BIT	0x04
#define ADXL345_INT_INACTIVITY_BIT	0x03
#define ADXL345_INT_FREE_FALL_BIT	0x02
#define ADXL345_INT_WATERMARK_BIT	0x01
#define ADXL345_INT_OVERRUNY_BIT	0x00

//IDs
#define ADXL345_ID					0x53


class ADXL345 {

	private:
		int fd;
		char buf[10];
		int range;
		int status;

		float convert_to_g(unsigned short raw);

	public:
		ADXL345();
		ADXL345(int i);
		~ADXL345();	

		int init(int channel);
		int get_status();
		int write_address(unsigned char reg);
		int write_byte(unsigned char reg, unsigned char data);
		int write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask);
		int read_current_byte(unsigned char *data);
		int read_byte(unsigned char reg, unsigned char *data);
		int measure_mode();
		int standby_mode();
		int set_low_power_mode(unsigned char power);
		int set_range(char range_set);
		int get_range(int *oRange);
		int get_data(float *x, float *y, float *z);
		int get_data_x(float *x);
		int get_data_y(float *y);
		int get_data_z(float *z);

};

ADXL345::ADXL345() {
	init(2);
}

ADXL345::ADXL345(int channel) {
	init(channel);
}

ADXL345::~ADXL345() {

}

int ADXL345::init(int channel) {

	char file[40];
	sprintf(file,"/dev/i2c-%d",channel);

	if((fd = open(file,O_RDWR)) < 0) {
		cout << "ERROR Opening File" << endl;
		status = -1;
		return status;
	}

	if(ioctl(fd,I2C_SLAVE,ADXL345_ID) < 0) {
		cout << "ERROR Opening Device" << endl;
		status = -1;
		return status;
	}

	status = get_range(&range);
	return status;
}

int ADXL345::get_status() {
	return status;
}

int ADXL345::write_address(unsigned char reg) {
	buf[0] = reg;

	if(write(fd,buf,1) != 1) {
		return -1;
	}
	return 0;
}

int ADXL345::write_byte(unsigned char reg, unsigned char data) {
	buf[0] = reg;
	buf[1] = data;

	if(write(fd,buf,2) != 2) {
		return -1;
	}
	return 0;
}

int ADXL345::write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask) {
	unsigned char currentData;

	if(write_address(reg) != 0) {
		return -1;
	}

	if(read_current_byte(&currentData) != 0) {
		return -2;
	}

	data = (currentData & ~mask) | (data & mask);
	return write_byte(reg,data);
}


int ADXL345::read_current_byte(unsigned char *data) {
	if(read(fd,buf,1) != 1) {
		return -1;
	}

	*data = buf[0];
	return 0;
}

int ADXL345::read_byte(unsigned char reg, unsigned char *data) {
	if(write_address(reg) != 0) {
		return -1;
	}

	return read_current_byte(data);
}

float ADXL345::convert_to_g(unsigned short raw) {
	bool neg = false;
	float res;

	if((raw >> 15) == 1) {
		raw = -raw + 1;
		neg = true;
	}

	res = (float)raw;
	if(neg) {
		res *= -1;
	}

	return (float)range * (res/0x1FF);
}

int ADXL345::measure_mode() {
	return write_masked_byte(ADXL345_POWER_CTL,0x08,0x08);
}

int ADXL345::standby_mode() {
	return write_masked_byte(ADXL345_POWER_CTL,0x00,0x08);
}

int ADXL345::set_low_power_mode(unsigned char power) {
	return write_masked_byte(ADXL345_BW_RATE, power<<3, 0x10);
}

int ADXL345::set_range(char range_set) {
	unsigned char rate = 0x0F;

	switch(range_set) {
		case 0x02:
			rate = 0x00;
			break;
		case 0x04:
			rate = 0x01;
			break;
		case 0x08:
			rate = 0x02;
			break;
		case 0x10:
			rate = 0x03;
			break;
		default:
			return -1;
	}

	if(write_masked_byte(ADXL345_DATA_FORMAT,rate,0x03) != 0) {
		return -2;
	}

	range = range_set;
	return 0;
}

int ADXL345::get_range(int *oRange) {
	unsigned char data;

	if(read_byte(ADXL345_DATA_FORMAT,&data) != 0) {
		return -1;
	}

	data = data & 0x03;

	switch(data) {
		case 0x00:
			*oRange = 2;
			break;
		case 0x01:
			*oRange = 4;
			break;
		case 0x02:
			*oRange = 8;
			break;
		case 0x03:
			*oRange = 16;
			break;
		default:
			return -1;
	}

	return 0;
}

int ADXL345::get_data(float *x, float *y, float *z) {
	int res = get_data_x(x);
	res += get_data_y(y);
	res += get_data_z(z);
	return res;
}

int ADXL345::get_data_x(float *x) {
	unsigned char data;
	unsigned short raw;

	if(read_byte(ADXL345_DATAX0, &data) != 0) {
		return -1;
	}

	raw = data;

	if(read_byte(ADXL345_DATAX1, &data) != 0) {
		return -1;
	}

	raw += data << 8;

	*x = convert_to_g(raw);
	return 0;
}

int ADXL345::get_data_y(float *y) {
	unsigned char data;
	unsigned short raw;

	if(read_byte(ADXL345_DATAY0, &data) != 0) {
		return -1;
	}

	raw = data;

	if(read_byte(ADXL345_DATAY1, &data) != 0) {
		return -1;
	}

	raw += data << 8;

	*y = convert_to_g(raw);
	return 0;
}

int ADXL345::get_data_z(float *z) {
	unsigned char data;
	unsigned short raw;

	if(read_byte(ADXL345_DATAZ0, &data) != 0) {
		return -1;
	}

	raw = data;

	if(read_byte(ADXL345_DATAZ1, &data) != 0) {
		return -1;
	}

	raw += data << 8;

	*z = convert_to_g(raw);
	return 0;
}


#endif
