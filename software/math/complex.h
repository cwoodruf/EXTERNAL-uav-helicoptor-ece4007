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

#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>
#include <iostream>

class Complex {

	private:
		double real;
		double imag;
		bool NaN;

		friend Complex operator*(const double &lhs,const Complex &rhs);
		friend Complex operator/(const double &lhs,const Complex &rhs);
		friend std::ostream& operator<<(std::ostream &os, const Complex &c);

	public:
		Complex();
		Complex(double r, double i);
		Complex(double r);
		Complex(bool b);
		Complex(const Complex &cpy);
		~Complex() {};

		double getReal();
		double getImag();
		bool isNaN();
		bool isReal();
		bool isComplex();
		void toConj();


		Complex operator+(const Complex &rhs) {
			return Complex(real+rhs.real,imag+rhs.imag);
		}

		Complex& operator+=(const Complex &rhs) {
			real += rhs.real;
			imag += rhs.imag;
			return *this;
		}

		Complex operator-(const Complex &rhs) {
			return Complex(real-rhs.real,imag-rhs.imag);
		}

		Complex& operator-=(const Complex &rhs) {
			real -= rhs.real;
			imag -= rhs.imag;
			return *this;
		}

		Complex operator*(const double &rhs) {
			return Complex(
				real*rhs,
				imag*rhs
			);
		}

		Complex& operator*=(const double &rhs) {
			*this = *this * rhs;
			return *this;
		}

		Complex operator/(const double &rhs) {
			return Complex(
				real/rhs,
				imag/rhs
			);
		}

		Complex& operator/=(const double &rhs) {
			*this = *this / rhs;
			return *this;
		}

		Complex operator*(const Complex &rhs) {
			return Complex(
				real*rhs.real - imag*rhs.imag,
				imag*rhs.real + real*rhs.imag
			);
		}

		Complex& operator*=(const Complex &rhs) {
			*this = *this * rhs;
			return *this;
		}

		Complex operator/(const Complex &rhs) {
			double den = (rhs.real*rhs.real + rhs.imag*rhs.imag);
			return Complex(
				(real*rhs.real + imag*rhs.imag)/den,
				(imag*rhs.real - real*rhs.imag)/den
			);
		}

		Complex& operator/=(const Complex &rhs) {
			*this = *this / rhs;
			return *this;
		}

		bool operator==(const Complex &rhs) {
			return ((real==rhs.real) && (imag==rhs.real));
		}
	
		bool operator!=(const Complex &rhs) {
			return !(*this != rhs); 
		}

		Complex& operator=(const Complex &rhs) {
			this->real = rhs.real;
			this->imag = rhs.imag;
			this->NaN = rhs.NaN;
			return *this;
		}

		Complex& operator=(const double &rhs) {
			this->real = rhs;
			this->imag = 0.0;
			this->NaN = false;
			return *this;
		}

		Complex operator-() {
			return Complex(-real,-imag);
		}

		bool operator<(const Complex &rhs);
		bool operator<=(const Complex &rhs);
		bool operator>(const Complex &rhs);
		bool operator>=(const Complex &rhs);
		bool operator<(const double &rhs);
		bool operator<=(const double &rhs);
		bool operator>(const double &rhs);
		bool operator>=(const double &rhs);
		bool operator==(const double &rhs);
		bool operator!=(const double &rhs);

		operator double();	
};

namespace complex_number {

	double real(Complex c) {
		return c.getReal();
	}

	double imag(Complex c) {
		return c.getImag();
	}

	Complex conj(Complex c) {
		return Complex(
			c.getReal(),
			-(c.getImag())
		);
	}

	double abs(Complex c) {
		double r = c.getReal();
		double i = c.getImag();
		return std::sqrt(r*r+i*i);
	}

	template <typename T> int sgn(T val) {
		return (val > T(0)) - (val < T(0));
	}	


	Complex sqrt(Complex c) {
		double r = c.getReal();
		double i = c.getImag();
		if(i != 0.0) {
			return Complex(
				std::sqrt((r+std::sqrt(r*r+i*i))/2),
				sgn<double>(i)*((-r+std::sqrt(r*r+i*i))/2)
			);
		} else {
			return Complex(std::sqrt(r));
		}
	}

}

Complex::Complex() : real(0), imag(0), NaN(false) {
}

Complex::Complex(double r) : real(r), imag(0), NaN(false) {
}

Complex::Complex(double r, double i) : real(r), imag(i), NaN(false) {
}

Complex::Complex(bool b) : real(0), imag(0), NaN(b) {
}

Complex::Complex(const Complex &cpy) {
	real = cpy.real;
	imag = cpy.imag;
	NaN = cpy.NaN;
}

double Complex::getReal() {
	return real;
}

double Complex::getImag() {
	return imag;
}

bool Complex::isNaN() {
	return NaN;
}

bool Complex::isReal() {
	return (imag == 0);
}

bool Complex::isComplex() {
	return (imag != 0);
}

void Complex::toConj() {
	imag = -imag;
}

bool Complex::operator<(const Complex &rhs) {
	return (complex_number::abs(*this) < complex_number::abs(rhs));
}

bool Complex::operator<=(const Complex &rhs) {
	return (complex_number::abs(*this) <= complex_number::abs(rhs));
}

bool Complex::operator>(const Complex &rhs) {
	return (complex_number::abs(*this) > complex_number::abs(rhs));
}

bool Complex::operator>=(const Complex &rhs) {
	return (complex_number::abs(*this) >= complex_number::abs(rhs));
}

bool Complex::operator<(const double &rhs) {
	return (complex_number::abs(*this) < rhs);
}

bool Complex::operator<=(const double &rhs) {
	return (complex_number::abs(*this) <= rhs);
}

bool Complex::operator>(const double &rhs) {
	return (complex_number::abs(*this) > rhs);
}

bool Complex::operator>=(const double &rhs) {
	return (complex_number::abs(*this) >= rhs);
}

bool Complex::operator==(const double &rhs) {
	return (static_cast<double>(*this) == rhs);
}

bool Complex::operator!=(const double &rhs) {
	return (static_cast<double>(*this) != rhs);
}

Complex::operator double() { 
	return this->real;
}

Complex operator*(const double &lhs,const Complex &rhs) {
	return Complex(
		lhs*rhs.real,
		lhs*rhs.imag
	);
}

Complex operator/(const double &lhs,const Complex &rhs) {
	return Complex(
		lhs/rhs.real,
		lhs/rhs.imag
	);
}


std::ostream& operator<<(std::ostream &os, const Complex &c) {
	if(c.imag) {
		os << "(" << c.real << "," << c.imag << ")"; 
	} else {
		os << c.real;
	}
	return os;
}
#endif
