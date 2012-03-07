#ifndef AHRS_H
#define AHRS_H

#include "vector3.h"
#include "quaternion.h"

class AHRS {

	private:
		double Kp;
		double Ki;
		double t;
		Quaternion Q;
		Vector3 errInt;

	public:

		AHRS() : Kp(2.0), Ki(0.005), t(0.1) {
		}

		AHRS(double oKp, double oKi, double ot) : Kp(oKp), Ki(oKi), t(ot) {
		}

		Quaternion update(Vector3 accel, Vector3 gyro, Vector3 magn) {

			double norm;

			//Reduces number of repeated operations
			double q0q0 = Q[0]*Q[0];
			double q0q1 = Q[0]*Q[1];
			double q0q2 = Q[0]*Q[2];
			double q0q3 = Q[0]*Q[3];
			double q1q1 = Q[1]*Q[1];
			double q1q2 = Q[1]*Q[2];
			double q1q3 = Q[1]*Q[3];
			double q2q2 = Q[2]*Q[2];
			double q2q3 = Q[2]*Q[3];
			double q3q3 = Q[3]*Q[3];

			//Normalize measurements
			Vector3 na = accel.normalize();
			Vector3 nm = magn.normalize();

			//Compute Reference direction of flux
			Vector3 H(
				2*nm[0]*(0.5 - q2q2 - q3q3) + 2*nm[1]*(q1q2 - q0q3) + 2*nm[2]*(q1q3 + q0q2),
				2*nm[0]*(q1q2 + q0q3) + 2*nm[1]*(0.5 - q1q1 - q3q3) + 2*nm[2]*(q2q3 - q0q1),
				2*nm[0]*(q1q3 - q0q2) + 2*nm[1]*(q2q3 + q0q1) + 2*nm[2]*(0.5 - q1q1 - q2q2)
			);

			double tmp = sqrt((H[0]*H[0]) + (H[1]*H[1]));
			Vector3 B(
				tmp,
				tmp,
				H[2]
			);

			//Estimated direction of gravity and flux (v & w)
			Vector3 v(
				2*(q1q3 - q0q2),
				2*(q0q1 + q2q3),
				q0q0 - q1q1 - q2q2 + q3q3
			);

			Vector3 w(
				2*B[0]*(0.5 - q2q2 - q3q3) + 2*B[2]*(q1q3 - q0q2),
				2*B[0]*(q1q2 - q0q3) + 2*B[2]*(q0q1 + q2q3),
				2*B[0]*(q0q2 + q1q3) + 2*B[2]*(0.5 - q1q1 - q2q2)
			);

			//Error calculation
			//sum of cross of ref dir fields and measured dir
			e = na.cross(v) + nm.cross(w);

			//Integral error sclaed integral gain
			errInt = errInt + e*Ki;

			//Adjusted Gyro Measurements
			Vector ng = gyro + Kp*e + eInt;

			//Integrate quaternion rate and nromalize
			double halft = t/2;
			Q = Quaernion(
				Q[0] + (-Q[1]*ng[0] - Q[2]*ng[1] - Q[3]*ng[2])*halft,
				Q[1] + ( Q[0]*ng[0] + Q[2]*ng[2] - Q[3]*ng[1])*halft,
				Q[1] + ( Q[0]*ng[1] - Q[1]*ng[2] + Q[3]*ng[0])*halft,
				Q[1] + ( Q[0]*ng[2] + Q[1]*ng[1] - Q[2]*ng[0])*halft,
			).normalize();

			return Q;
		}
};

#endif
