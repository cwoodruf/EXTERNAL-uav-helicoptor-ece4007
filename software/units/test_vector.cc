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
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "../datastructures/Vector.h"

using namespace std;

void Test1() { 

	cout << "Starting Test1" << endl;
	Vector<string> v;
	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		v.Push_Back(string(oss.str().c_str()));
	}

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string front " << i;
		v.Push_Front(string(oss.str().c_str()));
	}
	cout << "Test1 results" << endl;
	
	for (size_t i = 0; i < v.Size(); ++i) {
		cout << v[i] << endl;
	}

}

void Test2() { 

	cout << "Starting Test2" << endl;
	Vector<string> v;

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		string tmp(oss.str().c_str());
		v.Push_Back(string(oss.str().c_str()));
	}

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string front " << i;
		v.Push_Front(string(oss.str().c_str()));
	}
	cout << "Test2 results" << endl;
	
	while (!v.Empty()) {
		string st = v.Back();
		v.Pop_Back();
		cout << st << endl;
	}
}

void Test3()
{ 
	cout << "Starting Test3" << endl;
	Vector<string> v;
	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		v.Push_Back(string(oss.str().c_str()));
	}

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string front " << i;
		v.Push_Front(string(oss.str().c_str()));
	}
	cout << "Test3 results" << endl;
	
	while (!v.Empty()) {
		string st = v.Front();
		v.Pop_Front();
		cout << st << endl;
	}
}

void Test4() { 

	cout << "Starting Test4" << endl;
	Vector<string> v;

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		v.Push_Back(string(oss.str().c_str()));
	}

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string front " << i;
		v.Push_Front(string(oss.str().c_str()));
	}
	Vector<string> v1(v);
	
	cout << "Test4 results" << endl;

	for (size_t i = 0; i < v1.Size(); ++i) {
		cout << v1[i] << endl;
	}
	v.Clear();
	cout << "Test4 results again, should be Empty" << endl;

	for (size_t i = 0; i < v.Size(); ++i) {
		cout << v[i] << endl;
	}
}

void Test5() {
	cout << "Starting Test5" << endl;
	Vector<string> v;
	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		v.Push_Back(string(oss.str().c_str()));
	}

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string front " << i;
		v.Push_Front(string(oss.str().c_str()));
	}
	cout << "Test5 results" << endl;
	VectorIterator<string> it = v.Begin();

	while(it != v.End()) {
		cout << *it++ << endl;
	}
}

void Test6() { 

	cout << "Starting Test6" << endl;
	Vector<string> v;

	for (int i = 0; i < 50; ++i) {
		ostringstream oss;
		oss << "Hello from string back " << i;
		v.Push_Back(string(oss.str().c_str()));
	}

	for (size_t i = 0; i < v.Size(); ++i) {
		ostringstream oss;
		oss << "Hello from string replaced " << i;
		v[i] = string(oss.str().c_str());
	}

	cout << "Test6 results" << endl;

	for (size_t i = 0; i < v.Size(); ++i) {
		cout << v[i] << endl;
	}
}

	
int main(int argc, char** argv) { 
	int testNum = -1;
	if (argc > 1) testNum = atol(argv[1]);
	if (testNum < 0 || testNum == 1) Test1();
	if (testNum < 0 || testNum == 2) Test2();
	if (testNum < 0 || testNum == 3) Test3();
	if (testNum < 0 || testNum == 4) Test4();
	if (testNum < 0 || testNum == 5) Test5();
	if (testNum < 0 || testNum == 6) Test6();
	return 0;
}
