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
#ifndef MATRIX3_H
#define MATRIX3_H

#include "vector3.h"
#include <iostream>

class Matrix3 {

	private:
		double a11, a12, a13;
		double a21, a22, a23;
		double a31, a32, a33;

		friend Matrix3 operator*(const double &lhs,const Matrix3 &rhs);
		friend std::ostream& operator<<(std::ostream &os, const Matrix3 &m);

	public:

		Matrix3() :	a11(0), a12(0), a13(0),	a21(0), a22(0), a23(0),	a31(0), a32(0), a33(0) {
		}

		Matrix3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33) :
			a11(a11), a12(a12), a13(a13),	a21(a21), a22(a22), a23(a23),	a31(a31), a32(a32), a33(a33) {
		}

		Matrix3(Vector3 v1, Vector3 v2, Vector3 v3) {
			a11 = v1[0]; a12 = v1[1]; a13 = v1[2];
			a21 = v2[0]; a22 = v2[1]; a23 = v2[2];
			a31 = v3[0]; a32 = v3[1]; a33 = v3[2];
		}

		Matrix3(const Matrix3 &cpy) {
			a11=cpy.a11; a12=cpy.a12; a13=cpy.a13;	
			a21=cpy.a21; a22=cpy.a22; a23=cpy.a23;
			a31=cpy.a31; a32=cpy.a32; a33=cpy.a33;
		}

		Vector3 column(int n) {
			switch(n) {
				case 0:
					return Vector3(a11,a21,a31);
				case 1:
					return Vector3(a12,a22,a32);
				case 2:
					return Vector3(a13,a23,a33);
				default:
					throw "Matrix3: Invalid Column Index";
			}
		}

		Vector3 row(int n) {
			switch(n) {
				case 0:
					return Vector3(a11,a12,a13);
				case 1:
					return Vector3(a21,a22,a23);
				case 2:
					return Vector3(a31,a32,a33);
				default:
					throw "Matrix3: Invalid Row Index";
			}
		}

		void getColumnVectors(Vector3 &c1, Vector3 &c2, Vector3 &c3) {
			c1 = column(0);
			c2 = column(1);
			c3 = column(2);
		}

		void getRowVectors(Vector3 &r1, Vector3 &r2, Vector3 &r3) {
			r1 = row(0);
			r2 = row(1);
			r3 = row(2);
		}

		Matrix3 scale(Vector3 s) {
			return Matrix3(
				a11*s[0], a12*s[1], a13*s[2],
				a21*s[0], a22*s[1], a23*s[2],
				a31*s[0], a32*s[1], a33*s[2]
			);
		}

		Matrix3 transpose() {
			return Matrix3(
				a11, a21, a31,
				a12, a22, a32,
				a13, a23, a33
			);
		}

		double determinant() {
			return (a11*a22*a33 - a11*a23*a32 + a21*a32*a13 - a21*a12*a33 + a31*a12*a23 - a31*a22*a13);
		}

		Matrix3 inverse() {
			double d = (*this).determinant();
			double t11 = (a22*a33-a23*a32)/d;	
			double t12 = (a12*a33-a13*a32)/d;	
			double t13 = (a12*a23-a13*a22)/d;	
			double t21 = (-a31*a23+a21*a33)/d;	
			double t22 = (-a31*a13+a11*a33)/d;	
			double t23 = (-a21*a13+a11*a23)/d;	
			double t31 = (-a31*a22+a21*a32)/d;	
			double t32 = (-a31*a12+a11*a32)/d;	
			double t33 = (-a21*a12+a11*a22)/d;	

			return Matrix3(
				t11,t12,t13,
				t21,t22,t23,
				t31,t32,t33
			);
		}

		Matrix3& operator=(const double &s) {
			a11=s; a12=0; a13=0;	
			a21=0; a22=s; a23=0;
			a31=0; a32=0; a33=s;
			return *this;
		}

		Matrix3& operator=(const Matrix3 &cpy) {
			a11=cpy.a11; a12=cpy.a12; a13=cpy.a13;	
			a21=cpy.a21; a22=cpy.a22; a23=cpy.a23;
			a31=cpy.a31; a32=cpy.a32; a33=cpy.a33;
			return *this;
		}

		double& operator[](const int &n) {
			return row(n/3)[n%3];
		}

		double& operator()(const int &r,const int &c) {
			return row(r)[c];
		}

		Matrix3 operator+(const Matrix3 &rhs) {
			return Matrix3(
				a11+rhs.a11, a12+rhs.a12, a13+rhs.a13,
				a21+rhs.a21, a22+rhs.a22, a23+rhs.a23,
				a31+rhs.a31, a32+rhs.a32, a33+rhs.a33
			);
		}

		Matrix3 operator-(const Matrix3 &rhs) {
			return Matrix3(
				a11-rhs.a11, a12-rhs.a12, a13-rhs.a13,
				a21-rhs.a21, a22-rhs.a22, a23-rhs.a23,
				a31-rhs.a31, a32-rhs.a32, a33-rhs.a33
			);
		}

		Matrix3 operator*(const double &rhs) {
			return Matrix3(
				a11*rhs,a12*rhs,a13*rhs,
				a21*rhs,a22*rhs,a23*rhs,
				a31*rhs,a32*rhs,a33*rhs
			);
		}

		Matrix3 operator*(const Matrix3 &rhs) {
			return Matrix3(
				a11*rhs.a11 + a12*rhs.a21 + a13*rhs.a31,
				a11*rhs.a12 + a12*rhs.a22 + a13*rhs.a32,
				a11*rhs.a13 + a12*rhs.a23 + a13*rhs.a33,

				a21*rhs.a11 + a22*rhs.a21 + a23*rhs.a31,
				a21*rhs.a12 + a22*rhs.a22 + a23*rhs.a32,
				a21*rhs.a13 + a22*rhs.a23 + a23*rhs.a33,

				a31*rhs.a11 + a32*rhs.a21 + a33*rhs.a31,
				a31*rhs.a12 + a32*rhs.a22 + a33*rhs.a32,
				a31*rhs.a13 + a32*rhs.a23 + a33*rhs.a33
			);
		}

		Matrix3& operator~() {
			double tmp;
			tmp = a12; a12 = a21; a21 = tmp;
			tmp = a13; a13 = a31; a31 = tmp;
			tmp = a23; a23 = a32; a32 = tmp;
			return *this;
		}

		Matrix3& operator!() {
			Matrix3 t(*this);
			double d = (*this).determinant();
			a11 = (t.a22*t.a33-t.a23*t.a32)/d;	
			a12 = (t.a12*t.a33-t.a13*t.a32)/d;	
			a13 = (t.a12*t.a23-t.a13*t.a22)/d;	
			a21 = (-t.a31*t.a23+t.a21*t.a33)/d;	
			a22 = (-t.a31*t.a13+t.a11*t.a33)/d;	
			a23 = (-t.a21*t.a13+t.a11*t.a23)/d;	
			a31 = (-t.a31*t.a22+t.a21*t.a32)/d;	
			a32 = (-t.a31*t.a12+t.a11*t.a32)/d;	
			a33 = (-t.a21*t.a12+t.a11*t.a22)/d;	
			return *this;
		}

		bool operator==(const Matrix3 &rhs) {
			return (a11==rhs.a11 && a12==rhs.a12 && a13==rhs.a13 &&
					a21==rhs.a21 && a22==rhs.a22 && a23==rhs.a23 &&
					a31==rhs.a31 && a32==rhs.a32 && a33==rhs.a33
			);
		} 

		bool operator!=(const Matrix3 &rhs) {
			return !(a11==rhs.a11 && a12==rhs.a12 && a13==rhs.a13 &&
					a21==rhs.a21 && a22==rhs.a22 && a23==rhs.a23 &&
					a31==rhs.a31 && a32==rhs.a32 && a33==rhs.a33
			);
		} 

};

namespace matrix3 {
	Matrix3 identity() {
		return Matrix3(1,0,0,0,1,0,0,0,1);
	}
}

Matrix3 operator*(const double &lhs,const Matrix3 &rhs) {
	Matrix3 m(rhs);
	return m*lhs;
}

std::ostream& operator<<(std::ostream &os, const Matrix3 &m) {
	os <<
	"|" << m.a11 << "," << m.a12 << "," << m.a13 << "|" << std::endl <<
	"|" << m.a21 << "," << m.a22 << "," << m.a23 << "|" << std::endl <<
	"|" << m.a31 << "," << m.a32 << "," << m.a33 << "|";
	return os;
}

#endif
