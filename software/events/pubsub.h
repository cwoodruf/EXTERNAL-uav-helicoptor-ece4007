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
#ifndef PUBSUB_H
#define PUBSUB_H

#include "../datastructures/hashTable.h"
#include "../datastructures/Vector.h"
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

namespace pubsub {
	struct Subscription {
		long long handle;
		void (*cb)(void *);
		bool valid;
	};

	HashTable<Vector<Subscription> > cache;
	HashTable<string> handle_cache;
	unsigned long long last_handle;
}

void publish(const char *topic,void *data) {
	Vector<pubsub::Subscription> vec;

	if(pubsub::cache.get(topic,vec) == 0) {
		VectorIterator<pubsub::Subscription> it = vec.Begin();

		while(it != vec.End()) {
			if((*it).valid) {
				(*it).cb(data);
			}
			++it;
		}
	}
}

unsigned long long subscribe(const char *topic,void (*cb)(void *)) {
	Vector<pubsub::Subscription> vec;
	pubsub::Subscription sub = {pubsub::last_handle,cb,true};

	pubsub::cache.get(topic,vec);
	vec.Push_Back(sub);
	pubsub::cache.put(topic,vec);

	char buf[64];
	sprintf(buf,"%llu",pubsub::last_handle);
	pubsub::handle_cache.put(buf,topic);

	return pubsub::last_handle++;
}

void unsubscribe(unsigned long long handle) {
	Vector<pubsub::Subscription> vec;
	string s;

	char buf[64];
	sprintf(buf,"%llu",handle);
	if(pubsub::handle_cache.get(buf,s) == 0) {
		if(pubsub::cache.get(s.c_str(),vec) == 0) {
			VectorIterator<pubsub::Subscription> it = vec.Begin();

			while(it != vec.End()) {
				if((*it).handle == handle) {
					(*it).valid = false;
					pubsub::cache.put(s.c_str(),vec);
				}
				++it;
			}
		}
	}
}

#endif
