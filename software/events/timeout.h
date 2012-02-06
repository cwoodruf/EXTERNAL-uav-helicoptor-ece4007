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


namespace timeout {

	void dispatcher(int sig);

	struct TimeoutNode {
		void (*cb)();
		double diff;
	};

	List<TimeoutNode> toList;
	struct itimerval itimer;
	double TO;
	double epsilon = 1e-9;

	void setup_alarm(double time) {
		unsigned int s = (unsigned int)time;
		unsigned int us = (unsigned int)((time-s)*1e6);

		itimer.it_value.tv_sec = s;
		itimer.it_value.tv_usec = us;
		itimer.it_interval.tv_sec = 0;
		itimer.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL,&itimer,0);
		signal(SIGALRM,dispatcher);
	}

	void dispatcher(int sig) {
		TimeoutNode node;
		List<TimeoutNode> callList;

		toList.get(node,0);
		while(node.diff == 0 && !toList.isEmpty()) {
			toList.pop_front(node);
			callList.push(node);
			toList.get(node,0);
		}

		if(!toList.isEmpty()) {
			TO = node.diff;

			ListIterator<TimeoutNode> it = toList.begin();
			for(;it!=toList.end();++it) {
				(*it).diff = (*it).diff - TO;
				(*it).diff = ((*it).diff < epsilon) ? 0 : (*it).diff;
			}
			(*it).diff = (*it).diff - TO;
			(*it).diff = ((*it).diff < epsilon) ? 0 : (*it).diff;

			setup_alarm(TO);
		}

		ListIterator<TimeoutNode> it = callList.begin();
		for(;it!=callList.end();++it) {
			(*it).cb();
		}
		(*it).cb();
	}
}


void register_timeout(void (*func)(), double time) {

	timeout::TimeoutNode elem;
	elem.cb = func;

	if(timeout::toList.isEmpty()) {
		timeout::TO = time;
		elem.diff = 0;
		timeout::toList.push(elem);
		timeout::setup_alarm(timeout::TO);
	} else {
		double d = time - timeout::TO;

		if(d < 0) {
			elem.diff = 0;
			timeout::TO = time;

			ListIterator<timeout::TimeoutNode> it = timeout::toList.begin();
			for(;it!=timeout::toList.end();++it) {
				(*it).diff = (*it).diff - d;
				(*it).diff = ((*it).diff < timeout::epsilon) ? 0 : (*it).diff;
			}
			(*it).diff = (*it).diff - d;
			(*it).diff = ((*it).diff < timeout::epsilon) ? 0 : (*it).diff;

			timeout::toList.push_front(elem);
			timeout::setup_alarm(timeout::TO);
		} else {
			elem.diff = d;

			ListIterator<timeout::TimeoutNode> it = timeout::toList.begin();
			int i = 0;
			bool found = false;
			for(;it!=timeout::toList.end();++it) {
				if(d < (*it).diff) {
					found = true;
					break;
				}
				++i;
			}
			if(found) {
				timeout::toList.insert(elem,i);
			} else {
				if(d < (*it).diff) {
					timeout::toList.insert(elem,i);
				} else {
					timeout::toList.push(elem);
				}
			}
		
		}
	}
}

#endif
