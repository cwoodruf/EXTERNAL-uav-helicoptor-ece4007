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
#include "../sensors/ITG3200.h"
#include "../events/timeout.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

ITG3200 gyro;


void print_data() {
	short int x=0, y=0, z=0;
    gyro.get_x_out(x);
    gyro.get_y_out(y); 
    gyro.get_z_out(z);
    cout << "X: " << x << "\t"
         << "Y: " << y << "\t"
         << "Z: " << z << "\t"
         << endl;

	register_timeout(print_data,0.05);
}

int main() {

    //Verify connection to chip and bus by checking device id
    unsigned char id = 0;
    if(gyro.get_whoami(id) != 0) {
        cout << "Device not verified" << endl;
        exit(-1);
    } else {
        cout << "Whoami?: " << (int)id << endl;
    }

	register_timeout(print_data,0.05);

    while(1) {;}

    return 0;

}
