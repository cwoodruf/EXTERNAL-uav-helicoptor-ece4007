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
#include <sys/time.h>
#include <iostream>

using namespace std;

int main() {
	T_TONDelay delay0 = {false,false,1.000,{0}};
	T_TONDelay delay1 = {false,false,0.100,{0}};
	T_TONDelay delay2 = {false,false,0.010,{0}};
	T_TONDelay delay3 = {false,false,0.001,{0}};	

	struct timeval start,end;
	double s,us,t;

	gettimeofday(&start,NULL);
	while(!ton_delay(delay0,true)) {;}
	gettimeofday(&end,NULL);

	s = (double)(end.tv_sec - start.tv_sec);
	us = (double)(end.tv_usec - start.tv_usec);
	t = s + us/1000000;
	cout << "Expected 1.000s delay...got " << t << "s" << endl;

	gettimeofday(&start,NULL);
	while(!ton_delay(delay1,true)) {;}
	gettimeofday(&end,NULL);

	s = (double)(end.tv_sec - start.tv_sec);
	us = (double)(end.tv_usec - start.tv_usec);
	t = s + us/1000000;
	cout << "Expected 0.100s delay...got " << t << "s" << endl;

	gettimeofday(&start,NULL);
	while(!ton_delay(delay2,true)) {;}
	gettimeofday(&end,NULL);

	s = (double)(end.tv_sec - start.tv_sec);
	us = (double)(end.tv_usec - start.tv_usec);
	t = s + us/1000000;
	cout << "Expected 0.010s delay...got " << t << "s" << endl;

	gettimeofday(&start,NULL);
	while(!ton_delay(delay3,true)) {;}
	gettimeofday(&end,NULL);

	s = (double)(end.tv_sec - start.tv_sec);
	us = (double)(end.tv_usec - start.tv_usec);
	t = s + us/1000000;
	cout << "Expected 0.001s delay...got " << t << "s" << endl;

	return 0;
}
