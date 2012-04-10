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

#include "quaternion.h"
#include "vector3.h"
#include <math.h>

class AHRS {

    private:
        double period;
        double beta;
        Quaternion q;

    public:
        AHRS(double oPeriod=1.0/512.0,double oBeta=0.1) : 
        period(oPeriod), beta(oBeta) {
            q = Quaternion(1,Vector3(0,0,0));
        }

        void update(Vector3 gyro, Vector3 accel, Vector3 magn) {
            if((magn[0]==0.0) && (magn[1]==0.0) && (magn[2]==0.0)) {
                this->update(gyro,accel);
                return;
            }

            Quaternion qDot(
                0.5*(-q[1]*gyro[0] - q[2]*gyro[1] - q[3]*gyro[2]),
                Vector3(
                    0.5*( q[0]*gyro[0] + q[2]*gyro[2] - q[3]*gyro[1]),
                    0.5*( q[0]*gyro[1] - q[1]*gyro[2] + q[3]*gyro[0]),
                    0.5*( q[0]*gyro[2] + q[1]*gyro[1] - q[2]*gyro[0])
                )
            );

            if(!((accel[0]==0.0) && (accel[1]==0.0) && (accel[2]==0.0))) {
                accel = accel.normalize();
                magn = magn.normalize();

                //TODO: Finish this
            }
        }

        void update(Vector3 gyro, Vector3 accel) {

            Quaternion qDot(
                0.5*(-q[1]*gyro[0] - q[2]*gyro[1] - q[3]*gyro[2]),
                Vector3(
                    0.5*( q[0]*gyro[0] + q[2]*gyro[2] - q[3]*gyro[1]),
                    0.5*( q[0]*gyro[1] - q[1]*gyro[2] + q[3]*gyro[0]),
                    0.5*( q[0]*gyro[2] + q[1]*gyro[1] - q[2]*gyro[0])
                )
            );

            if(!((accel[0]==0.0) && (accel[1]==0.0) && (accel[2]==0.0))) {

                accel = accel.normalize();

                double _2q0 = 2.0*q[0];
                double _2q1 = 2.0*q[1];
                double _2q2 = 2.0*q[2];
                double _2q3 = 2.0*q[3];
                double _4q0 = 2.0*q[0];
                double _4q1 = 2.0*q[1];
                double _4q2 = 2.0*q[2];
                double _8q1 = 2.0*q[1];
                double _8q2 = 2.0*q[2];
                double q0q0 = q[0]*q[0];
                double q1q1 = q[1]*q[1];
                double q2q2 = q[2]*q[2];
                double q3q3 = q[3]*q[3];

                Quaternion s(
                    _4q0*q2q2 + _2q2*accel[0] + _4q0*q1q1 - _2q1*accel[1],
                    Vector3(
                        _4q1*q3q3 - _2q3*accel[0] + 4.0*q0q0*q[1] - _2q0*accel[1] - _4q1 + _8q1*q1q1 + _8q1*q2q2 + _4q1*accel[2],
                        4.0*q0q0*q[2] + _2q0*accel[0] + _4q2*q3q3 - _2q3*accel[1] - _4q2 + _8q2*q1q1 + _8q2*q2q2 + _4q2*accel[2],
                        4.0*q1q1*q[3] - _2q1*accel[0] + 4.0*q2q2*q[3] - _2q2*accel[1]
                    )
                );
                s.normalize();

                qDot = qDot + -1.0*(beta*s);
            }

            q = q + (qDot*period);
            q.normalize();
        }

        Vector3 get_angles() {
            return ((q.conjugate()).Euler_ZYX()) * (180.0/M_PI);
        }
};

#endif
