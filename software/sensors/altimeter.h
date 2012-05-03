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

#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "I2C.h"

//IDs
#define ALTIMETER_ID		0x74
#define ALTIMETER_ID_R		0xE9
#define ALTIMETER_ID_W		0xE8

//Registers
#define ALTIMETER_MODE_READ	0x07
#define ALTIMETER_DATA_MSB	0x00
#define ALTIMETER_DATA_LSB	0x01

class Altimeter : public I2C {

	private:
	
	public:
		Altimeter();
		Altimeter(int i);
		Altimeter(bool);
		Altimeter(int i,bool);

		int set_read_mode();
		int get_data_raw(short int &data);

};


Altimeter::Altimeter() : I2C(3,ALTIMETER_ID) {
}

Altimeter::Altimeter(int i) : I2C(i,ALTIMETER_ID) {
}

Altimeter::Altimeter(bool) : I2C(3,ALTIMETER_ID_R,ALTIMETER_ID_W) {
}

Altimeter::Altimeter(int i, bool) : I2C(i,ALTIMETER_ID_R,ALTIMETER_ID_W) {
}


int Altimeter::set_read_mode() {
	return write_byte(ALTIMETER_MODE_READ,0x01);
}

int Altimeter::get_data_raw(short int &data) {
	unsigned char d;
	int status = read_byte(ALTIMETER_DATA_MSB,d);
	data = ((short int)d) << 8;
	status |= read_byte(ALTIMETER_DATA_LSB,d);
	data |= d;
	return status;
}

#endif
