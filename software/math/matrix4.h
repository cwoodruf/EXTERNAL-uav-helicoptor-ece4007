#ifndef MATRIX4_H
#define MATRIX4_H

#include "vector3.h"
#include <iostream>

class Matrix4 {

	private:
		double a11, a12, a13, a14;
		double a21, a22, a23, a24;
		double a31, a32, a33, a34;
		double a41, a42, a43, a44;

		friend std::ostream& operator<<(std::ostream &os, const Matrix4 &m);
		friend Matrix4 operator*(const double &lhs, const Matrix4 &rhs);
	public:

		Matrix4() : a11(0), a12(0), a13(0), a14(0), a21(0), a22(0), a23(0), a24(0), a31(0), a32(0), a33(0), a34(0), a41(0), a42(0), a43(0), a44(0) {
		}

		Matrix4(double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44) :
			a11(a11), a12(a12), a13(a13), a14(a14), a21(a21), a22(a22), a23(a23), a24(a24), a31(a31), a32(a32), a33(a33), a34(a34), a41(a41), a42(a42), a43(a43), a44(a44) {
		}

		Matrix4(const Matrix4 &cpy) {
			a11 = cpy.a11; a12 = cpy.a12; a13 = cpy.a13; a14 = cpy.a14;
			a21 = cpy.a21; a22 = cpy.a22; a23 = cpy.a23; a24 = cpy.a24;
			a31 = cpy.a31; a32 = cpy.a32; a33 = cpy.a33; a34 = cpy.a34;
			a41 = cpy.a41; a42 = cpy.a42; a43 = cpy.a43; a44 = cpy.a44;
		}

		Matrix4 inverse() {
			Matrix4 m(
				 a22*a33*a44 - a22*a34*a43 - a32*a23*a44 + a32*a24*a43 + a42*a23*a34 - a42*a24*a33,
				-a12*a33*a44 + a12*a34*a43 + a32*a13*a44 - a32*a14*a43 - a42*a13*a34 + a42*a14*a33,
				 a12*a23*a44 - a12*a24*a43 - a22*a13*a44 + a22*a14*a43 + a42*a13*a24 - a42*a14*a23,
				-a12*a23*a34 + a12*a24*a33 + a22*a13*a34 - a22*a14*a33 - a32*a13*a24 + a32*a14*a23,

				-a21*a33*a44 + a21*a34*a43 + a31*a23*a44 - a31*a24*a43 - a41*a23*a34 + a41*a24*a33,
				 a11*a33*a44 - a11*a34*a43 - a31*a13*a44 + a31*a14*a43 + a41*a13*a34 - a41*a14*a33,
				-a11*a23*a44 + a11*a24*a43 + a21*a13*a44 - a21*a14*a43 - a41*a13*a24 + a41*a14*a23,
				 a11*a23*a34 - a11*a24*a33 - a21*a13*a34 + a21*a14*a33 + a31*a13*a24 - a31*a14*a23,

				 a21*a32*a44 - a21*a34*a42 - a31*a22*a44 + a31*a24*a42 + a41*a22*a34 - a41*a24*a32,
				-a11*a32*a44 + a11*a34*a42 + a31*a12*a44 - a31*a14*a42 - a41*a12*a34 + a41*a14*a32,
				 a11*a22*a44 - a11*a24*a42 - a21*a12*a44 + a21*a14*a42 + a41*a12*a24 - a41*a14*a22,
				-a11*a22*a34 + a11*a24*a32 + a21*a12*a34 - a21*a14*a32 - a31*a12*a24 + a31*a14*a22,

				-a21*a32*a43 + a21*a33*a42 + a31*a22*a43 - a31*a23*a42 - a41*a22*a33 + a41*a23*a32,
				 a11*a32*a43 - a11*a33*a42 - a31*a12*a43 + a31*a13*a42 + a41*a12*a33 - a41*a13*a32,
				-a11*a22*a43 + a11*a23*a42 + a21*a12*a43 - a21*a13*a42 - a41*a12*a23 + a41*a13*a22,
				 a11*a22*a33 - a11*a23*a32 - a21*a12*a33 + a21*a13*a32 + a31*a12*a23 - a31*a13*a22
			);	

			double D = a11*m.a11 + a12*m.a12 + a31*m.a13 + a41*m.a14;
			if(D != 0) {
				m.a11/=D; m.a12/=D; m.a13/=D; m.a14/=D;
				m.a21/=D; m.a22/=D; m.a23/=D; m.a24/=D;
				m.a31/=D; m.a32/=D; m.a33/=D; m.a34/=D;
				m.a41/=D; m.a42/=D; m.a43/=D; m.a44/=D;
			}

			return m;
		}

