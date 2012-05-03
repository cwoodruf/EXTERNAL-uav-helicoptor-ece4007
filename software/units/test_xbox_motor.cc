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

	mbed.set_setpoints(5000,5000,5000,5000);

	while(1) {
		if(tXbox.buttons[1]) {
			mbed.set_setpoints(0,0,0,0);
			break;
		} 
		m1 = (int)(5000 + (double)tXbox.y1/256 * 3000);
		mbed.set_setpoints(m1,m1,m1,m1);
	}
}


