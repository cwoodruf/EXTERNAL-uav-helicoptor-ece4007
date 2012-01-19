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
#endif
