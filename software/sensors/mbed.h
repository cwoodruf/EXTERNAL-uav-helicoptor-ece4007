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

#ifndef MBED_H
#define MBED_H

#include "I2C.h"
#include "../util.h"
#include <iostream>

using namespace std;

//Register Addresses
#define MBED_STATUS			0x00
#define MBED_DATA			0x01

#define MBED_MOTOR_ID		0x10
#define MBED_IMU_ID			0x11


class MMBED : public I2C {

	private:

	public:
		MMBED();
		MMBED(int i);

		~MMBED();
		
		int get_status(unsigned char &status);
		int get_data(short int &m1, short int &m2, short int &m3, short int &m4, short int &sonar);
		int set_data(short int m1, short int m2, short int m3, short int m4);
};

MMBED::MMBED() : I2C(3,MBED_MOTOR_ID) {
}

MMBED::MMBED(int i) : I2C(i,MBED_MOTOR_ID) {
}

MMBED::~MMBED() {
}

int MMBED::get_status(unsigned char &status) {
	return read_byte(MBED_STATUS,status);
}

int MMBED::get_data(short int &m1, short int &m2, short int &m3, short int &m4, short int &sonar) {
	unsigned char data[10];
	int status = read_multiple_bytes(MBED_DATA,data,10);

	m1 = ((short int)data[0] << 8) | data[1];
	m2 = ((short int)data[2] << 8) | data[3];
	m3 = ((short int)data[4] << 8) | data[5];
	m4 = ((short int)data[6] << 8) | data[7];
	sonar = ((short int)data[8] << 8) | data[9];

	return status;
}

int MMBED::set_data(short int m1, short int m2, short int m3, short int m4) {
	unsigned char data[8];
	
	data[0] = (unsigned char)(m1 >> 8);
	data[1] = (unsigned char)(m1 && 0x00FF);
	data[2] = (unsigned char)(m2 >> 8);
	data[3] = (unsigned char)(m2 && 0x00FF);
	data[4] = (unsigned char)(m3 >> 8);
	data[5] = (unsigned char)(m3 && 0x00FF);
	data[6] = (unsigned char)(m4 >> 8);
	data[7] = (unsigned char)(m4 && 0x00FF);

	return write_multiple_bytes(MBED_DATA,data,8);
}


class IMBED : public I2C {

	private:

	public:
		IMBED();
		IMBED(int i);

		~IMBED();
		
		int get_status(unsigned char &status);
		int get_data(short int &x, short int &y, short int &z);
};

IMBED::IMBED() : I2C(3,MBED_IMU_ID) {
}

IMBED::IMBED(int i) : I2C(i,MBED_IMU_ID) {
}

IMBED::~IMBED() {
}

int IMBED::get_status(unsigned char &status) {
	return read_byte(MBED_STATUS,status);
}

int IMBED::get_data(short int &x, short int &y, short int &z) {
	unsigned char data[6];
	int status = read_multiple_bytes(MBED_DATA,data,6);

	x = ((short int)data[0] << 8) | data[1];
	y = ((short int)data[2] << 8) | data[3];
	z = ((short int)data[4] << 8) | data[5];

	return status;
}

#endif
