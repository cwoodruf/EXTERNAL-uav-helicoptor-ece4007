#include <iostream>
#include "../sensors/mbed.h"
#include "../util.h"

using namespace std;

int main() {
	IMBED imu("/dev/ttyACM0",B115200);
	SMA<int,long int> sma_x(16);
	SMA<int, long int> sma_y(16);
	SMA<int, long int> sma_z(16);

	int x=0,y=0,z=0;
	while(1) {
		imu.get_data(x,y,z);
		x = sma_x.filter(x);
		y = sma_y.filter(y);
		z = sma_z.filter(z);
		cout << (double)x/100 << ", " << (double)y/100 << ", " << (double)z/100 << endl;
	}
}
