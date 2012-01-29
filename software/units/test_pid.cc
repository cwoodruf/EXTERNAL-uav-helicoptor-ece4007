#include "../controls/pid.h"
#include "../signals/ticker.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

PID pid;

void control_loop(int sig) {

	static unsigned short int value = 0;
	unsigned short int out = 0;

	if(!inDeadBand(1234,value,2)) {
		if(pid.regulate(1234,value,out) != 0) {
			cout << "ERROR" << endl;
		}

		if(value > 1234) value -= out;
		else value += out;

		cout << "out: " << out << "\t";
		cout << "newVal: " << value << endl;
	} else {
		cout << "In Deadband at: " << value << endl;
	}


}


int main(int argc, char **argv) {
	Ticker t;

	if(pid.init(40,50,0,50,100) < 0) {
		cout << "Warning Unstable PID Parameters" << endl;
		return -1;
	}

	t.attach(control_loop,0.050);
	
	while(1) {;}
	return 0;
}
