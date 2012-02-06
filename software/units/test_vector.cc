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
