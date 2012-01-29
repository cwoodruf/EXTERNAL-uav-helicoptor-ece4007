#include <iostream>
#include "../signals/timeout.h"

using namespace std;

void run_on_timeout(int value) {
    cout << "Timeout occured" << endl;
}

int main() {
    //Setup a timer to run every 1s
    Timeout t(run_on_timeout,1.0);

    //Start the timer
    t.start();

    //Busy work
    while(1) {}

    return 0;
}
