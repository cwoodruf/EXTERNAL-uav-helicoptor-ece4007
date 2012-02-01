#include "../io/gpio.h"
#include "../time/timeout.h"
#include <iostream>

using namespace std;

GPIO g;
bool go = true;


void catchit(int) {
	go = false;
	g.detach();
}

void blink() {
	static int value = 0;
	value = !value;
	g.set_value(value);
}

int main(int argc, char **argv) {

	if(argc != 2) {
		cout << "usage: blink [period]" << endl;
		return -1;
	}

	g.init("P9_15");
	g.set_dir("out");

	signal(SIGINT,catchit);

	register_timeout(blink,atof(argv[1]),true);

	cout << "Blinking at " << (1/atof(argv[1])) << " Hz - 50\% Duty Cycle" << endl; 

	while(go) {
		;
	}

	return 0;
}
