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
#include <pthread.h>
#include "imu.h"
#include "sensors/mbed.h"
#include "sensors/altimeter.h"
#include "io/gpio.h"
#include "events/timeout.h"
#include "controls/pid.h"
#include "util.h"
#include "main.h"
#include "user.h"

using namespace std;

//Global Objects
GPIO calib_lamp;
GPIO comm_lamp;
GPIO err_lamp;
int orient[3];
int target_orient[3];
int zero_orient[3] = {0,0,0};
MMBED mmbed;
IMBED imbed;
Altimeter alt;
PID alt_regulator;
PID x_regulator;
PID y_regulator;
User user(0,0);
ofstream err_file;

//Global Variables
bool FATAL = false;
eSTATE eState = eSETUP;
eSTATE ePrevState = eSETUP;
int m1,m2,m3,m4,sonar;
unsigned short int target_alt = TAKEOFF_ALTITUDE;
short int altitude;


// Main Program - Runs Through the State Machine
int main() {
	T_TONDelay delay = {false,false,REGULATION_RATE,{0}};
	while(1) {
		switch(eState) {
			case eSETUP:
				//Run setup routines, proceed if all pass
				if(sys_setup() && /*io_setup() &&*/ comm_setup()) { 
					calib_lamp.set_value(1);
					comm_lamp.set_value(1);
					eState = eGROUND;
				} else {
					eState = eERR;
				}
				ePrevState = eSETUP;
				break;
			case eGROUND:
				//Wait for takeoff command (A Button)
				if(SSL_SERVER::tXbox.buttons[0]) {
					cout << "TAKEOFF!!!" << endl;
					ePrevState = eState;
					eState = eTAKEOFF;
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
				//TODO: Update orient and alt with controller input
				if(ton_delay(delay,true)) {
					target_orient[0] = LIMIT(MIN_ORIENT,target_orient[0],MAX_ORIENT);
					target_orient[1] = LIMIT(MIN_ORIENT,target_orient[1],MAX_ORIENT);
					target_alt = LIMIT(MIN_ALTITUDE,target_alt,MAX_ALTITUDE);
					get_orient();
					get_motors();
					safety_checks();
						flight_altitude(target_alt);
						flight_stabilize(target_orient);
					set_motors();
					ton_delay(delay,false);
				}
				break;
			case eLAND:
				//Hover down slowly and land
				if(ton_delay(delay,true)) {
					if(landing()) {
						ePrevState = eState;
						eState = eGROUND;
					}
					ton_delay(delay,false);
				}
				break;
			case eERR:
				switch(ePrevState) {
					case eSETUP:
						exit(-1);
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


// Sets the FATAL flag and turns on the error lamp
void fatal_err() {
	err_lamp.set_value(1);
	FATAL = true;
}

// Writes a message out to the error log with a timestamp
// DayW Month DayN Time Year - MSG
// Tue Apr 10 08:19:55 2012 - SOME ERROR - Error Details
void error_log(const char *data) {
	pthread_mutex_lock(&SSL_SERVER::data_mutex);
		char buf[256];
		time_t t;
		t = time(NULL);
		sprintf(buf,"%s",asctime(localtime(&t)));
		buf[24] = '\0';
		err_file << buf << " - " << data << endl;
	pthread_mutex_unlock(&SSL_SERVER::data_mutex);
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

	T_TONDelay delay = {false,false,5.0,{0}};
	int mstat = 0;
	int istat = 0;
	while(!ton_delay(delay,true)) {
		if(!mstat) mmbed.get_status(mstat);
		if(!istat) imbed.get_status(istat);
		if(mstat && istat) break;
	}
	
	if(!mstat) {
		fatal_err();
		error_log("IO ERROR - [MOTOR MBED] Setup Failed");
	}

	if(!istat) {
		fatal_err();
		error_log("IO ERROR - [IMU MBED] Setup Failed");
	}

	return !FATAL;
}

// Setup Communications
// Returns true for success, false for failed
bool comm_setup() {

	//Initialize the server
	SSL_SERVER::server_init();

	//Setup logging and error tripping
	SSL_SERVER::logger = &error_log;
	SSL_SERVER::error = &fatal_err;

	//Start the server
	SSL_SERVER::server_start();
	T_TONDelay timeout = {false,false,300.0,{0}}; //5 Minutes
	while(!SSL_SERVER::server_isConnected()) {
		//TODO:WHAT TO DO WHILE WE WAIT?
		if(ton_delay(timeout,true)) {
			error_log("COMM ERROR - [SSL SERVER] Connection Timeout");
			fatal_err();
			break;
		}
	}

	return !FATAL;
}

//Updates orientation vector
void get_orient() {
	if(imbed.get_data(orient[0],orient[1],orient[2]) < 0) {
		fatal_err();
		error_log("IO ERROR - [IMU MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}
}

//Updates global variables with the motor speeds
void get_motors() {
	if(mmbed.get_data(m1,m2,m3,m4,sonar)) {
		fatal_err();
		error_log("IO ERROR - [MOTOR MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}
}

//Sets the motor speeds with the global variables
void set_motors() {
	if(mmbed.set_setpoints(m1,m2,m3,m4)) {
		fatal_err();
		error_log("IO ERROR - [MOTOR MBED] Lost Communications");
		ePrevState = eState;
		eState = eERR;
	}
}

//Automates the takeoff proceedure
bool takeoff() {	
	get_orient();
	safety_checks();
	get_motors();
		bool ok = flight_altitude(target_alt);
		flight_stabilize(zero_orient);
	set_motors();

	return ok;
}

//Automates the landing proceedure
bool landing() {	
	get_orient();
	safety_checks();
	get_motors();
		bool ok = flight_altitude(0);
		flight_stabilize(zero_orient);
	set_motors();

	return ok;
}

// Run the altitude control system
bool flight_altitude(unsigned short int desired) {
	if(!inDeadBand(desired,altitude,ALTITUDE_DEADBAND)) {
		unsigned short int out;
		if(alt_regulator.regulate(desired,altitude,out)) {
			error_log("IO ERROR - [ALTITUDE] PID Regulate Overflow");
		}
		if(altitude < desired) {
			m1 += out; m2 += out; m3 += out; m4 += out;
		} else {
			m1 -= out; m2 -= out; m3 -= out; m4 -= out;
		}
		return false;
	}

	return true;
}

// Run the stabilization control system
// The desired vector is what orientation we want to be at
void flight_stabilize(int *desired) {
	/* We want maintain some pitch and some yaw
	*
	*  (1)    (2)
	*    \    /          +x ^
	*     ----              |
	*     |  |       +y <---O +z
	*     ----
	*    /    \
	*  (4)    (3)
	*
	*  Rotation about x-axis is roll
	*  Rotation about y-axis is pitch
	*  Rotation about z-axis is yaw
	*
	*  If x is < 0, inc motors 2 & 3, dec motors 1 & 4
	*  If x is > 0, inc motors 1 & 4, dec motors 2 & 3
	*  If y is < 0, inc motors 3 & 4, dec motors 1 & 2
	*  If y is > 0, inc motors 1 & 2, dec motors 3 & 4
	*  We are going to ignore z for now...but it will mod opposite pairs
	*/  
	
	// Predict motor speed changes
	unsigned short int xd = (unsigned short int)(desired[0] + 9000);
	unsigned short int xo = (unsigned short int)(orient[0] + 9000);
	if(!inDeadBand(xd,xo,ORIENT_DEADBAND)) {
		unsigned short int out;
		if(x_regulator.regulate(xd,xo,out)) {
			error_log("IO ERROR - [ORIENTATION] PID Regulate Overflow");
		}
		if(xo < xd) {
			m2 += out; m3 += out;
			m1 -= out; m4 -= out;
		} else {
			m1 += out; m4 += out;
			m2 -= out; m3 -= out;
		}
	}

	unsigned short int yd = (unsigned short int)(desired[1] + 9000);
	unsigned short int yo = (unsigned short int)(orient[1] + 9000);
	if(!inDeadBand(yd,yo,ORIENT_DEADBAND)) {
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

void safety_checks() {
	//Check Connection
	if(!SSL_SERVER::server_isConnected()) {
		//Should already be logged and in fatal error mode
		ePrevState = eState;
		eState = eERR;
	}

	//Check Roll & Pitch
	if(orient[0] < MIN_CRIT_ANGLE || orient[0] > MAX_CRIT_ANGLE ||
	   orient[1] < MIN_CRIT_ANGLE || orient[1] > MAX_CRIT_ANGLE) {
		error_log("IO ERROR - [ORIENTATION] CRITICAL ANGLE REACHED");
		fatal_err();
		//WHAT DO WE DO????
	}
}