		Matrix4 transpose() {
			return Matrix4(
				a11, a21, a31, a41,
				a12, a22, a32, a42,
				a13, a23, a33, a43,
				a14, a24, a34, a44
			);
		}

		Matrix4& operator=(const Matrix4 &rhs) {
			a11 = rhs.a11; a12 = rhs.a12; a13 = rhs.a13; a14 = rhs.a14;
			a21 = rhs.a21; a22 = rhs.a22; a23 = rhs.a23; a24 = rhs.a24;
			a31 = rhs.a31; a32 = rhs.a32; a33 = rhs.a33; a34 = rhs.a34;
			a41 = rhs.a41; a42 = rhs.a42; a43 = rhs.a43; a44 = rhs.a44;
			return *this;
		}

		bool operator==(const Matrix4 &rhs) {
			return (a11 == rhs.a11 && a12 == rhs.a12 && a13 == rhs.a13 && a14 == rhs.a14 &&	a21 == rhs.a21 && a22 == rhs.a22 && a23 == rhs.a23 && a24 == rhs.a24 && a31 == rhs.a31 && a32 == rhs.a32 && a33 == rhs.a33 && a34 == rhs.a34 && a41 == rhs.a41 && a42 == rhs.a42 && a43 == rhs.a43 && a44 == rhs.a44);
		}

		bool operator!=(const Matrix4 &rhs) {
			return !(*this == rhs);
		}

		Matrix4 operator*(const Matrix4 &rhs) {
			return Matrix4(
				a11*rhs.a11 + a12*rhs.a21 + a13*rhs.a31 + a14*rhs.a41,
				a11*rhs.a12 + a12*rhs.a22 + a13*rhs.a32 + a14*rhs.a42,
				a11*rhs.a13 + a12*rhs.a23 + a13*rhs.a33 + a14*rhs.a43,
				a11*rhs.a14 + a12*rhs.a24 + a13*rhs.a34 + a14*rhs.a44,

				a21*rhs.a11 + a22*rhs.a21 + a23*rhs.a31 + a24*rhs.a41,
				a21*rhs.a12 + a22*rhs.a22 + a23*rhs.a32 + a24*rhs.a42,
				a21*rhs.a13 + a22*rhs.a23 + a23*rhs.a33 + a24*rhs.a43,
				a21*rhs.a14 + a22*rhs.a24 + a23*rhs.a34 + a24*rhs.a44,

				a31*rhs.a11 + a32*rhs.a21 + a33*rhs.a31 + a34*rhs.a41,
				a31*rhs.a12 + a32*rhs.a22 + a33*rhs.a32 + a34*rhs.a42,
				a31*rhs.a13 + a32*rhs.a23 + a33*rhs.a33 + a34*rhs.a43,
				a31*rhs.a14 + a32*rhs.a24 + a33*rhs.a34 + a34*rhs.a44,

				a41*rhs.a11 + a42*rhs.a21 + a43*rhs.a31 + a44*rhs.a41,
				a41*rhs.a12 + a42*rhs.a22 + a43*rhs.a32 + a44*rhs.a42,
				a41*rhs.a13 + a42*rhs.a23 + a43*rhs.a33 + a44*rhs.a43,
				a41*rhs.a14 + a42*rhs.a24 + a43*rhs.a34 + a44*rhs.a44
			);
		}

