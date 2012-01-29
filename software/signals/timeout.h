#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

class Timeout {

	private:
		struct sigaction sa;
		struct itimerval timer;

	public:
		Timeout();
		Timeout(void (*fptr)(int), double t);
		Timeout(void (*fptr)(int), unsigned int t);

		void setup(void (*fptr)(int),double t);
		void setup(void (*fptr)(int),unsigned int t);
		void start();
		void stop();
		void clear();
};

Timeout::Timeout() {

}

Timeout::Timeout(void (*fptr)(int),double t) {
	setup(fptr,t);
}

Timeout::Timeout(void (*fptr)(int), unsigned int t) {
	setup(fptr,t);
}

void Timeout::setup(void (*fptr)(int),double t) {

	unsigned int sec = (unsigned int)t;
	unsigned int usec = (unsigned int)((t-sec)*1000000);

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = fptr;
	sigaction(SIGVTALRM, &sa, NULL);


	timer.it_value.tv_sec = sec;
	timer.it_value.tv_usec = usec;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;

}

void Timeout::setup(void (*fptr)(int), unsigned int t) {
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = fptr;
	sigaction(SIGVTALRM, &sa, NULL);


	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = t;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;

}

void Timeout::start() {
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void Timeout::clear() {
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}
