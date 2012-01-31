/*
Copyright (c) 2012 Joey Yore

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

class Ticker {

	private:
		struct sigaction sa;
		struct itimerval timer;

	public:
		Ticker();

		void attach(void (*fptr)(int), double t);
		void attach_us(void (*fptr)(int), unsigned int t);
		void detach();
};

Ticker::Ticker() {
}

void Ticker::attach(void (*fptr)(int), double t) {

	unsigned int sec = (unsigned int)t;
	unsigned int usec = (unsigned int)((t-sec)*1000000);

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = fptr;
	sigaction(SIGVTALRM, &sa, NULL);


	timer.it_value.tv_sec = sec;
	timer.it_value.tv_usec = usec;
	timer.it_interval.tv_sec = sec;
	timer.it_interval.tv_usec = usec;

	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void Ticker::attach_us(void (*fptr)(int), unsigned int t) {
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = fptr;
	sigaction(SIGVTALRM, &sa, NULL);


	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = t;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = t;

	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void Ticker::detach() {
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;

	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}
