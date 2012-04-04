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

#include "mbed.h"
#include "vector3.h"
#include "matrix.h"
#include "quaternion.h"
#include <math.h>

class AHRS {

    private:
        double period;
        Quaternion q;
        double beta;
        Vector3 angles;

        void update_angles() {
            Quaternion qc = q.conjugate();
            angles = qc.Euler_ZYX() * (180/M_PI);
        }

    public:

        AHRS(double oPeriod=0.00390625, Quaternion oQuaternion=Quaternion(1,Vector3(0,0,0)), double oBeta=1) : period(oPeriod), q(oQuaternion), beta(oBeta) {
        }

        void update(Vector3 gyro,Vector3 accel,Vector3 magn) {

            accel = accel.normalize();
            magn = magn.normalize();

            Quaternion h = q * (Quaternion(0,magn) * q.conjugate());
            Quaternion b = Quaternion(0, Vector3(h[1],h[2],h[3]).normalize());

            Matrix F(1,6);
            F(0,0) = 2*(q[1]*q[3] - q[0]*q[2]) - accel[0];
            F(0,1) = 2*(q[0]*q[1] - q[2]*q[3]) - accel[1];
            F(0,2) = 2*(0.5 - q[1]*q[1] - q[2]*q[2]) - accel[2];
            F(0,3) = 2*b[1]*(0.5 - q[2]*q[2] - q[3]*q[3]) + 2*b[3]*(q[1]*q[3] - q[0]*q[2]) - magn[0];
            F(0,4) = 2*b[1]*(q[1]*q[2] - q[0]*q[3]) + 2*b[3]*(q[0]*q[1] + q[2]*q[3]) - magn[1];
            F(0,5) = 2*b[1]*(q[0]*q[2] + q[1]*q[3]) + 2*b[3]*(0.5 - q[1]*q[1] - q[2]*q[2]) - magn[2];

            Matrix J(1,24);
            J(0,0) = -2*q[2];
            J(0,1) = 2*q[3];
            J(0,2) = -2*q[0];
            J(0,3) = 2*q[1];
            J(0,4) = 2*q[1];
            J(0,5) = 2*q[0];
            J(0,6) = 2*q[3];
            J(0,7) = 2*q[2];
            J(0,8) = 0;
            J(0,9) = -4*q[1];
            J(0,10) = -4*q[2];
            J(0,11) = 0;
            J(0,12) = -2*b[3]*q[2];
            J(0,13) = 2*b[3]*q[3];
            J(0,14) = -4*b[1]*q[2] - 2*b[3]*q[0];
            J(0,15) = -4*b[1]*q[3] + 2*b[3]*q[1];
            J(0,16) = -2*b[1]*q[3] + 2*b[3]*q[1];
            J(0,17) = 2*b[1]*q[2] + 2*b[3]*q[0];
            J(0,18) = 2*b[1]*q[1] + 2*b[3]*q[3];
            J(0,19) = -2*b[1]*q[0] + 2*b[3]*q[2];
            J(0,20) = 2*b[1]*q[2];
            J(0,21) = 2*b[1]*q[3] - 4*b[3]*q[1];
            J(0,22) = 2*b[1]*q[0] - 4*b[3]*q[2];
            J(0,23) = 2*b[1]*q[1];

            Matrix Jt = J.transpose();
            Matrix step = Jt * F;
            double s = step.norm();

            Quaternion qDot = 0.5 * q*Quaternion(0,gyro);
            qDot[0] = qDot[0] - beta*s;
            qDot[1] = qDot[1] - beta*s;
            qDot[2] = qDot[2] - beta*s;
            qDot[3] = qDot[3] - beta*s;
            q = q + qDot * period;
            q = q.normalize();

        }

        void update(Vector3 gyro, Vector3 accel) {
            
            accel = accel.normalize();

            Matrix F(1,3);
            F(0,0) = 2*(q[1]*q[3] - q[0]*q[2]) - accel[0];
            F(0,1) = 2*(q[0]*q[1] - q[2]*q[3]) - accel[1];
            F(0,2) = 2*(0.5 - q[1]*q[1] - q[2]*q[2]) - accel[2];
            
            Matrix J(1,12);
            J(0,0) = -2*q[2];
            J(0,1) = 2*q[3];
            J(0,2) = -2*q[0];
            J(0,3) = 2*q[1];
            J(0,4) = 2*q[1];
            J(0,5) = 2*q[0];
            J(0,6) = 2*q[3];
            J(0,7) = 2*q[2];
            J(0,8) = 0;
            J(0,9) = -4*q[1];
            J(0,10) = -4*q[2];
            J(0,11) = 0;
            
            Matrix Jt = J.transpose();
            Matrix step = Jt * F;
            double s = step.norm();

            Quaternion qDot = 0.5 * q*Quaternion(0,gyro);
            qDot[0] = qDot[0] - beta*s;
            qDot[1] = qDot[1] - beta*s;
            qDot[2] = qDot[2] - beta*s;
            qDot[3] = qDot[3] - beta*s;
            q = q + qDot * period;
            q = q.normalize();
           
        }

        Vector3 get_angles() {
            this->update_angles();
            return angles;
        }
};

#endif
