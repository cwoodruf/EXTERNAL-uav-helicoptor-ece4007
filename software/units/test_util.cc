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



#include "../util.h"
#include <iostream>

using namespace std;

T_CharCurvePoint curve[4] = {
	{0,0},
	{1,1},
	{9,9},
	{10,10}
};


int main() {

	cout << "Testing MIN..." << ((MIN(5,23) == 5) ? "Pass":"Fail") << endl;
	cout << "Testing MAX..." << ((MAX(5,23) == 23) ? "Pass":"Fail") << endl;
	cout << "Testing LIMIT..." << (((LIMIT(1,-10,100) == 1) && (LIMIT(1,110,100) == 100) && (LIMIT(1,10,100) == 10)) ? "Pass":"Fail") << endl;
	cout << "Testing ABSV..." << (((ABSV(-10) == 10) && (ABSV(10) == 10)) ? "Pass":"Fail") << endl;

	cout << "Testing swap...";
	int a = 10, b = 20;
	swap(&a,&b);
	cout << (((a == 20) && (b == 10)) ? "Pass":"Fail") << endl;

	cout << "Testing scale...";
	unsigned short v1 = scale<unsigned char,unsigned short>(10,100,true);
	unsigned short v2 = scale<unsigned short,unsigned char>(1000,100,false);
	cout << (((v1 == 1000) && (v2 == 10)) ? "Pass":"Fail") << endl;
	
	cout << "Testing inDeadBand..." << ((inDeadBand(10,12,5) && !inDeadBand(10,16,5)) ? "Pass":"Fail") << endl;

	
	cout << "Testing char_curve..." << ((char_curve(curve,5) == 5) ? "Pass":"Fail") << endl;

	cout << "Testing ton_delay...Please Wait 5 seconds...";
	T_TONDelay ton = {false,false,5.0,{0}};
	cout.flush();

	while(!ton_delay(ton,true)) {;}
	cout << "Pass" << endl;

	cout << "Testing toff_delay...Please Wait 5 seconds...";
	T_TONDelay toff = {true,true,5.0,{0}};
	cout.flush();

	while(toff_delay(toff,false)) {;}
	cout << "Pass" << endl;

	return 0;
}
