#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "../user.h"
#include "../sensors/mbed.h"

using namespace std;
using namespace SSL_SERVER;

MMBED mbed;

void log(const char *str) {
	cout << str << endl;
}	

void killed(int a, void *v) {
	mbed.set_setpoints(0,0,0,0);
}

void catchit(int) {
	cout << "STOPPING" << endl;
	exit(-1);
}

int main() {

	int m1 = 0;

	signal(SIGINT,&catchit);
	on_exit(&killed,NULL);

	server_init();
	server_start();

	logger = &log;

	while(!server_isConnected()) {;}

	mbed.set_setpoints(2500,2500,2500,2500);

	while(1) {
		if(tXbox.buttons[0]) {
			mbed.set_setpoints(8000,8000,8000,8000);
		}
		if(tXbox.buttons[1]) {
			mbed.set_setpoints(0,0,0,0);
		}
	}
}


