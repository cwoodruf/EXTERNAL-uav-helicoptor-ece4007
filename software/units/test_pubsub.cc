#include "../events/pubsub.h"
#include <iostream>

using namespace std;

void gogogo(void *data) {
	cout << "hello" << endl;
}

void gogogo2(void *data) {
	cout << "hello2: " << (int)data << endl;
}

int main() {

	cout << "Subscribing func0 to \'test/topic\': " << subscribe("test/topic",gogogo) << endl;
	cout << "Subscribing func1 to \'test/topic\': " << subscribe("test/topic",gogogo2) << endl;
	cout << "Publishing to \'test/topic\'" << endl;
	publish("test/topic",NULL);
	cout << "Unsubscribing func0 from \'test/topic\'" << endl;
	unsubscribe(0);
	cout << "Publishing to \'test/topic\'" << endl;
	publish("test/topic",(void *)123);

	return 0;
}
