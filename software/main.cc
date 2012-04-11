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
#include "controls/pid.h"
#include "util.h"
#include "main.h"

using namespace std;

//Global Objects
GPIO calib_lamp;
GPIO comm_lamp;
GPIO err_lamp;
IMU imu(RATE,0.001);
Vector3 orient;
MBED mbed;
Altimeter alt;
PID alt_regulator;
PID x_regulator;
PID y_regulator;
ofstream err_file;

//Global Variables
bool FATAL = false;
eSTATE eState = eSETUP;
eSTATE ePrevState = eSETUP;
eCONTROLLER eController = eLOCAL;
short int m1,m2,m3,m4;


// Main Program - Runs Through the State Machine
int main() {
	T_TONDelay delay = {false,false,0.05,0};
	while(1) {
		switch(eState) {
			case eSETUP:
				//Run setup routines, proceed if all pass
				if(sys_setup() && io_setup() && comm_setup()) { 
					calib_lamp.set_value(1);
					comm_lamp.set_value(1);
					register_timeout(imu_loop,RATE);
					eState = eGROUND;
				} else {
					eState = eERR;
				}
				ePrevState = eSETUP;
				break;
			case eGROUND:
				//Wait for takeoff command
				switch(eController) {
					case eLOCAL:
						char cmd;
						cin >> cmd;
						if(cmd == 't') {
							cout << "TAKEOFF BEGIN" << endl;
							ePrevState = eState;
							eState = eTAKEOFF;
						}
						break;
					case eREMOTE:
						break;
					default:
						eState = ePrevState = eERR;
						error_log("SYSTEM ERROR - Unknown Controller Mode");
						fatal_err();
						break;
				}
				break;
			case eTAKEOFF:
				//Take off to a reasonable height and hover
				if(ton_delay(delay,true)) {
					if(takeoff()) {
						ePrevState = eState;
						eState = eFLY;
					}
					ton_delay(delay,false);
				}
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
				eState = ePrevState = eERR;
				error_log("SYSTEM ERROR - Unknown State Reached");
				fatal_err();
				break;
		}
	}
	return 0;
}


// Local Functions

// Sets the FATAL flag and turns on the error lamp
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

// Called on a timeout, updates orientation
void imu_loop() {
	imu.update(orient);
	register_timeout(imu_loop,RATE);
}

// Setup System
// Returns true for success, false for failed
bool sys_setup() {
	err_file.open("error.log",ios::out | ios::app);
	return !FATAL;
}

// Setup IO
// Returns true for success, false for failed
bool io_setup() {

	//Setup GPIO
	if(err_lamp.init("P9_15") != 1) {
		error_log("IO ERROR - [ERROR LAMP] GPIO Setup Failed");
		fatal_err();
	}
	err_lamp.set_dir("out"); 
	err_lamp.set_value(0);

	if(calib_lamp.init("P9_23") != 1) {
		error_log("IO ERROR - [CALIB LAMP] GPIO Setup Failed");
		fatal_err();
	} 
	calib_lamp.set_dir("out"); 
	calib_lamp.set_value(0);

	if(comm_lamp.init("P9_25") != 1) {
		error_log("IO ERROR - [COMM LAMP] GPIO Setup Failed");
		fatal_err();
	}
	comm_lamp.set_dir("out"); 
	comm_lamp.set_value(0);

	//Calibrate Sensors
	if(imu.calibrate()) {
		error_log("IO ERROR - [IMU] Calibration Failed");
		fatal_err();
	}

	T_TONDelay delay = {false,false,5.0,0};
	unsigned char status = 0;
	while(!ton_delay(delay,true)) {
		mbed.get_status(status);
		if(status == MBED_STATUS_READY) {
			break;
		}
	}

	if(status != MBED_STATUS_READY) {
		error_log("IO ERROR - [MBED] Timeout on Setup");
		fatal_err();
	}

	if(alt_regulator.init(40,50,0,50,100)) {
		error_log("IO ERROR - [ALTITUDE] PID Unstable Init Params");
		fatal_err();
	}

	if(x_regulator.init(40,50,0,50,100)) {
		error_log("IO ERROR - [ORIENTATION] PID Unstable Init Params");
		fatal_err();
	}

	if(y_regulator.init(40,50,0,50,100)) {
		error_log("IO ERROR - [ORIENTATION] PID Unstable Init Params");
		fatal_err();
	}

	return !FATAL;
}

