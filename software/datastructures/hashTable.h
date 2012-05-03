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
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>
#include <stdint.h>
#include "datahasher.h"

template <typename T>
class HashTable {

	private:
		DataHasher d;
		int *table;
		int *nextPtrs;
		uint64_t *hashValues;
		T *elements;
		int nextHashValuePos;
		int hashMask;
		int size;

	public:
		HashTable() {
			int sz = 128;
			int maxElements = 90;
			int desiredTableSize = maxElements * 4 / 3;
			while(sz < desiredTableSize) sz <<= 1;
			table = new int[sz];
			nextPtrs = new int[sz];
			hashValues = new uint64_t[sz];
			elements = new T[sz];
			memset(table,-1,sizeof(int)*sz);
			hashMask = sz-1;
			nextHashValuePos = 0;
			size = 0;
		}

		HashTable(int maxElements) {
			int sz = 128;
			int desiredTableSize = maxElements * 4 / 3;
			while(sz < desiredTableSize) sz <<= 1;
			table = new int[sz];
			nextPtrs = new int[sz];
			hashValues = new uint64_t[sz];
			elements = new T[sz];
			memset(table,-1,sizeof(int)*sz);
			hashMask = sz-1;
			nextHashValuePos = 0;
			size = 0;
		}  

		~HashTable() {
			delete[] table; table = NULL;
			delete[] nextPtrs; nextPtrs = NULL;
			delete[] hashValues; hashValues = NULL;
			delete[] elements; elements = NULL;
		}

		int get(const char *key, T &value);
		int put(const char *key, T value);
		int getSize() {return size;};
};



template <typename T>
int HashTable<T>::get(const char *key, T &value) {
	uint64_t hash = d.hash(key);
	int hc = (int) hash & hashMask;
	int k = table[hc];
	if(k != -1) {
		do {
			if(hashValues[k] == hash) {
				value = elements[k];
				return 0;  // Success
			}
			k = nextPtrs[k];
		} while (k != -1);
	}
	return -1;  //No Key-Value Pair
}

template <typename T>
int HashTable<T>::put(const char *key, T value) {
	uint64_t hash = d.hash(key);
	int hc = (int) hash & hashMask;
	int k = table[hc];

	if(k == -1) {
		k = nextHashValuePos++;
		table[hc] = k;
	} else {
		int lastk;

		do {
			if(hashValues[k] == hash) {
				elements[k] = value;
				return 1;  //Replaced a value
			}
			lastk = k;
			k = nextPtrs[k];
		} while(k != -1);

		k = nextHashValuePos++;
		nextPtrs[lastk] = k;
	}

	hashValues[k] = hash;
	nextPtrs[k] = -1;
	elements[k] = value;
	size++;
	return 0;  //Stored new value
}

#endif
