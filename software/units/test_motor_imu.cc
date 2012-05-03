//Includes
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "../sensors/mbed.h"
#include "../controls/pid.h"
#include "../util.h"

//Defines
#define ORIENT_DEADBAND 	300
#define ALTITUDE_DEADBAND	3

//Globals
MMBED mmbed;
IMBED imbed;
PID x_regulator;
PID y_regulator;
PID alt_regulator;
SMA<int,int> smax(16), smay(16), smaz(16), ss(16);
int orient[3] = {0};
int target_orient[3] = {0};//{-500,300,0};
int m1=0,m2=0,m3=0,m4=0,sonar=0;

//Prototypes
void get_orient();
void get_motors();
void set_motors();
bool flight_altitude(unsigned short int desired);
void flight_stabilize(int *desired);
void catchit(int);
void killed(int a, void *v);

int main() {

	int istat=0, mstat=0;
	T_TONDelay setpt_delay = {false,false,15.0,{0}};
	T_TONDelay reg_delay = {false,false,0.025,{0}};
	srand(time(NULL));
	imbed.get_status(istat);
	mmbed.get_status(mstat);
	cout << istat + mstat << endl;

	cout << x_regulator.init(50,0,0,50,5) << endl;
	cout << y_regulator.init(50,0,0,50,5) << endl;
	cout << alt_regulator.init(40,10,0,50,15) << endl;

	signal(SIGINT,&catchit);
	on_exit(&killed,NULL);

	//int setpt = rand() % 5500 + 2500;
	int setpt = 6500;
	m1 = m2 = m3 = m4 = setpt;
	set_motors();
	while(istat && mstat) {

		/*if(ton_delay(setpt_delay,true)) {
			setpt = rand() % 5500 + 2500;
			m1 = m2 = m3 = m4 = setpt;
			set_motors();
			imbed.get_status(istat);
			mmbed.get_status(mstat);
			ton_delay(setpt_delay,false);
		}*/

		if(ton_delay(reg_delay,true)) {
			get_orient();
			get_motors();
				flight_stabilize(target_orient);
				//flight_altitude(1000);
				cout << sonar << endl;
			set_motors();
			ton_delay(reg_delay,false);
		}
	}

	return 0;
}

void killed(int a, void *v) {

	while(m1 != 0 || m2 != 0 || m3 != 0 || m4 != 0) {
		m1 = m2 = m3 = m4 = 0;
		set_motors();
		usleep(100000);
		get_motors();
	}
}

void catchit(int) {
	cout << "STOPPING" << endl;
	exit(-1);
}

//Updates orientation vector
void get_orient() {
	imbed.get_data(orient[0],orient[1],orient[2]);
	orient[0] = smax.filter(orient[0]);
	orient[1] = smay.filter(orient[1]);
	orient[2] = smaz.filter(orient[2]);
}

//Updates global variables with the motor speeds
void get_motors() {
	mmbed.get_data(m1,m2,m3,m4,sonar);
	//`sonar = ss.filter(sonar);
}

void set_motors() {
	mmbed.set_setpoints(m1,m2,m3,m4);
}

// Run the altitude control system
bool flight_altitude(unsigned short int desired) {
	if(sonar > 40) sonar = 0;
	if(!inDeadBand(desired,sonar,ALTITUDE_DEADBAND)) {
		unsigned short int out;
		if(sonar < desired) {
			alt_regulator.regulate(desired,sonar,out);
			m1 += out; m2 += out; m3 += out; m4 += out;
		} else {
			alt_regulator.regulate(sonar,desired,out);
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
		if(xo < xd) {
			x_regulator.regulate(xd,xo,out);
			m1 = LIMIT(2500,m1+out,8000); m2 = LIMIT(2500,m2+out,8000);
			m3 = LIMIT(2500,m3-out,8000); m4 = LIMIT(2500,m4-out,8000);
		} else {
			x_regulator.regulate(xo,xd,out);
			m3 = LIMIT(2500,m3+out,8000); m4 = LIMIT(2500,m4+out,8000);
			m1 = LIMIT(2500,m1-out,8000); m2 = LIMIT(2500,m2-out,8000);
		}
	}

	
	unsigned short int yd = (unsigned short int)(desired[1] + 9000);
	unsigned short int yo = (unsigned short int)(orient[1] + 9000);
	if(!inDeadBand(yd,yo,ORIENT_DEADBAND)) {
		unsigned short int out;
		if(yo < yd) {
			y_regulator.regulate(yd,yo,out);
			m1 = LIMIT(2500,m1+out*2,8000); m4 = LIMIT(2500,m4+out*2,8000);
			m2 = LIMIT(2500,m2-out,8000); m3 = LIMIT(2500,m3-out,8000);
		} else {
			y_regulator.regulate(yo,yd,out);
			m2 = LIMIT(2500,m2+out*2,8000); m3 = LIMIT(2500,m3+out*2,8000);
			m1 = LIMIT(2500,m1-out,8000); m4 = LIMIT(2500,m4-out,8000);
		}
	}
	
}

