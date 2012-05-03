#include <iostream>
#include "../sensors/mbed.h"
#include "../util.h"

using namespace std;

int main() {
	IMBED imu;
	int x=0,y=0,z=0,stat=0;
	imu.get_status(stat);
	cout << stat << endl;
	while(stat) {
		imu.get_data(x,y,z);
		cout << (double)x/100 << ", " << (double)y/100 << ", " << (double)z/100 << endl;
	}
}
