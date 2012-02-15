#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <iostream>

class Vector3 {

	private:
		double x;
		double y;
		double z;

		friend std::ostream& operator<<(std::ostream &os, const Vector3 &v);

	public:
		Vector3() : x(0), y(0), z(0) {
		}

		Vector3(double x, double y, double z) : x(x), y(y), z(z) {
		}

		Vector3(const Vector3 &cpy) {
			x = cpy.x;
			y = cpy.y;
			z = cpy.z;
		}

		double dot(const Vector3 &v) {
			return x*v.x + y*v.y + z*v.z;
		}

		double xydot(const Vector3 &v) {
			return x*v.x + y*v.y;
		}

		Vector3 cross(const Vector3 &v) {
			Vector3 o(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
			return o;
		}

		Vector3 normalize() {
			double l = sqrt(x*x + y*y + z*z);
			if(l == 0.0) {
				Vector3 v(1,0,0);
				return v;
			}
			l = 1/l;
			Vector3 v(x*l,y*l,z*l);
			return v;
		}

		double mag() {
			return sqrt(x*x + y*y + z*z);
		}

		double xymag() {
			return sqrt(x*x + y*y);
		}

		double squared_mag() {
			return x*x + y*y + z*z;
		}

		void toArray(double *array) {
			array[0] = x;
			array[1] = y;
			array[2] = z;
		}

		Vector3	operator+(const Vector3 &rhs) {
			Vector3 v(x+rhs.x,y+rhs.y,z+rhs.z);
			return v;
		}

		Vector3	operator-(const Vector3 &rhs) {
			Vector3 v(x-rhs.x,y-rhs.y,z-rhs.z);
			return v;
		}

		double& operator[](const int &ind) {
			return ((ind > 0) ? ((ind > 1) ? z : y) : x);
		}

		Vector3 operator*(const double &rhs) {
			Vector3 v(x*rhs,y*rhs,z*rhs);
			return v;
		}

		Vector3 operator*(const Vector3 &rhs) {
			Vector3 v(x*rhs.x,y*rhs.y,z*rhs.z);
			return v;
		}

		Vector3& operator=(const Vector3 &rhs) {
			x = rhs.x; y = rhs.y; z = rhs.z;
			return *this;
		}

		Vector3& operator=(const double &rhs) {
			x = rhs; y = rhs; z = rhs;
			return *this;
		}
};

std::ostream& operator<<(std::ostream &os, const Vector3 &v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	return os;
}

namespace vector3 {

	Vector3 i() {
		Vector3 v(1.0,0.0,0.0);
		return v;
	}

	Vector3 j() {
		Vector3 v(0.0,1.0,0.0);
		return v;
	}

	Vector3 k() {
		Vector3 v(0.0,0.0,1.0);
		return v;
	}

}

#endif
