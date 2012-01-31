/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TIMEOUT_H
#define TIMEOUT_H


#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include "../datastructures/list.h"

using namespace std;

int register_timeout(void (*func)(),double time,bool reschedule);

namespace timeout {

	struct TimeoutNode {
		void (*cb)();
		double diff;
		double time;
		bool reschedule;
	};

	List<TimeoutNode> toList;
	struct itimerval itimer;
	double currentTime;

	void dispatcher(int sig) {
		TimeoutNode to;
		toList.pop_front(to);
		to.cb();


		if(!toList.isEmpty()) {
			ListIterator<TimeoutNode> it=toList.begin();

			while((*it).diff == 0) {
				toList.pop_front(to);
				to.cb();
				it=toList.begin();
			}
		
			currentTime = (*it).diff;

			for(;it!=toList.end();++it) {
				(*it).diff = (*it).diff - currentTime;
			}
			(*it).diff = (*it).diff - currentTime;

			unsigned int s = (unsigned int)currentTime;
			unsigned int us = (unsigned int)((currentTime-s)*1000000);
			itimer.it_value.tv_sec = s;
			itimer.it_value.tv_usec = us;
			itimer.it_interval.tv_sec = 0;
			itimer.it_interval.tv_usec = 0;
			setitimer(ITIMER_REAL,&timeout::itimer,0);
			signal(SIGALRM, dispatcher);


		}
		if(to.reschedule) register_timeout(to.cb,to.time,true);
	}
}


int register_timeout(void (*func)(),double time,bool reschedule=false) {

	timeout::TimeoutNode to;

	if(timeout::toList.isEmpty()) {
		to.cb = func;
		to.diff = 0;
		to.reschedule = reschedule;
		to.time = time;
		timeout::toList.push(to);
		timeout::currentTime = time;

		unsigned int s = (unsigned int)time;
		unsigned int us = (unsigned int)((time-s)*1000000);
		timeout::itimer.it_value.tv_sec = s;
		timeout::itimer.it_value.tv_usec = us;
		timeout::itimer.it_interval.tv_sec = 0;
		timeout::itimer.it_interval.tv_usec = 0;

		setitimer(ITIMER_REAL,&timeout::itimer,0);
		signal(SIGALRM, timeout::dispatcher);
		return 0;
	}

	ListIterator<timeout::TimeoutNode> it=timeout::toList.begin();
	
	//is front?
	if(time < ((*it).diff+timeout::currentTime)) {
		for(;it!=timeout::toList.end();++it) {
			(*it).diff = (*it).diff + timeout::currentTime - time;
		}
		(*it).diff = (*it).diff + timeout::currentTime - time;

		to.diff = 0;
		to.cb = func;
		timeout::toList.push_front(to);
		timeout::currentTime = time;

		unsigned int s = (unsigned int)time;
		unsigned int us = (unsigned int)((time-s)*1000000);
		timeout::itimer.it_value.tv_sec = s;
		timeout::itimer.it_value.tv_usec = us;
		timeout::itimer.it_interval.tv_sec = 0;
		timeout::itimer.it_interval.tv_usec = 0;

		setitimer(ITIMER_REAL,&timeout::itimer,0);
		signal(SIGALRM, timeout::dispatcher);


	} else {
		int place = 0;
		bool found = false;
		to.diff = time - timeout::currentTime;
		to.cb = func;

		for(;it!=timeout::toList.end();++it) {
			if(time <= (*it).diff + timeout::currentTime) {
				found = true;
				break;
			}
			++place;
		}
		if(!found) {
			if(time > (*it).diff + timeout::currentTime) {
				timeout::toList.push(to);	
			} else {
				timeout::toList.insert(to,place);
			}
		} else {
			timeout::toList.insert(to,place);
		}
	}

	return 0;
}
#endif
