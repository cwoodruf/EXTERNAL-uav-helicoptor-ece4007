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
#include "../../events/timeout.h"
#include "../../io/gpio.h"

GPIO g;
bool NEXT = false;

void pulseoff() {
	g.set_value(0);
	NEXT = true;
}

int main() {
	g.init("P9_15");
	g.set_dir("out");

	register_timeout(pulseoff,1.000);
	g.set_value(1); while(!NEXT) {;} NEXT = false;
	register_timeout(pulseoff,0.100);
	g.set_value(1); while(!NEXT) {;} NEXT = false;
	register_timeout(pulseoff,0.010);
	g.set_value(1); while(!NEXT) {;} NEXT = false;
	register_timeout(pulseoff,0.001);

	g.detach();
	return 0;
}