// Setup Communications
// Returns true for success, false for failed
bool comm_setup() {
	switch(eController) {
		case eLOCAL:
			break;
		case eREMOTE:
			error_log("COMM ERROR - Connection Failed");
			fatal_err();
			break;
		default:
			error_log("SYSTEM ERROR - Unknown Controller Mode");
			fatal_err();
			break;
	}

	return !FATAL;
}

//Updates global variables with the motor speeds
void get_motors() {
	int nok = mbed.get_motor_1(m1); nok |= mbed.get_motor_2(m2);
	nok |= mbed.get_motor_3(m3); nok |= mbed.get_motor_4(m4);

	if(nok) {
		fatal_err();
		error_log("IO ERROR - [MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}
}

//Sets the motor speeds with the global variables
void set_motors() {
	int nok = mbed.set_motor_1(m1); nok |= mbed.set_motor_2(m2);
	nok |= mbed.set_motor_3(m3); nok |= mbed.set_motor_4(m4);

	if(nok) {
		fatal_err();
		error_log("IO ERROR - [MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}
}

//Automates the takeoff proceedure
bool takeoff() {	

	get_motors();
		bool ok = flight_altitude(TAKEOFF_ALTITUDE);
		flight_stabilize(Vector3(0,0,0));
	set_motors();

	return ok;
}

// Run the altitude control system
bool flight_altitude(unsigned short int desired) {
	short int inch;
	if(mbed.get_sonar(inch)) {
		fatal_err();
		error_log("IO ERROR - [MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}

	if(!inDeadBand(desired,inch,4)) {
		unsigned short int out;
		if(alt_regulator.regulate(desired,(unsigned short int)inch,out)) {
			error_log("IO ERROR - [ALTITUDE] PID Regulate Overflow");
		}
		m1 += out; m2 += out; m3 += out; m4 += out;
		return false;
	}

	return true;
}

// Run the stabilization control system
// The desired vector is what orientation we want to be at
void flight_stabilize(Vector3 desired) {
	/* We want maintain some pitch and some yaw
	*
	*  (1)    (2)
	*    \    /          +x ^
	*     ----              |
	*     |  |       +y <---O +z
	*     ----
	*    /    \
	*  (3)    (4)
	*
	*  Rotation about x-axis is roll
	*  Rotation about y-axis is pitch
	*  Rotation about z-axis is yaw
	*
	*  If x is < 0, inc motors 2 & 4, dec motors 1 & 3
	*  If x is > 0, inc motors 1 & 3, dec motors 2 & 4
	*  If y is < 0, inc motors 3 & 4, dec motors 1 & 2
	*  If y is > 0, inc motors 1 & 2, dec motors 3 & 4
	*  We are going to ignore z for now...
	*/  
	
	// Predict motor speed changes
	unsigned short int xd = (unsigned short int)(desired[0]+0.5);
	unsigned short int xo = (unsigned short int)(orient[0]+0.5);
	if(!inDeadBand(xd,xo,2)) {
		unsigned short int out;
		if(x_regulator.regulate(xd,xo,out)) {
			error_log("IO ERROR - [ORIENTATION] PID Regulate Overflow");
		}
		if(xo < xd) {
			m2 += out; m4 += out;
			m1 -= out; m3 -= out;
		} else {
			m1 += out; m3 += out;
			m2 -= out; m4 -= out;
		}
	}

	unsigned short int yd = (unsigned short int)(desired[1]+0.5);
	unsigned short int yo = (unsigned short int)(orient[1]+0.5);
	if(!inDeadBand(yd,yo,2)) {
		unsigned short int out;
		if(y_regulator.regulate(yd,yo,out)) {
			error_log("IO ERROR - [ORIENTATION] PID Regulate Overflow");
		}

		if(yo < yd) {
			m3 += out; m4 += out;
			m1 -= out; m2 -= out;
		} else {
			m1 += out; m2 += out;
			m3 -= out; m4 -= out;
		}
	}
}
