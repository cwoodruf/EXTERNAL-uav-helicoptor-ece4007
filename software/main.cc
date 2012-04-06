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

#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include "events/timeout.h"
#include "events/pubsub.h"
#include "imu.h"
#include "io/gpio.h"
#include "util.h"

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
ofstream err_file;

//Variables
bool CALIB = false;
bool COMM = false;
bool FATAL = false;
bool GROUNDED = true;

void fatal_err() {
	err_lamp.set_value(1);
	FATAL = true;
}

void error_log(void *data) {

	char buf[256];
	time_t t;
	t = time(NULL);
	sprintf(buf,"%s",asctime(localtime(&t)));
	buf[24] = '\0';
	err_file << buf << " - " << ((string *)data)->c_str() << endl;
}

void sys_setup() {

	//Logging
	err_file.open("error.log",ios::out | ios::app);

	//Subscriptions
	subscribe("system/error",error_log);
}

void io_setup() {

	//Setup GPIO
	if(err_lamp.init("P9_15")) {
		eState = eERR;
		string msg; msg = "IO ERROR - GPIO Setup Failed [ERROR LAMP]";
		publish("system/error",(void *)&msg);
	}
	err_lamp.set_dir("out"); 
	err_lamp.set_value(0);

	//TODO: Assign GPIO Pins
	if(calib_lamp.init("P9_??")) {
		eState = eERR;
		string msg; msg = "IO ERROR - GPIO Setup Failed [CALIB LAMP]";
		publish("system/error",(void *)&msg);
	} 
	calib_lamp.set_dir("out"); 
	calib_lamp.set_value(0);

	if(comm_lamp.init("P9_??")) {
		eState = eERR;
		string msg; msg = "IO ERROR - GPIO Setup Failed [COMM LAMP]";
		publish("system/error",(void *)&msg);
	}
	comm_lamp.set_dir("out"); 
	comm_lamp.set_value(0);

	//Calibrate Sensors
	if(imu.calibrate()) {
		eState = eERR;
		string msg; msg = "IO ERROR - IMU Calibration Failed";
		publish("system/error",(void *)&msg);
		fatal_error();
	}

	//GET MBED IS READY
}

void comm_setup() {

	//IF NO WORK
	//fatal_error();
}

int main() {

	while(1) {
		switch(eState) {
			case eSETUP:
				sys_setup();
				io_setup();
				if(imu.isCalibrated()) {
					CALIB = true;
					calib_lamp.set_value(1);
				}
				comm_setup();
				comm_lamp.set_value(1);

				ePrevState = eSETUP;
				if(!FATAL) {
					eState = eLAND;
				} else {
					eState = eERR;
				}
				break;
			case eLAND:
				break;
			case eFLY:
				break;
			case eERR:
				switch(ePrevState) {
					case eSETUP:
						//STAY HERE UNTIL RESET?
						break;
					case eLAND:
						//STAY HERE UNTIL RESET?
						break;
					case eFLY:
						//SAFE LAND AND WAIT?
						break;
					default:
						//WAIT?
						break;
				}
				break;
			default:
				eState = eERR;
				ePrevState = eERR;
				string msg; msg = "SYSTEM ERROR - Unknown State Reached";
				publish("system/error",(void *)&msg);
				break;
		}

	}

	return 0;
}
