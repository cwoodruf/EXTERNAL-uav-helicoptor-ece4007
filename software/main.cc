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

#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include "imu.h"
#include "sensors/mbed.h"
#include "sensors/altimeter.h"
#include "io/gpio.h"
#include "events/timeout.h"
#include "util.h"

using namespace std;

#define RATE 0.0012

typedef enum _eSTATE {
	eSETUP,
	eGROUND,
	eTAKEOFF,
	eFLY,
	eLAND,
	eERR
}eSTATE;

eSTATE eState = eSETUP;
eSTATE ePrevState = eSETUP;

//Objects
GPIO calib_lamp;
GPIO comm_lamp;
GPIO err_lamp;
IMU imu(RATE,0.001);
MBED mbed;
Altimeter alt;
ofstream err_file;

//Variables
bool MBED_OK = false;
bool MBED_TO = false;
bool CALIB = false;
bool COMM = false;
bool FATAL = false;

void fatal_err() {
	err_lamp.set_value(1);
	FATAL = true;
}

// Writes a message out to the error log with a timestamp
// DayW Month DayN Time Year - MSG
// Tue Apr 10 08:19:55 2012 - SOME ERROR - Error Details
void error_log(const char *data) {
	char buf[256];
	time_t t;
	t = time(NULL);
	sprintf(buf,"%s",asctime(localtime(&t)));
	buf[24] = '\0';
	err_file << buf << " - " << data << endl;
}

// Called on a timeout, sets the MBED_TO flag
void mbed_to() {
	MBED_TO = true;
}

// Setup System
// Returns true for success, false for failed
bool sys_setup() {

	//Logging
	err_file.open("error.log",ios::out | ios::app);

	return !FATAL;
}

// Setup IO
// Returns true for success, false for failed
bool io_setup() {

	//Setup GPIO
	if(err_lamp.init("P9_15")) {
		error_log("IO ERROR - GPIO Setup Failed [ERROR LAMP]");
		fatal_err();
	}
	err_lamp.set_dir("out"); 
	err_lamp.set_value(0);

	if(calib_lamp.init("P9_23")) {
		error_log("IO ERROR - GPIO Setup Failed [CALIB LAMP]");
		fatal_err();
	} 
	calib_lamp.set_dir("out"); 
	calib_lamp.set_value(0);

	if(comm_lamp.init("P9_25")) {
		error_log("IO ERROR - GPIO Setup Failed [COMM LAMP]");
		fatal_err();
	}
	comm_lamp.set_dir("out"); 
	comm_lamp.set_value(0);

	//Calibrate Sensors
	if(imu.calibrate()) {
		error_log("IO ERROR - IMU Calibration Failed");
		fatal_err();
	}

	unsigned char status = 0;
	register_timeout(mbed_to,5.0);
	while(!MBED_OK && !MBED_TO) {
		mbed.get_status(status);
		if(status == MBED_STATUS_READY) {
			MBED_OK = true;
		}
	}

	if(!MBED_OK) {
		error_log("IO ERROR - MBED Timeout on Setup");
		fatal_err();
	}

	return !FATAL;
}

// Setup Communications
// Returns true for success, false for failed
bool comm_setup() {
	error_log("COMM ERROR - Connection Failed");
	fatal_err();
	return !FATAL;
}

int main() {
	while(1) {
		switch(eState) {
			case eSETUP:
				ePrevState = eSETUP;	

				//Run setup routines, proceed if all pass
				if(sys_setup() && io_setup() && comm_setup()) { 
					calib_lamp.set_value(1);
					comm_lamp.set_value(1);
					eState = eGROUND;
				} else {
					eState = eERR;
				}
				break;
			case eGROUND:
				//Wait for takeoff command
				break;
			case eTAKEOFF:
				//Take off to a reasonable height and hover
				break;
			case eFLY:
				//Hover while waiting for user command and react
				break;
			case eLAND:
				//Hover down slowly and land
				break;
			case eERR:
				switch(ePrevState) {
					case eSETUP:
						//Just sit here and make sure io is off
						break;
					case eGROUND:
						//Just sit here and make sure io is off
						break;
					case eTAKEOFF:
						//SAFE LAND AND WAIT?
						break;
					case eFLY:
						//SAFE LAND AND WAIT?
						break;
					case eLAND:
						//STAY HERE UNTIL RESET?
						break;
					case eERR:
						//Just sit here and make sure io is off
						break;
					default:
						//Just sit here and make sure io is off
						break;
				}
				break;
			default:
				eState = eERR;
				ePrevState = eERR;
				error_log("SYSTEM ERROR - Unknown State Reached");
				fatal_err();
				break;
		}

	}

	return 0;
}
