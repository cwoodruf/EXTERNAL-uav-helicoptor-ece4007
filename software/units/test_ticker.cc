#include "../time/ticker.h"
#include <iostream>

using namespace std;

Ticker t;

void run_on_timeout(int value) {
    static int count = 1;
    if(count == 10) t.detach();
    cout << "Timer has run " << count++ << " times" << endl;
}

int main() {
    //Setup a timer to run every 1s
    t.attach(run_on_timeout,1);

    //Busy work
    while(1) {}

    return 0;
}
