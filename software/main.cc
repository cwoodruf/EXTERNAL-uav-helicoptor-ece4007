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

#include "events/timeout.h"
#include "events/pubsub.h"
#include "imu.h"
#include "io/gpio.h"
#include "util.h"
#include <string>
#include <time.h>

#define RATE 0.0012

typedef enum _eSTATE {

	eSETUP,
	eLAND,
	eFLY,
	eERR
}eSTATE;

eSTATE eState = eSETUP;
eSTATE ePrevState = eSETUP;

//Objects
GPIO calib_lamp;
GPIO comm_lamp;
GPIO err_lamp;
IMU imu(RATE,0.001);


void fatal_err() {
	err_lamp.set_value(1);
}

void error_log(void *data) {
	//TODO: Format a TIMESTAMP
	cout << ((string *)data)->c_str() << endl;
	//TODO: Format the message
	//TODO: Write the message to a log
}

void io_setup() {
	//Setup GPIO
	int status = err_lamp.init("P9_15"); 
	status |= err_lamp.set_dir("out"); 
	status |= err_lamp.set_value(0);

	//TODO: Assign GPIO Pins
	calib_lamp.init("P9_??"); calib_lamp.set_dir("out"); calib_lamp.set_value(0);
	comm_lamp.init("P9_??"); comm_lamp.set_dir("out"); comm_lamp.set_value(0);

	if(status) {
		eState = eERR;
		string msg; msg = "IO ERROR - GPIO Setup Failed.";
		publish("system/error",(void *)&msg);
	}

	//Calibrate Sensors
	if(imu.calibrate()) {
		eState = eERR;
		string msg; msg = "IO ERROR - Calibration Failed.";
		publish("system/error",(void *)&msg);
	}

}

void sys_setup() {
	//Subscriptions
	subscribe("system/error",error_log);
}

void comm_setup() {
}

int main() {

	while(1) {

		switch(eState) {

			case eSETUP:
				sys_setup();
				io_setup();
				comm_setup();
				break;
			case eLAND:
				break;
			case eFLY:
				break;
			case eERR:
				break;
			default:
				eState = eERR;
				ePrevState = eERR;
				string msg; msg = "SYSTEM ERROR - Unknown State Reached.";
				publish("system/error",(void *)&msg);
				break;
		}

	}

	return 0;
}
