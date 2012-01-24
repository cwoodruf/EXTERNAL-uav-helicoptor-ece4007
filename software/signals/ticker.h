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

		void attach(void (*fptr)(int), float t);
		void attach_us(void (*fptr)(int), unsigned int t);
		void detach();
};

Ticker::Ticker() {
}

void Ticker::attach(void (*fptr)(int), float t) {

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
