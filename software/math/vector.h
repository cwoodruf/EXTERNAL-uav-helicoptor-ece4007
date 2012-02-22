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
#ifndef VECTOR_H
#define VECTOR_H

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

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
		Vector(int num);
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

			//Delete Memory
			if(elements) {
				for(int i=0;i<reserved;++i) {
					if(i < count) {
						elements[i].~T();
					}
				}
				free(elements);
			}

			//Allocate New Memory
			count = cpy.count;
			reserved = cpy.reserved;
			elements = (T*)malloc((reserved) * sizeof(T));
			if(elements) {
				//Copy Data
				for(int i=0;i<count;++i) {
					new (&elements[i]) T(cpy.elements[i]);
				}
			}

			return *this;
		}

		//Component by Component Addition
		Vector<T> operator+(const Vector &rhs) {
			Vector<T> out;
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] + rhs.elements[i]);
			}

			return out;
		}

		//Component by Component Addition Assign
		Vector<T>& operator+=(const Vector &rhs) {
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				elements[i] += rhs.elements[i];
			}

			return &this;
		}

		//Component by Component Subtraction
		Vector<T> operator-(const Vector &rhs) {
			Vector<T> out;
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] - rhs.elements[i]);
			}

			return out;
		}


		//Component by Component Subtraction Assign
		Vector<T>& operator-=(const Vector &rhs) {
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				elements[i] -= rhs.elements[i];
			}

			return &this;
		}

		//Scalar Multiplication
		Vector<T> operator*(const double &rhs) {
			Vector<T> out;
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] * rhs);
			}

			return out;
		}

	
		//Scalar Mulitplication Assign
		Vector<T>& operator*=(const double &rhs) {
			for(int i=0;i<count;++i) {
				elements[i] *= rhs;
			}

			return &this;
		}

		//Scalar Multiplication
		Vector<T> operator*(const T &rhs) {
			Vector<T> out;
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] * rhs);
			}

			return out;
		}

		//Scalar Mulitplication Assign
		Vector<T>& operator*=(const T &rhs) {
			for(int i=0;i<count;++i) {
				elements[i] *= rhs;
			}

			return &this;
		}

		//Component by Component Multiplication
		Vector<T> operator*(const Vector &rhs) {
			Vector<T> out;
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] * rhs.elements[i]);
			}

			return out;
		}

		//Component by Component Mulitplication Assign
		Vector<T>& operator*=(const Vector &rhs) {
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				elements[i] *= rhs.elements[i];
			}

			return *this;
		}

		//Scalar Division
		Vector<T> operator/(const double &rhs) {
			Vector<T> out;
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] / rhs);
			}

			return out;
		}

		//Scalar Division Assign
		Vector<T>& operator/=(const double &rhs) {
			for(int i=0;i<count;++i) {
				elements[i] /= rhs;
			}

			return &this;
		}


		//Scalar Division
		Vector<T> operator/(const T &rhs) {
			Vector<T> out;
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] / rhs);
			}

			return out;
		}

		//Scalar Division Assign
		Vector<T>& operator/=(const T &rhs) {
			for(int i=0;i<count;++i) {
				elements[i] /= rhs;
			}

			return &this;
		}

		//Component by Component Division
		Vector<T> operator/(const Vector &rhs) {
			Vector<T> out;
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				out.Push_Back(elements[i] / rhs.elements[i]);
			}

			return out;
		}

		//Component by Component Addition Assign
		Vector<T>& operator/=(const Vector &rhs) {
			if(count != rhs.count) {
				throw "Vector: Dimensions do not agree";
			}
			for(int i=0;i<count;++i) {
				elements[i] /= rhs.elements[i];
			}

			return &this;
		}

		//Index Operator
		T& operator[](const int &index) {
			int i;
			if(index < 0) {
				i = count + index;
			} else {
				if(index > count) {
					i = count;
				} else {
					i = index;
				}
			}

			return elements[i];
		}

		//slice
		Vector<T> operator()(const int &start, const int &end) {

			int st = (start < 0) ? 0 : start;
			int en = (end < 0) ? count+end : ((end > count) ? count : end);
			int n = en-start+1;

			Vector<T> out;
			for(int i=start;i<n+start;++i) {
				out.Push_Back(elements[i]);
			}

			return out;
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
Vector<T>::Vector(int num) {
	count = num;
	reserved = num;
	elements = (T*)malloc((reserved) * sizeof(T));

	for(int i=0;i<count;++i) {
		new (&elements[i]) T();
	}
}

template <typename T>
Vector<T>::Vector(const Vector& cpy) {

	count = cpy.count;
	reserved = cpy.reserved;
	elements = (T*)malloc((reserved) * sizeof(T));

	for(int i=0;i<count;++i) {
		new (&elements[i]) T(cpy.elements[i]);
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

template <typename T>
Vector<T> operator*(const double &lhs,const Vector<T> &rhs) {
	Vector<T> out;
	for(int i=0,l=rhs.Size();i<l;++i) {
		out.Push_Back(rhs[i] * lhs);
	}

	return out;
}

template <typename T>
Vector<T> operator*(const T &lhs,const Vector<T> &rhs) {
	Vector<T> out;
	for(int i=0,l=rhs.Size();i<l;++i) {
		out.Push_Back(rhs[i] * lhs);
	}

	return out;
}

/*
template <typename T>
Vector<T> operator/(const double &lhs,const Vector<T> &rhs) {
	Vector<T> out;
	for(int i=0,l=rhs.Size();i<l;++i) {
		out.Push_Back(lhs/rhs[i]);
	}

	return out;
}

template <typename T>
Vector<T> operator/(const T &lhs,const Vector<T> &rhs) {
	Vector<T> out;
	for(int i=0,l=rhs.Size();i<l;++i) {
		out.Push_Back(lhs/rhs[i]);
	}

	return out;
}
*/

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector<T> &v) {
	os << "<";
	for(int i=0,l=v.Size();i<l;++i) {
		os << v[i]; 
		if(i != l-1) {
			os << ",";
		}
	}
	os << ">";

	return os;
}

#endif
