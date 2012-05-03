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
#include "UART.h"
#include "../util.h"
#include <termios.h>
#include <iostream>

using namespace std;


class MMBED : public UART {

	private:

	public:
		MMBED();
		MMBED(const char *port, speed_t baud);
		~MMBED();
		
		int get_status(int &status);
		int get_data(int &m1, int &m2, int &m3, int &m4, int &s);
		int set_setpoints(int m1, int m2, int m3, int m4);
};

MMBED::MMBED() : UART("/dev/ttyO2",B115200) {
}

MMBED::MMBED(const char *port, speed_t baud) : UART(port,baud) {	
}

MMBED::~MMBED() {
}

int MMBED::get_status(int &status) {
	char buffer[32];
	int n;
	writeline("S");
	if((n = readline(buffer,32)) > 0) {
		sscanf(buffer,"%d",&status);
	}
	return n;
}

int MMBED::get_data(int &m1, int &m2, int &m3, int &m4, int &s) {
	char buffer[256];
	int n;
	writeline("D");
	if((n = readline(buffer,256)) > 0) {
		sscanf(buffer,"%d,%d,%d,%d,%d",&m1,&m2,&m3,&m4,&s);
	}
	return n;
}

int MMBED::set_setpoints(int m1, int m2, int m3, int m4) {
	char buffer[256];
	sprintf(buffer,"%d,%d,%d,%d\n",m1,m2,m3,m4);
	return writeline(buffer);
}


class IMBED : public UART {

	private:

	public:
		IMBED();
		IMBED(const char *port, speed_t baud);
		~IMBED();
		
		int get_status(int &status);
		int get_data(int &x, int &y, int &z);
};

IMBED::IMBED() : UART("/dev/ttyO1",B115200) {
}

IMBED::IMBED(const char *port, speed_t baud) : UART(port,baud) {	
}

IMBED::~IMBED() {
}

int IMBED::get_status(int &status) {
	char buffer[32];
	int n;
	writeline("S");
	if((n = readline(buffer,32)) > 0) {
		sscanf(buffer,"%d",&status);
	}
	return n;
}

int IMBED::get_data(int &x, int &y, int &z) {
	char buffer[256];
	int n;
	writeline("D");
	if((n = readline(buffer,256)) > 0) {
		sscanf(buffer,"%d,%d,%d",&x,&y,&z);
	}
	return n;
}

#endif