		Vector3 operator*(const Vector3 &rhs) {
			Vector3 v(rhs);
			double t = v[0]*a41 + v[1]*a42 + v[2]*a43 + a44;
			return Vector3(
				(v[0]*a11 + v[1]*a12 + v[2]*a13 + a14)/t,
				(v[0]*a21 + v[1]*a22 + v[2]*a23 + a24)/t,
				(v[0]*a31 + v[1]*a32 + v[2]*a33 + a34)/t
			);
		}

		Matrix4 operator*(const double &rhs) {
			return Matrix4(
				a11*rhs, a12*rhs, a13*rhs, a14*rhs,
				a21*rhs, a22*rhs, a23*rhs, a24*rhs,
				a31*rhs, a32*rhs, a33*rhs, a34*rhs,
				a41*rhs, a42*rhs, a43*rhs, a44*rhs
			);
		}

		Matrix4 operator~() {
			return (*this).transpose();
		}

		Matrix4 operator!() {
			return (*this).inverse();
		} 

		double& operator[](const int &i) {
			switch(i) {
				case 0: return a11;
				case 1: return a12;
				case 2: return a13;
				case 3: return a14;
				case 4: return a21;
				case 5: return a22;
				case 6: return a23;
				case 7: return a24;
				case 8: return a31;
				case 9: return a32;
				case 10: return a33;
				case 11: return a34;
				case 12: return a41;
				case 13: return a42;
				case 14: return a43;
				case 15: return a44;
				default: throw "Matrix4: Invalid Index";
			}
		}

		double& operator()(const int &r,const int &c) {
			switch(r) {
				case 0:
					switch(c) {
						case 0: return a11;
						case 1: return a12;
						case 2: return a13;
						case 3: return a14;
						default: throw "Matrix4: Invalid Column Index";
					}
				case 1:
					switch(c) {
						case 0: return a21;
						case 1: return a22;
						case 2: return a23;
						case 3: return a24;
						default: throw "Matrix4: Invalid Column Index";
					}
				case 2:
					switch(c) {
						case 0: return a31;
						case 1: return a32;
						case 2: return a33;
						case 3: return a34;
						default: throw "Matrix4: Invalid Column Index";
					}
				case 3:
					switch(c) {
						case 0: return a41;
						case 1: return a42;
						case 2: return a43;
						case 3: return a44;
						default: throw "Matrix4: Invalid Column Index";
					}
				default:	
					throw "Matrix4: Invalid Row Index";
			}
		}

};

namespace matrix4 {
	Matrix4 identity() {
		return Matrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
}

Matrix4 operator*(const double &lhs, const Matrix4 &rhs) {
	return Matrix4(
		rhs.a11*lhs, rhs.a12*lhs, rhs.a13*lhs, rhs.a14*lhs,
		rhs.a21*lhs, rhs.a22*lhs, rhs.a23*lhs, rhs.a24*lhs,
		rhs.a31*lhs, rhs.a32*lhs, rhs.a33*lhs, rhs.a34*lhs,
		rhs.a41*lhs, rhs.a42*lhs, rhs.a43*lhs, rhs.a44*lhs
	);
}

std::ostream& operator<<(std::ostream &os, const Matrix4 &m) {
	os <<
	"|" << m.a11 << "\t" << m.a12 << "\t" << m.a13 << "\t" << m.a14 << "|" << std::endl <<
	"|" << m.a21 << "\t" << m.a22 << "\t" << m.a23 << "\t" << m.a24 << "|" << std::endl <<
	"|" << m.a31 << "\t" << m.a32 << "\t" << m.a33 << "\t" << m.a34 << "|" << std::endl <<
	"|" << m.a41 << "\t" << m.a42 << "\t" << m.a43 << "\t" << m.a44 << "|";
	return os;
}

#endif
