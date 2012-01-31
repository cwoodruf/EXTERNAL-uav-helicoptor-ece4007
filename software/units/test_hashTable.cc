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
#include "../datastructures/hashTable.h"

using namespace std;

int main() {

	//Constructors
	cout << "Testing Constructors...";
	HashTable<int> cht1;
	HashTable<int> cht2(150);
	HashTable<int> *cht3 = new HashTable<int>;
	cout << "PASS" << endl;
	

	//Put
	bool tput = true;
	cout << "Testing Put...";
	tput &= (cht1.put("test1",10) == 0);
	tput &= (cht1.put("test2",10) == 0);
	tput &= (cht1.put("test3",10) == 0);
	tput &= (cht1.put("test1",20) == 1);
	tput &= (cht3->put("test1",20) == 0);
	cout << (tput ? "PASS" : "FAIL") << endl;
	

	//Get
	bool tget = true;
	int v;
	cout << "Testing Get...";
	tget &= (cht1.get("test1",v) == 0) && (v == 20);
	tget &= (cht1.get("test2",v) == 0) && (v == 10);
	tget &= (cht1.get("test3",v) == 0) && (v == 10);
	tget &= (cht1.get("test4",v) == -1);
	cout << (tget ? "PASS" : "FAIL") << endl;

	//Destructors
	cout << "Testing Destructor...";
	delete cht3;
	cout << "PASS" << endl;

	return 0;
}
