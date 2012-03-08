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

#ifndef AHRS_H
#define AHRS_H

#include <math.h>
#include "vector3.h"
#include "quaternion.h"
#include "../util.h"

class AHRS {

	private:
		double Kp;		//Proportional Gain
		double t;		//Sample Period
		Quaternion Q;	//Position Quaternion
		
		friend std::ostream& operator<<(std::ostream &os, const AHRS &q);

	public:

		AHRS() : Kp(0.05), t(0.1) {
		}

		AHRS(double oKp,double ot) : Kp(oKp), t(ot) {
		}
		
		double get_gain() {
		 	return Kp;   
		}
		
		double get_period() {
		 	return t;   
		}
		
		void set_gain(double oKp) {
		 	Kp = oKp;   
		}
		
		void set_period(double ot) {
			t = ot;
		}
		
		//Uses the MadgwickAHRS algorithm
		Quaternion update(Vector3 accel, Vector3 gyro, Vector3 magn) {
		 
			//Magnetometer data invalid, so don't account for it
			if(magn == Vector3(0.0,0.0,0.0)) {
			 	return update(accel,gyro);   
			}   
			
			//Rate of change of the Quaternion based on the Gyro
			double qDot1 = 0.5 * (-Q[1] * gyro[0] - Q[2] * gyro[1] - Q[3] * gyro[2]);
			double qDot2 = 0.5 * ( Q[0] * gyro[0] + Q[2] * gyro[2] - Q[3] * gyro[1]);
			double qDot3 = 0.5 * ( Q[0] * gyro[1] - Q[2] * gyro[2] + Q[3] * gyro[0]);
			double qDot4 = 0.5 * ( Q[0] * gyro[2] + Q[1] * gyro[1] - Q[2] * gyro[0]);
			
			//Compute feedback only if accel is valid
			if(accel != Vector(0.0,0.0,0.0)) {
			
				//Normalize measurements
				Vector3 a = accel.normalize();
				Vector3 m = magn.normalize();
			
				//Precalculations
				double _2q0mx = 2.0 * Q[0] * m[0];
				double _2q0my = 2.0 * Q[0] * m[1];
				double _2q0mz = 2.0 * Q[0] * m[2];
				double _2q1mx = 2.0 * Q[0] * m[0];
				double _2q0 = 2.0 * Q[0];
				double _2q1 = 2.0 * Q[1];
				double _2q2 = 2.0 * Q[2];
				double _2q3 = 2.0 * Q[3];
				double _2q0q2 = 2.0 * Q[0] * Q[2];
				double _2q2q3 = 2.0 * Q[2] * Q[3];
				double q0q0 = Q[0] * Q[0];
				double q0q1 = Q[0] * Q[1];
				double q0q2 = Q[0] * Q[2];
				double q0q3 = Q[0] * Q[3];
				double q1q1 = Q[1] * Q[1];
				double q1q2 = Q[1] * Q[2];
				double q1q3 = Q[1] * Q[3];
				double q2q2 = Q[2] * Q[2];
				double q2q3 = Q[2] * Q[3];
				double q3q3 = Q[3] * Q[3];
			
				// Reference direction of Earth's magnetic field
				double hx = m[0] * q0q0 - _2q0my * Q[3]+ _2q0mz * Q[2]+ m[0] * q1q1 + _2q1 * m[1] * Q[2]+ _2q1 * m[2] * Q[3]- m[0] * q2q2 - m[0] * q3q3;
				double hy = _2q0mx * Q[3]+ m[1] * q0q0 - _2q0mz * Q[1]+ _2q1mx * Q[2]- m[1] * q1q1 + m[1] * q2q2 + _2q2 * m[2] * Q[3]- m[1] * q3q3;
				double _2bx = sqrt(hx * hx + hy * hy);
				double _2bz = -_2q0mx * Q[2]+ _2q0my * Q[1]+ m[2] * q0q0 + _2q1mx * Q[3]- m[2] * q1q1 + _2q2 * m[1] * Q[3]- m[2] * q2q2 + m[2] * q3q3;
				double _4bx = 2.0 * _2bx;
				double _4bz = 2.0 * _2bz;

				// Gradient decent algorithm corrective step
				Quaternion S = Quaternion( 
					-_2q2 * (2.0 * q1q3 - _2q0q2 - a[0]) + _2q1 * (2.0 * q0q1 + _2q2q3 - a[1]) - _2bz * Q[2]* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m[0]) + (-_2bx * Q[3]+ _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m[1]) + _2bx * Q[2]* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m[2]),
					_2q3 * (2.0 * q1q3 - _2q0q2 - a[0]) + _2q0 * (2.0 * q0q1 + _2q2q3 - a[1]) - 4.0f * Q[1]* (1 - 2.0 * q1q1 - 2.0 * q2q2 - a[2]) + _2bz * Q[3]* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m[0]) + (_2bx * Q[2]+ _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m[1]) + (_2bx * Q[3]- _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m[2]),
					-_2q0 * (2.0 * q1q3 - _2q0q2 - a[0]) + _2q3 * (2.0 * q0q1 + _2q2q3 - a[1]) - 4.0f * Q[2]* (1 - 2.0 * q1q1 - 2.0 * q2q2 - a[2]) + (-_4bx * Q[2]- _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m[0]) + (_2bx * Q[1]+ _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m[1]) + (_2bx * Q[0]- _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m[2]),
					_2q1 * (2.0 * q1q3 - _2q0q2 - a[0]) + _2q2 * (2.0 * q0q1 + _2q2q3 - a[1]) + (-_4bx * Q[3]+ _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m[0]) + (-_2bx * Q[0]+ _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m[1]) + _2bx * Q[1]* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m[2])
				).normalize();

				// Apply feedback step
				qDot1 -= 2.0 * Kp * S[0];
				qDot2 -= 2.0 * Kp * S[1];
				qDot3 -= 2.0 * Kp * S[2];
				qDot4 -= 2.0 * Kp * S[3];
			}
			
			// Integrate rate of change of quaternion to yield quaternion
			Q[0] += qDot1 * t;
			Q[1] += qDot2 * t;
			Q[2] += qDot3 * t;
			Q[3] += qDot4 * t;

			// Normalise quaternion - also save for next step and return
			Q = Q.normalize();
			return Q;
		}
		
		//Uses the MadgwickAHRS algorithm
		Quaternion update(Vector3 accel, Vector3 gyro) {
			
			//Rate of change of the Quaternion based on the Gyro
			double qDot1 = 0.5 * (-Q[1] * gyro[0] - Q[2] * gyro[1] - Q[3] * gyro[2]);
			double qDot2 = 0.5 * ( Q[0] * gyro[0] + Q[2] * gyro[2] - Q[3] * gyro[1]);
			double qDot3 = 0.5 * ( Q[0] * gyro[1] - Q[2] * gyro[2] + Q[3] * gyro[0]);
			double qDot4 = 0.5 * ( Q[0] * gyro[2] + Q[1] * gyro[1] - Q[2] * gyro[0]);
			
			//Compute feedback only if accel is valid
			if(accel != Vector(0.0,0.0,0.0)) {
			
				//Normalize measurements
				Vector3 a = accel.normalize();
			
				//Precalculations
				double _2q0 = 2.0 * Q[0];
				double _2q1 = 2.0 * Q[1];
				double _2q2 = 2.0 * Q[2];
				double _2q3 = 2.0 * Q[3];
				double _4q0 = 4.0 * Q[0];
				double _4q1 = 4.0 * Q[1];
				double _4q2 = 4.0 * Q[2];
				double _8q1 = 8.0 * Q[1];
				double _8q2 = 8.0 * Q[2];
				double q0q0 = Q[0] * Q[0];
				double q1q1 = Q[1] * Q[1];
				double q2q2 = Q[2] * Q[2];
				double q3q3 = Q[3] * Q[3];

				// Gradient decent algorithm corrective step
				Quaternion S = Quatnerion(
					_4q0 * q2q2 + _2q2 * a[0] + _4q0 * q1q1 - _2q1 * a[1],
					_4q1 * q3q3 - _2q3 * a[0] + 4.0f * q0q0 * q1 - _2q0 * a[1] - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * a[2],
					4.0f * q0q0 * q2 + _2q0 * a[0] + _4q2 * q3q3 - _2q3 * a[1] - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * a[2],
					4.0f * q1q1 * q3 - _2q1 * a[0] + 4.0f * q2q2 * q3 - _2q2 * a[1]
				).normalize();
			
			
				// Apply feedback step
				qDot1 -= 2.0 * Kp * S[0];
				qDot2 -= 2.0 * Kp * S[1];
				qDot3 -= 2.0 * Kp * S[2];
				qDot4 -= 2.0 * Kp * S[3];
			}
			
			// Integrate rate of change of quaternion to yield quaternion
			Q[0] += qDot1 * t;
			Q[1] += qDot2 * t;
			Q[2] += qDot3 * t;
			Q[3] += qDot4 * t;

			// Normalise quaternion - also save for next step and return
			Q = Q.normalize();
			return Q;
		}

		//In radians
		Vector3 to_angles_radians() {
			return Q.conjugate().Euler_ZYX();
		}

		//In degrees
		Vector3 to_angles_degrees() {
			Vector3 t = Q.conjugate().Euler_ZYX();
	
			return Vector3(
				rad2deg(t[0]),
				rad2deg(t[1]),
				rad2deg(t[2])
			);
		}
};


std::ostream& operator<<(std::ostream &os, const AHRS &q) {
	os << "{ " << q.Kp << ", " << q.t << << ", " << q.Q << " }";
	return os;
}

#endif
