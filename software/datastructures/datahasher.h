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
			HSTART = 0xBB40E64DA205B064;
			HMULT = 766435821815920749;

			byteTable = new uint64_t[256];
			uint64_t h = 0x544B2FBaCAAF168;

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
