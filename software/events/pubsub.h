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
