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

#include "../controls/pid.h"
#include "../time/timeout.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

PID pid;

void control_loop() {

	static unsigned short int value = 0;
	unsigned short int out = 0;

	if(!inDeadBand(1234,value,5)) {
		if(pid.regulate(1234,value,out) != 0) {
			cout << "ERROR" << endl;
		}

		if(value > 1234) value -= out;
		else value += out;

		cout << "out: " << out << "\t";
		cout << "newVal: " << value << endl;
	} else {
		cout << "In Deadband at: " << value << endl;
	}

	register_timeout(control_loop,0.05);
}


int main(int argc, char **argv) {

	if(pid.init(40,50,0,50,100) < 0) {
		cout << "Warning Unstable PID Parameters" << endl;
		return -1;
	}

	register_timeout(control_loop,0.05);
	
	while(1) {;}
	return 0;
}
