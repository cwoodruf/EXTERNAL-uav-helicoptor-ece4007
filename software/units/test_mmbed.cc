#include <iostream>
#include "../sensors/mbed.h"
#include "../util.h"

using namespace std;

int main() {
	MMBED mbed;
	int stat = 0;
	int m1, m2, m3, m4, s;
	mbed.get_status(stat);
	cout << stat << endl;
	mbed.set_setpoints(1000,2000,500,8000);
	while(stat) {
		mbed.get_data(m1,m2,m3,m4,s);
		cout << m1 << ", " << m2 << ", " << m3 << ", " << m4 << ", " << s << endl;
	}
}
