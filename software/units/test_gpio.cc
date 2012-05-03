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
#include "../io/gpio.h"
#include "../events/timeout.h"
#include <iostream>

using namespace std;

GPIO g;
bool go = true;
double interval = 0.0;

void catchit(int) {
	go = false;
	g.detach();
}

void blink() {
	static int value = 0;
	value = !value;
	g.set_value(value);
	register_timeout(blink,interval);
}

int main(int argc, char **argv) {

	if(argc != 2) {
		cout << "usage: blink [period]" << endl;
		return -1;
	}

	interval = atoi(argv[1]);

	g.init("P9_15");
	g.set_dir("out");

	signal(SIGINT,catchit);

	register_timeout(blink,interval);

	cout << "Blinking at " << (1/atof(argv[1])) << " Hz - 50\% Duty Cycle" << endl; 

	while(go) {
		;
	}

	return 0;
}
