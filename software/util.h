#ifndef UTIL_H
#define UTIL_H

#define MIN(a,b) ((a) < (b)) ? (a) : (b) 
#define MAX(a,b) ((a) > (b)) ? (a) : (b)
#define LIMIT(l,v,u) MIN(MAX((v),(l)),(u))


template <typename T>
void swap(T *a, T *b) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

#endif
