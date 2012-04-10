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
#ifndef DATAHASHER_H
#define DATAHASHER_H

#include <string.h>
#include <stdint.h>

class DataHasher {
	private:
		uint64_t *byteTable;
		uint64_t HSTART;
		uint64_t HMULT;

	public:

		DataHasher() {
			HSTART = 0xBB40E64DA205B064LLU;
			HMULT = 766435821815920749LLU;

			byteTable = new uint64_t[256];
			uint64_t h = 0x544B2FBACAAF168LLU;

			for(int i=0;i<256;++i) {
				for(int j=0;j<31;++j) {
					h = (h >> 7) ^ h;
					h = (h << 11) ^ h;
					h = (h >> 10) ^ h;
				}
				byteTable[i] = h;
			}
		}

		~DataHasher() {
			delete[] byteTable;
		}

		uint64_t hash(const char *cs) {
			uint64_t h = HSTART;
			int len = strlen(cs);

			for(int i=0;i<len;++i) {
				char ch = cs[i];
				h = (h * HMULT) ^ byteTable[ch & 0x0F];
				h = (h * HMULT) ^ byteTable[(ch >> 4) & 0x0F];
			}
			return h;
		}

};

#endif
