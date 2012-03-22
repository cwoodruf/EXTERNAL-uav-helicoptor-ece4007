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
#define MBED_WHOAMI		0x00
#define MBED_MOTOR1		0x01
#define MBED_MOTOR2		0x02
#define MBED_MOTOR3		0x03
#define MBED_MOTOR4		0x04
#define MBED_SONAR		0x05

#define MBED_ID			0x10

class MBED : public I2C {

	private:
		unsigned char range;

	public:
		MBED();
		MBED(int i);

		~MBED();
		

		int get_whoami(unsigned short int &id);
};

MBED::MBED() : I2C(3,MBED_ID) {
}

MBED::MBED(int i) : I2C(i,MBED_ID) {
}

MBED::~MBED() {
}

int MBED::get_whoami(unsigned short int &id) {
	return read_byte(MBED_WHOAMI,id)
}
