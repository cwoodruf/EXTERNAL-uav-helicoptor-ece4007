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

#ifndef IMU_H
#define IMU_H

#include "sensors/ADXL345.h"
#include "sensors/HMC5883L.h"
#include "sensors/ITG3200.h"
#include "math/ahrs.h"
#include "math/vector3.h"

class IMU {

	private:
		ADXL345 accel;
		ITG3200 gyro;
		HMC5883L magn;
		AHRS ahrs;
		double *cal_a;
		double *cal_g;
		double *cal_m;
		bool CALIBRATED;

	public:
		IMU() {
			CALIBRATED = false;
			cal_a = new double[3];
			cal_g = new double[3];
			cal_m = new double[3];
		}

		IMU(double rate, double beta) : ahrs(rate,beta) {
			CALIBRATED = false;
			cal_a = new double[3];
			cal_g = new double[3];
			cal_m = new double[3];
		}

		IMU(const IMU &cpy) {
			CALIBRATED = cpy.CALIBRATED;
			accel = cpy.accel;
			gyro = cpy.gyro;
			magn = cpy.magn;
			cal_a = new double[3];
			cal_g = new double[3];
			cal_m = new double[3];

			for(int i=0;i<3;++i) {
				cal_a[i] = cpy.cal_a[i];
				cal_g[i] = cpy.cal_g[i];
				cal_m[i] = cpy.cal_m[i];
			}
		}

		~IMU() {
			delete[] cal_a;
			delete[] cal_g;
			delete[] cal_m;
		}


		int calibrate(int samples=1000) {
			accel.set_range(0,false);
		    accel.set_measurement_mode();

			short int ax,ay,az;
			short int gx,gy,gz;
			//short int mx,my,mz;
			double a[3] = {0.0};
			double g[3] = {0.0};
			//double m[3] = {0.0};

			for(int i=0;i<samples;++i) {
				int status = accel.get_data_x_raw(ax); 
				status |= accel.get_data_y_raw(ay); 
				status |= accel.get_data_z_raw(az);
				status |= gyro.get_x_out(gx); 
				status |= gyro.get_y_out(gy); 
				status |= gyro.get_z_out(gz);
				//status |= magn.get_data_x_raw(mx); 
				//status |= magn.get_data_y_raw(my); 
				//status |= magn.get_data_z_raw(mz);

				if(status) {
					return -1;
				}
	
				a[0] += (double)ax; a[1] += (double)ay; a[2] += (double)az;
				g[0] += (double)gx; g[1] += (double)gy; g[2] += (double)gz;
				//m[0] += (double)mx; m[1] += (double)my; m[2] += (double)mz;
			}

			for(int i=0;i<3;++i) {
				cal_a[i] /= 1000.0;
				cal_g[i] /= 1000.0;
				//cal_m[i] /= 1000.0;
			}

			CALIBRATED = true;
			return 0;
		}

		bool isCalibrated() {
			return CALIBRATED;
		}

		int update(Vector3 &angles) {
			if(!CALIBRATED) return -1;
			short int ax,ay,az;
			short int gx,gy,gz;
			//short int mx,my,mz;

			int status = accel.get_data_x_raw(ax); 
			status |= accel.get_data_y_raw(ay); 
			status |= accel.get_data_z_raw(az);
			status |= gyro.get_x_out(gx); 
			status |= gyro.get_y_out(gy); 
			status |= gyro.get_z_out(gz);
			//status |= magn.get_data_x_raw(mx); 
			//status |= magn.get_data_y_raw(my); 
			//status |= magn.get_data_z_raw(mz);

			if(status) {
				return -1;
			}

			Vector3 a(
				(double)(ax-cal_a[0])*0.004,
				(double)(ay-cal_a[1])*0.004,
				(double)(az-cal_a[2])*0.004
			);
			a[0] = (a[0] > 2.0) ? 2.0 : (a[0] < -2.0 ? -2.0 : a[0]);
			a[1] = (a[1] > 2.0) ? 2.0 : (a[1] < -2.0 ? -2.0 : a[1]);
			a[2] = (a[2] > 2.0) ? 2.0 : (a[2] < -2.0 ? -2.0 : a[2]);
	
			Vector3 g(
				((gx-cal_g[0])/14.375)*M_PI/180,
				((gy-cal_g[1])/14.375)*M_PI/180,
				((gz-cal_g[2])/14.375)*M_PI/180
			);
	
			//Vector3 m(mx,my,mz);
	
			ahrs.update(g,a);
			angles = ahrs.get_angles();
			
			return 0;
		}


		IMU& operator=(const IMU &rhs) {
			delete[] cal_a;
			delete[] cal_g;
			delete[] cal_m;

			accel = rhs.accel;
			gyro = rhs.gyro;
			magn = rhs.magn;
			cal_a = new double[3];
			cal_g = new double[3];
			cal_m = new double[3];

			for(int i=0;i<3;++i) {
				cal_a[i] = rhs.cal_a[i];
				cal_g[i] = rhs.cal_g[i];
				cal_m[i] = rhs.cal_m[i];
			}
		}
};

#endif
