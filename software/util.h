#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define MIN(a,b) ((a) < (b)) ? (a) : (b) 
#define MAX(a,b) ((a) > (b)) ? (a) : (b)
#define LIMIT(l,v,u) MIN(MAX((v),(l)),(u))
#define ABSV(a)  ((a) < 0) ? -(a) : (a)


template <typename T>
void swap(T *a, T *b) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}


template <typename T, typename U>
U scale(T value, float factor, bool scaleup) {

	long f = factor * 1000;
	U res = 0;

	if(scaleup) {
		res = (U)((((long long)value*f)+500)/1000);
	} else {
		res = (U)((((long long)value*1000)-500)/f);
	}
	
	return res;
}

bool inDeadBand(int64_t valueA, int64_t valueB, int64_t thresh) {

	return (ABSV(valueA-valueB) < thresh);
}
#endif
