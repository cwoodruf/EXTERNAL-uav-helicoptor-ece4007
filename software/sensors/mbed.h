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
#define MBED_WHOAMI			0x00
#define MBED_STATUS			0x01
#define MBED_MOTOR_1_HB		0x02
#define MBED_MOTOR_1_LB		0x03
#define MBED_MOTOR_2_HB		0x04
#define MBED_MOTOR_2_LB		0x05
#define MBED_MOTOR_3_HB		0x06
#define MBED_MOTOR_3_LB		0x07
#define MBED_MOTOR_4_HB		0x08
#define MBED_MOTOR_4_LB		0x09
#define MBED_SONAR_HB		0x0A
#define MBED_SONAR_LB		0x0B

#define MBED_ID				0x10


#define MBED_STATUS_READY				0x01
#define MBED_STATUS_PID_INIT_ERROR		0x02
#define MBED_STATUS_PID_REGULATE_ERROR	0x04

class MBED : public I2C {

	private:

	public:
		MBED();
		MBED(int i);

		~MBED();
		
		int get_whoami(unsigned char &id);
		int get_status(unsigned char &status);
		int get_motor_1(short int &rpm);
		int set_motor_1(short int rpm);
		int get_motor_2(short int &rpm);
		int set_motor_2(short int rpm);
		int get_motor_3(short int &rpm);
		int set_motor_3(short int rpm);
		int get_motor_4(short int &rpm);
		int set_motor_4(short int rpm);
		int get_sonar(short int &in);
};

MBED::MBED() : I2C(3,MBED_ID) {
}

MBED::MBED(int i) : I2C(i,MBED_ID) {
}

MBED::~MBED() {
}

int MBED::get_whoami(unsigned char &id) {
	return read_byte(MBED_WHOAMI,id);
}

int MBED::get_status(unsigned char &status) {
	return read_byte(MBED_STATUS,status);
}

int MBED::get_motor_1(short int &rpm) {
	unsigned char d;
	int status = read_byte(MBED_MOTOR_1_HB,d);
	rpm = d << 8;
	status |= read_byte(MBED_MOTOR_1_LB,d);
	rpm |= d;
	return status;
}

int MBED::set_motor_1(short int rpm) {
	unsigned char dlb = rpm & 0x00FF;
	unsigned char dhb = rpm >> 8;
	int status = write_byte(MBED_MOTOR_1_HB,dhb);
	return status | write_byte(MBED_MOTOR_1_LB,dlb);
}

int MBED::get_motor_2(short int &rpm) {
	unsigned char d;
	int status = read_byte(MBED_MOTOR_2_HB,d);
	rpm = d << 8;
	status |= read_byte(MBED_MOTOR_2_LB,d);
	rpm |= d;
	return status;
}

int MBED::set_motor_2(short int rpm) {
	unsigned char dlb = rpm & 0x00FF;
	unsigned char dhb = rpm >> 8;
	int status = write_byte(MBED_MOTOR_2_HB,dhb);
	return status | write_byte(MBED_MOTOR_2_LB,dlb);
}

int MBED::get_motor_3(short int &rpm) {
	unsigned char d;
	int status = read_byte(MBED_MOTOR_3_HB,d);
	rpm = d << 8;
	status |= read_byte(MBED_MOTOR_3_LB,d);
	rpm |= d;
	return status;
}

int MBED::set_motor_3(short int rpm) {
	unsigned char dlb = rpm & 0x00FF;
	unsigned char dhb = rpm >> 8;
	int status = write_byte(MBED_MOTOR_3_HB,dhb);
	return status | write_byte(MBED_MOTOR_3_LB,dlb);
}

int MBED::get_motor_4(short int &rpm) {
	unsigned char d;
	int status = read_byte(MBED_MOTOR_4_HB,d);
	rpm = d << 8;
	status |= read_byte(MBED_MOTOR_4_LB,d);
	rpm |= d;
	return status;
}

int MBED::set_motor_4(short int rpm) {
	unsigned char dlb = rpm & 0x00FF;
	unsigned char dhb = rpm >> 8;
	int status = write_byte(MBED_MOTOR_4_HB,dhb);
	return status | write_byte(MBED_MOTOR_4_LB,dlb);
}

int MBED::get_sonar(short int &in) {
	unsigned char d;
	int status = read_byte(MBED_SONAR_HB,d);
	in = d << 8;
	status |= read_byte(MBED_SONAR_LB,d);
	in |= d;
	return status;
}

#endif
