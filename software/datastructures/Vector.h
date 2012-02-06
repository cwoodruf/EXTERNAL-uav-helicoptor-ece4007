#ifndef VECTOR_H
#define VECTOR_H

#include <unistd.h>
#include <stdlib.h>

template <typename T> class VectorIterator;
template <typename T>
class Vector {

	private:
		T* elements;
		size_t count;
		size_t reserved;


	public:
		Vector();
		Vector(const Vector&);
		~Vector();

		void Push_Back(const T&);
		void Push_Front(const T&);
		void Pop_Back();
		void Pop_Front();
		T& Front() const;
		T& Back() const;
		T& operator[](size_t) const;
		size_t Size() const;
		bool Empty() const;

		void Clear();	

		VectorIterator<T> Begin() const;
		VectorIterator<T> End() const;

		Vector &operator=(const Vector &cpy) {
			count = cpy.count;
			reserved = cpy.reserved;
			elements = (T*)malloc((reserved) * sizeof(T));

			for(int i=0;i<count;++i) {
				new (&elements[i]) T(cpy.elements[i]);
			}

			return *this;
		}
};

template <typename T>
class VectorIterator {
	private:
		T* current;
		friend class Vector<T>;

	public:
		VectorIterator();
		VectorIterator(T*);
		VectorIterator(const VectorIterator&);
		T& operator*() const;
		VectorIterator operator++();
		VectorIterator operator++(int);
		bool operator !=(const VectorIterator&) const;
		bool operator ==(const VectorIterator&) const;
};

template <typename T>
Vector<T>::Vector() : elements(NULL), count(0), reserved(0) {
}

template <typename T>
Vector<T>::Vector(const Vector& rhs) {

	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T*)malloc((reserved) * sizeof(T));

	for(int i=0;i<count;++i) {
		new (&elements[i]) T(rhs.elements[i]);
	}
}

template <typename T>
Vector<T>::~Vector() {
	for(int i=0;i<reserved;++i) {
		if(i < count) {
			elements[i].~T();
		}
	}
	free(elements);
}

template <typename T>
void Vector<T>::Push_Back(const T& rhs) {
	if(reserved > count) { 
		new (&elements[count]) T(rhs);
	} else {
		T* newElements = (T*)malloc((count + 1) * sizeof(T));
		reserved = count + 1;

		for (size_t i = 0; i < count; ++i) {
			new (&newElements[i]) T(elements[i]);
			elements[i].~T();
				}

		new (&newElements[count]) T(rhs);

		free(elements);
		elements = newElements;
	}
	count++;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs) {
	if(reserved > count) {
		for(int i=0;i<=count;++i) {
			elements[i+1] = elements[i];
		}

		elements[0].~T();
		new (&elements[0]) T(rhs);
	} else {
		T* newElements = (T*)malloc((count + 1) * sizeof(T));
		reserved = count + 1;

		for(int i=0;i<count;++i) {
			new (&newElements[i+1]) T(elements[i]);
			elements[i].~T();
		}
		
		new (&newElements[0]) T(rhs);

		free(elements);
		elements = newElements;
	}
	count++;
}

template <typename T>
void Vector<T>::Pop_Back() {

	if(count > 1) {
		--count;

		T* newElements = (T*)malloc((count) * sizeof(T));
		for(int i=0;i<count;++i) {
			new (&newElements[i]) T(elements[i]);
			elements[i].~T();
		}

		elements[count].~T();
		free(elements);
		elements = newElements;
	} else if(count == 1) {
		elements[0].~T();
		--count;
	} else {
		return;
	}
}

template <typename T>
void Vector<T>::Pop_Front() {
	if(count > 1) {
		--count;

		T* newElements = (T*)malloc((count) * sizeof(T));
		for(int i=0;i<count;++i) {
			new (&newElements[i]) T(elements[i+1]);
			elements[i+1].~T();
		}

		elements[0].~T();
		free(elements);
		elements = newElements;
	} else if(count == 1) {
		elements[0].~T();
		--count;
	} else {
		return;
	}
}

template <typename T>
T& Vector<T>::Front() const {
	return elements[0];
}

template <typename T>
T& Vector<T>::Back() const {
	return elements[count-1];
}

template <typename T>
T& Vector<T>::operator[](size_t i) const {
	return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const {
	return count;
}

template <typename T>
bool Vector<T>::Empty() const {
	return (count == 0); 
}

template <typename T>
void Vector<T>::Clear() {
	while(count) Pop_Back();
}

template <typename T>
VectorIterator<T> Vector<T>::Begin() const {
	return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const {
	return VectorIterator<T>(elements+count);
}

template <typename T>
VectorIterator<T>::VectorIterator() {
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)	{
	current = c;
}

template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs) {
	current = rhs.current;
}

template <typename T>
T& VectorIterator<T>::operator*() const {
	return *current;
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator++() {
	if(current) {
		++current;
	}
	return *this;
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int) {
	VectorIterator tmp(*this);
	if(current) {
		++current;
	}
	return tmp;
}

template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const {
	return (current != rhs.current);
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const {
	return (current == rhs.current);
}

#endif
