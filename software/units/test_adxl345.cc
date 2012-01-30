#include "../sensors/ADXL345.h"
#include "../time/ticker.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

ADXL345 accel;


void print_data(int sig) {
	short int x=0, y=0, z=0;
    accel.get_data_x_raw(x);
    accel.get_data_y_raw(y); 
    accel.get_data_z_raw(z);
    cout << "X: " << x << "\t"
         << "Y: " << y << "\t"
         << "Z: " << z << "\t"
         << endl;
}

int main() {
	Ticker t;
	t.attach(print_data,0.5);

    //Verify connection to chip and bus by checking device id
    unsigned char id = 0;
    if(accel.get_dev_id(id) != 0) {
        cout << "Device not verified" << endl;
        exit(-1);
    } else {
        cout << "Device ID: " << oct << (int)id << endl;
    }

    //Set the device to measure in full resolution
    accel.set_full_res();

    //Set the device in measurement mode
    accel.set_measurement_mode();

    while(1) {;}

    return 0;

}
