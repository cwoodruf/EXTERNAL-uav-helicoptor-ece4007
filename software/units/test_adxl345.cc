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
#include "../sensors/ADXL345.h"
#include "../events/timeout.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

ADXL345 accel;


void print_data() {
	short int x=0, y=0, z=0;
    accel.get_data_x_raw(x);
    accel.get_data_y_raw(y); 
    accel.get_data_z_raw(z);
    cout << "X: " << x << "\t"
         << "Y: " << y << "\t"
         << "Z: " << z << "\t"
         << endl;

	register_timeout(print_data,0.05);
}

int main() {

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

	register_timeout(print_data,0.05);

    while(1) {;}

    return 0;

}
