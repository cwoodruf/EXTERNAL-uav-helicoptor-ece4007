
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include "../sensors/mbed.h"
#include "../util.h"

using namespace std;

int main() {
	cout.flush();
	int stat = 0;
	int m1 = 0,m2 = 0,m3 = 0,m4 = 0,s=0;
	T_TONDelay delay = {false,false,10.0,{0}};

	srand(time(NULL));
	MMBED mbed;
	mbed.get_status(stat);
	while(stat) {
		// Change setpoints every 10s
		if(ton_delay(delay,true)) {
			int setpt = rand() % 5500 + 2500;
			cout << setpt << " : ";
			mbed.set_setpoints(setpt,setpt,setpt,setpt);	
			usleep(10000);
			mbed.get_data(m1,m2,m3,m4,s);
			cout << s << endl;
			ton_delay(delay,false);
		}
	}
	return 0;
}
