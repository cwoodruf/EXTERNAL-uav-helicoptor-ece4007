#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3.h"
#include <cmath>
#include <iostream>

class Quaternion {

	private:
		Vector3 v;
		double s;

		friend Quaternion operator*(const double &lhs,const Quaternion &rhs);
		friend std::ostream& operator<<(std::ostream &os, const Quaternion &q);
	public:

		Quaternion() : v(1,0,0), s(0.0) {
		}

		Quaternion(double s,Vector3 v) : v(v), s(s) {
		}

		Quaternion(const Quaternion &cpy) {
			v = cpy.v;
			s = cpy.s;
		}

		Vector3 rotate(Vector3 v) {
			Quaternion vq(0.0f,v);
			Quaternion t = (*this) * vq * conjugate();

			return Vector3(t.v);
		}

		double norm() {
			return sqrt(s*s + v.squared_norm());
		}

		Quaternion conjugate() {
			return Quaternion(s,v*-1);
		}

		Matrix3 rotation_matrix() {
			double x = v[0], y = v[1], z = v[2];
			return Matrix3(
				1-2*(y*y+z*z),	2*x*y-2*s*z,	2*s*y+2*x*z, 
				2*x*y+2*s*z,	1-2*(x*x+z*z),	-2*s*x+2*y*z,
				-2*s*y+2*x*z,	2*s*x+2*y*z,	1-2*(x*x+y*y)
			);
		}

		Quaternion normalize() {
			double l = sqrt(s*s + v.squared_norm());
			return Quaternion(
				s/l,
				Vector3(v[0]/l,v[1]/l,v[2]/l)
			);
		}

		double dot(Quaternion q) {
			return  v.dot(q.v) + s*q.s;
		}

		Quaternion interpolate(Quaternion qb, double t) {
			Quaternion qa(*this);
			double theta = acos(qa.dot(qb)/180*M_PI);

			if(std::abs(theta) < 1e-7) {
				return *this;
			}
			return qa*sin((1-t)*theta) + qb*sin(t*theta)*(1/sin(theta));
		}

		Vector3 angular_velocity(Quaternion q1) {
			Quaternion q0(*this);

			Quaternion q0inv= Quaternion(q0.s,q0.v*-1)*(1/q0.dot(q0));
			Quaternion r = q0inv*q1;

			double so = r.v.norm();
			if(std::abs(so) < 1e-7) {
				return Vector3();
			}

			Vector3 n = r.v * (1/so);
			double omegaNorm = asin(so/180*M_PI)*2;
			return n*omegaNorm;
		}	

		Quaternion operator*(const Quaternion &rhs) {
			Quaternion n(rhs);
			return Quaternion(
				s*n.s-v.dot(n.v),
				n.v*s + v*n.s + v.cross(n.v)
			);
		}

		Quaternion operator*(const double &rhs) {
			return Quaternion(s*rhs,v*rhs);
		}

		Quaternion operator+(const Quaternion &rhs) {
			return Quaternion(s+rhs.s,v+rhs.v);
		}
};

namespace quaternion {

	Quaternion rotation(double theta, Vector3 n) {
		return Quaternion(
			cos(theta/2.0f),
			n*sin(theta/2.0f)
		);
	}

	Quaternion orientation(Vector3 unit) {
		Vector3 i = vector3::i();
		double theta = acos(i.dot(unit)/180*M_PI);
		Vector3 v = i.cross(unit);
		return rotation(theta,v);
	}	
}

Quaternion operator*(const double &lhs,const Quaternion &rhs) {
	Vector3 v(rhs.v);
	return Quaternion(rhs.s*lhs,v*lhs);
}

std::ostream& operator<<(std::ostream &os, const Quaternion &q) {
	os << "[" << q.s << "," << q.v << "]";
	return os;
}

#endif
