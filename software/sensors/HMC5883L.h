/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef HMC5883L_H
#define HMC5883L_H

#include "I2C.h"
#include "../util.h"
#include <iostream>

using namespace std;

#define HMC5883L_ID			0X1E

//Register Addresses
#define HMC5883L_CONFIG_A			0x00
#define HMC5883L_CONFIG_B			0x01
#define HMC5883L_MODE				0x02
#define HMC5883L_DATA_X_MSB			0x03
#define HMC5883L_DATA_X_LSB			0x04
#define HMC5883L_DATA_Z_MSB			0x05
#define HMC5883L_DATA_Z_LSB			0x06
#define HMC5883L_DATA_Y_MSB			0x07
#define HMC5883L_DATA_Y_LSB			0x08
#define HMC5883L_STATUS				0x09
#define HMC5883L_ID_A				0x0A
#define HMC5883L_ID_B				0x0B
#define HMC5883L_ID_C				0x0C

//Samples to Avg - Part of Config A
#define HMC5883L_SAMPLES_AVG_1		0x00
#define HMC5883L_SAMPLES_AVG_2		0x01
#define HMC5883L_SAMPLES_AVG_4		0x02
#define HMC5883L_SAMPLES_AVG_8		0x03

//Data Output Rate (Hz) - Part of Config A
#define HMC5883L_OUTPUT_RATE_0_75		0x00
#define HMC5883L_OUTPUT_RATE_1_5		0x01
#define HMC5883L_OUTPUT_RATE_3_0		0x02
#define HMC5883L_OUTPUT_RATE_7_5		0x03
#define HMC5883L_OUTPUT_RATE_15_0		0x04
#define HMC5883L_OUTPUT_RATE_30_0		0x05
#define HMC5883L_OUTPUT_RATE_75_0		0x06

//Measurement Mode - Part of Config A
#define HMC5883L_MEASURE_MODE_NORM			0x00
#define HMC5883L_MEASURE_MODE_POS_BIAS		0x01
#define HMC5883L_MEASURE_MODE_NEG_BIAS		0x02

//Gain Configuration (LSB/Gauss) - For Config B
#define HMC5883L_GAIN_1370		0x00		//Field Range: +-0.88 Ga
#define HMC5883L_GAIN_1090		0x01		//Field Range: +-1.3  Ga
#define HMC5883L_GAIN_820		0x02		//Field Range: +-1.9  Ga
#define HMC5883L_GAIN_660		0x03		//Field Range: +-2.5  Ga
#define HMC5883L_GAIN_440		0x04		//Field Range: +-4.0  Ga
#define HMC5883L_GAIN_390		0x05		//Field Range: +-4.7  Ga
#define HMC5883L_GAIN_330		0x06		//Field Range: +-5.6  Ga
#define HMC5883L_GAIN_230		0x07		//Field Range: +-8.1  Ga

//Chip Mode
#define HMC5883L_MODE_CONTINUOUS_MEASUREMENT	0x00
#define HMC5883L_MODE_SINGLE_MEASUREMENT		0x01
#define HMC5883L_MODE_IDLE						0x02
#define HMC5883L_MODE_IDLE2						0x03

//Status
#define HMC5883L_STATUS_LOCK	0x00
#define HMC5883L_STATUS_RDY 	0x01

class HMC5883L : public I2C {

	private:

	public:
		//Constructors
		HMC5883L();
		HMC5883L(int i);

		//Destructors
		~HMC5883L();

		//Methods
		int get_config_a(unsigned char &MA, unsigned char &DO, unsigned char &MS);
		int set_config_a(unsigned char MA, unsigned char DO, unsigned char MS);
		int get_config_b(unsigned char &GN);
		int set_config_b(unsigned char GN);
		int get_data_x_raw(unsigned short int &x);
		int get_data_y_raw(unsigned short int &y);
		int get_data_z_raw(unsigned short int &z);
		int get_status(unsigned char &status);
		int get_id_a(unsigned char &id);
		int get_id_b(unsigned char &id);
		int get_id_c(unsigned char &id);
};


HMC5883L::HMC5883L() : I2C(3,HMC5883L_ID) {
}

HMC5883L::HMC5883L(int i) : I2C(i,HMC5883L_ID) {
}

HMC5883L::~HMC5883L() {
}

int HMC5883L::get_config_a(unsigned char &MA, unsigned char &DO, unsigned char &MS) {
	unsigned char reg;
	int status = read_byte(HMC5883L_CONFIG_A,reg);

	MA = (reg  >> 5);
	DO = ((reg & 0x1F) >> 2);
	MS = (reg & 0x3);

	return status;
}

int HMC5883L::set_config_a(unsigned char MA, unsigned char DO, unsigned char MS) {
	unsigned char reg = 0x00;
	reg |= (MA & 0x03) << 5;
	reg |= (DO & 0x07) << 2;
	reg |= (MA & 0x03);

	return write_byte(HMC5883L_CONFIG_A,reg);
}

int HMC5883L::get_config_b(unsigned char &GN) {
	int status = read_byte(HMC5883L_CONFIG_B,GN);
	GN >>= 5;

	return status;
}

int HMC5883L::set_config_b(unsigned char GN) {
	unsigned char reg = GN << 5;
	return write_byte(HMC5883L_CONFIG_B,reg);
}

int HMC5883L::get_data_x_raw(unsigned short int &x) {
	unsigned char lsb = 0;
	unsigned char msb = 0;

	int status = read_byte(HMC5883L_DATA_X_LSB,lsb);
	status += read_byte(HMC5883L_DATA_X_MSB,msb);

	x = ((short int)msb << 8) | lsb;

	return status;
}

int HMC5883L::get_data_y_raw(unsigned short int &y) {
	unsigned char lsb = 0;
	unsigned char msb = 0;

	int status = read_byte(HMC5883L_DATA_Y_LSB,lsb);
	status += read_byte(HMC5883L_DATA_Y_MSB,msb);

	y = ((short int)msb << 8) | lsb;

	return status;
}

int HMC5883L::get_data_z_raw(unsigned short int &z) {
	unsigned char lsb = 0;
	unsigned char msb = 0;

	int status = read_byte(HMC5883L_DATA_Z_LSB,lsb);
	status += read_byte(HMC5883L_DATA_Z_MSB,msb);

	z = ((short int)msb << 8) | lsb;

	return status;
}

int HMC5883L::get_status(unsigned char &status) {
	return read_byte(HMC5883L_STATUS,status);
}

int HMC5883L::get_id_a(unsigned char &id) {
	return read_byte(HMC5883L_ID_A,id);
}

int HMC5883L::get_id_b(unsigned char &id) {
	return read_byte(HMC5883L_ID_B,id);
}

int HMC5883L::get_id_c(unsigned char &id){
	return read_byte(HMC5883L_ID_C,id);
}

#endif
