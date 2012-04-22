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
#include "../../util.h"
#include "../../io/gpio.h"

using namespace std;

int main() {
	GPIO g;
	g.init("P9_15");
	g.set_dir("out");

	T_TONDelay delay0 = {false,false,1.000,{0}};
	T_TONDelay delay1 = {false,false,0.100,{0}};
	T_TONDelay delay2 = {false,false,0.010,{0}};
	T_TONDelay delay3 = {false,false,0.001,{0}};

	g.set_value(1);	while(!ton_delay(delay0,true)) {;} g.set_value(0);
	g.set_value(1);	while(!ton_delay(delay1,true)) {;} g.set_value(0);
	g.set_value(1);	while(!ton_delay(delay2,true)) {;} g.set_value(0);
	g.set_value(1);	while(!ton_delay(delay3,true)) {;} g.set_value(0);

	g.detach();

	return 0;
}
