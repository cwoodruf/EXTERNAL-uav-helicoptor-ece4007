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

#include "../datastructures/list.h"
#include <iostream>

using namespace std;

int main() {

	List<int> l;
	bool status = true;
	int v;

	for(int i=0;i<100;++i) {
		l.push(i);
	}

	for(int i=0;i<100;++i) {
		l.pop(v);
		status &= (v == 100-i-1);
	}

	cout << "Testing push/pop..." << (status?"Pass":"Fail") << endl;

	for(int i=0;i<100;++i) {
		l.push_front(i);
	}

	for(int i=0;i<100;++i) {
		l.pop_front(v);
		status &= (v == 100-i-1);
	}

	cout << "Testing push/pop front..." << (status?"Pass":"Fail") << endl;

	for(int i=0;i<100;++i) {
		l.push(i);
	}

	l.insert(123,30);
	l.remove(v,30);
	status &= (v == 123);
	cout << "Testing insert/remove..." << (status?"Pass":"Fail") << endl;

	ListIterator<int> it=l.begin();
	int itmp = 0;
	for(;it!=l.end();++it) {
		status &= (*it == itmp++);
	}
	status &= (*it == itmp);
	cout << "Testing Iterator..." << (status?"Pass":"Fail") << endl;

	
}
