#include <iostream>
#include "../user.h"

using namespace std;
using namespace SSL_SERVER;

void log(const char *str) {
	cout << str << endl;
}	

int main() {

	server_init();
	server_start();

	logger = &log;


	while(1) {
		if(tXbox.buttons[0]) cout << "A Button" << endl;
		if(tXbox.buttons[1]) cout << "B Button" << endl;
		if(tXbox.buttons[2]) cout << "X Button" << endl;
		if(tXbox.buttons[3]) cout << "Y Button" << endl;
		if(tXbox.buttons[4]) cout << "LB Button" << endl;
		if(tXbox.buttons[5]) cout << "RB Button" << endl;
		if(tXbox.buttons[6]) cout << "Back Button" << endl;
		if(tXbox.buttons[7]) cout << "Start Button" << endl;
		if(tXbox.buttons[8]) cout << "XBox Button" << endl;
		if(tXbox.buttons[9]) cout << "L3 Button" << endl;
		if(tXbox.buttons[10]) cout << "R3 Button" << endl;
/*
		if(SSL_SERVER::tXbox.x1) cout << "X1 Button" << endl;
		if(SSL_SERVER::tXbox.y1) cout << "Y1 Button" << endl;
		if(SSL_SERVER::tXbox.x2) cout << "X2 Button" << endl;
		if(SSL_SERVER::tXbox.y2) cout << "Y2 Button" << endl;
		if(SSL_SERVER::tXbox.lt) cout << "LT Button" << endl;
		if(SSL_SERVER::tXbox.rt) cout << "RT Button" << endl;
*/
	}
}


