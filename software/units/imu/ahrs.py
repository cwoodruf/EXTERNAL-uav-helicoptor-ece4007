'''
Copyright (c) 2012 Joey Yore

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
'''


import math

GYRO_SENSOR_ADJUST = 6.575
ACCEL_SENSOR_ADJUST = 1
MAGN_SENSOR_ADJUST = 1

class AHRS():

	q0 = 1.0
	q1 = 0.0
	q2 = 0.0
	q3 = 0.0

	def __init__(self,beta=0.1,sample_freq=512.0):
		self.beta = beta
		self.sample_freq = sample_freq

	def update(self,gx,gy,gz,ax,ay,az,mx,my,mz):
		q0 = self.q0; q1 = self.q1; q2 = self.q2; q3 = self.q3

		gx *= GYRO_SENSOR_ADJUST; gy *= GYRO_SENSOR_ADJUST; gz *= GYRO_SENSOR_ADJUST;
		ax *= ACCEL_SENSOR_ADJUST; ay *= ACCEL_SENSOR_ADJUST; az *= ACCEL_SENSOR_ADJUST;
		mx *= MAGN_SENSOR_ADJUST; my *= MAGN_SENSOR_ADJUST; mz *= MAGN_SENSOR_ADJUST;


		if mx == 0.0 and my == 0.0 and mz == 0.0:
			return self.updateIMU(gx,gy,gz,ax,ay,az)

		qDot1 = 0.5*(-q1*gx - q2*gy - q3*gz)
		qDot2 = 0.5*( q0*gx + q2*gz - q3*gy)
		qDot3 = 0.5*( q0*gy - q1*gz + q3*gx)
		qDot4 = 0.5*( q0*gz + q1*gy - q2*gx) 

		if not (ax == 0.0 and ay == 0.0 and az == 0.0):
			rmag = 1/math.sqrt(ax*ax + ay*ay + az*az)
			ax *= rmag; ay *= rmag; az *= rmag

			rmag = 1/math.sqrt(mx*mx + my*my + mz*mz)
			mx *= rmag; my *= rmag; mz *= rmag

			_2q0mx = 2.0 * q0 * mx;
			_2q0my = 2.0 * q0 * my;
			_2q0mz = 2.0 * q0 * mz;
			_2q1mx = 2.0 * q1 * mx;
			_2q0 = 2.0 * q0;
			_2q1 = 2.0 * q1;
			_2q2 = 2.0 * q2;
			_2q3 = 2.0 * q3;
			_2q0q2 = 2.0 * q0 * q2;
			_2q2q3 = 2.0 * q2 * q3;
			q0q0 = q0 * q0;
			q0q1 = q0 * q1;
			q0q2 = q0 * q2;
			q0q3 = q0 * q3;
			q1q1 = q1 * q1;
			q1q2 = q1 * q2;
			q1q3 = q1 * q3;
			q2q2 = q2 * q2;
			q2q3 = q2 * q3;
			q3q3 = q3 * q3;
			
			hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3
			hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3
			_2bx = math.sqrt(hx * hx + hy * hy)
			_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
			_4bx = 2.0 * _2bx
			_4bz = 2.0 * _2bz

			s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - ax) + _2q1 * (2.0 * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
			s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - ax) + _2q0 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q1 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + _2bz * q3 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
			s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - ax) + _2q3 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q2 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)
			s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - ax) + _2q2 * (2.0 * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz)

			rmag = math.sqrt(s0*s0 + s1*s1 + s2*s2 + s3*s3)
			if rmag != 0.0:
				rmag = 1/rmag
			s0 *= rmag; s1 *= rmag; s2 *= rmag; s3 *= rmag

			qDot1 -= self.beta*s0
			qDot2 -= self.beta*s1
			qDot3 -= self.beta*s2
			qDot4 -= self.beta*s3

		q0 += qDot1 * (1/self.sample_freq)
		q1 += qDot2 * (1/self.sample_freq)
		q2 += qDot3 * (1/self.sample_freq)
		q3 += qDot4 * (1/self.sample_freq)

		rmag = 1/math.sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3)
		q0 *= rmag; q1 *= rmag; q2 *= rmag; q3 *= rmag

		self.q0 = q0; self.q1 = q1; self.q2 = q2; self.q3 = q3
		return (self.q0,self.q1,self.q2,self.q3)


	def updateIMU(self,gx,gy,gz,ax,ay,az):
		q0 = self.q0; q1 = self.q1; q2 = self.q2; q3 = self.q3

		qDot1 = 0.5*(-q1*gx - q2*gy - q3*gz)
		qDot2 = 0.5*( q0*gx + q2*gz - q3*gy)
		qDot3 = 0.5*( q0*gy - q1*gz + q3*gx)
		qDot4 = 0.5*( q0*gz + q1*gy - q2*gx) 

		if not (ax == 0.0 and ay == 0.0 and az == 0.0):
			rmag = 1/math.sqrt(ax*ax + ay*ay + az*az)
			ax *= rmag; ay *= rmag; az *= rmag

			_2q0 = 2.0 * q0;
			_2q1 = 2.0 * q1;
			_2q2 = 2.0 * q2;
			_2q3 = 2.0 * q3;
			_4q0 = 4.0 * q0;
			_4q1 = 4.0 * q1;
			_4q2 = 4.0 * q2;
			_8q1 = 8.0 * q1;
			_8q2 = 8.0 * q2;
			q0q0 = q0 * q0;
			q1q1 = q1 * q1;
			q2q2 = q2 * q2;
			q3q3 = q3 * q3;

			s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
			s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
			s2 = 4.0 * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
			s3 = 4.0 * q1q1 * q3 - _2q1 * ax + 4.0 * q2q2 * q3 - _2q2 * ay;

			rmag = math.sqrt(s0*s0 + s1*s1 + s2*s2 + s3*s3)
			if rmag != 0.0:
				rmag = 1/rmag
			s0 *= rmag; s1 *= rmag; s2 *= rmag; s3 *= rmag

			qDot1 -= self.beta*s0
			qDot2 -= self.beta*s1
			qDot3 -= self.beta*s2
			qDot4 -= self.beta*s3

		q0 += qDot1 * (1/self.sample_freq)
		q1 += qDot2 * (1/self.sample_freq)
		q2 += qDot3 * (1/self.sample_freq)
		q3 += qDot4 * (1/self.sample_freq)

		rmag = 1/math.sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3)
		self.q0 = q0*rmag; self.q1 = q1*rmag 
		self.q2 = q2*rmag; self.q3 = q3*rmag

		return (self.q0,self.q1,self.q2,self.q3)

	def rad2deg(self,rad):
		return rad*180.0/math.pi

	def deg2rad(self,deg):
		return deg*math.pi/180.0

	def get_conj(self):
		return (self.q0,-self.q1,-self.q2,-self.q3)

	def toEuler(self,q):
		R11 = 2*q[0]*q[0]-1+2*q[1]*q[1]
		R21 = 2*(q[1]*q[2]-q[0]*q[3])
		R31 = 2*(q[1]*q[3]+q[0]*q[2])
		R32 = 2*(q[2]*q[3]-q[0]*q[1])
		R33 = 2*q[0]*q[0]-1+2*q[3]*q[3]

		phi = math.atan2(R32,R33)
		theta = -math.atan(R31/math.sqrt(1-R31*R31))
		psi = math.atan2(R21,R11)

		return (phi,theta,psi)

	def transform(self):
		x,y,z = self.toEuler(self.get_conj())
		return (self.rad2deg(x),self.rad2deg(y),self.rad2deg(z))



class MARG():

	def __init__(self,rate=0.0,error=0.0,drift=0.0):

		self.rate = self.deltat = rate
		self.error = error
		self.drift = drift

		self.first_update = 0

		self.AEq_1 = 1; self.AEq_2 = 0; self.AEq_3 = 0; self.AEq_4 = 0
		self.SEq_1 = 1; self.SEq_2 = 0; self.SEq_3 = 0; self.SEq_4 = 0

		self.b_x = 1; self.b_z = 0

		self.w_bx = 0; self.w_by = 0; self.w_bz = 0
		
		#self.beta = math.sqrt(3.0/4.0) * (math.pi * (self.error/180))
		self.beta = 0.1
		self.zeta = math.sqrt(3.0/4.0) * (math.pi * (self.drift/180))


	def update(self,w_x,w_y,w_z,a_x,a_y,a_z,m_x,m_y,m_z):

		halfSEq_1 = 0.5 * self.SEq_1
		halfSEq_2 = 0.5 * self.SEq_2
		halfSEq_3 = 0.5 * self.SEq_3
		halfSEq_4 = 0.5 * self.SEq_4
		twoSEq_1 = 2.0 * self.SEq_1
		twoSEq_2 = 2.0 * self.SEq_2
		twoSEq_3 = 2.0 * self.SEq_3
		twoSEq_4 = 2.0 * self.SEq_4
		twob_x = 2.0 * self.b_x
		twob_z = 2.0 * self.b_z
		twob_xSEq_1 = 2.0 * self.b_x * self.SEq_1
		twob_xSEq_2 = 2.0 * self.b_x * self.SEq_2
		twob_xSEq_3 = 2.0 * self.b_x * self.SEq_3
		twob_xSEq_4 = 2.0 * self.b_x * self.SEq_4
		twob_zSEq_1 = 2.0 * self.b_z * self.SEq_1
		twob_zSEq_2 = 2.0 * self.b_z * self.SEq_2
		twob_zSEq_3 = 2.0 * self.b_z * self.SEq_3
		twob_zSEq_4 = 2.0 * self.b_z * self.SEq_4
		SEq_1SEq_2 = 0
		SEq_1SEq_3 = self.SEq_1 * self.SEq_3
		SEq_1SEq_4 = 0
		SEq_2SEq_3 = 0
		SEq_2SEq_4 = self.SEq_2 * self.SEq_4
		SEq_3SEq_4 = 0
		twom_x = 2.0 * m_x
		twom_y = 2.0 * m_y
		twom_z = 2.0 * m_z

		norm = math.sqrt(a_x * a_x + a_y * a_y + a_z * a_z)
		if norm == 0.0:
			a_x = 0; a_y = 0; a_z = 0
		else:
			a_x /= norm;  a_y /= norm;  a_z /= norm

		norm = math.sqrt(m_x * m_x + m_y * m_y + m_z * m_z)
		if norm == 0.0:
			m_x = 0; m_y = 0; m_z = 0
		else:
			m_x /= norm;  m_y /= norm;  m_z /= norm


		f_1 = twoSEq_2 * self.SEq_4 - twoSEq_1 * self.SEq_3 - a_x
		f_2 = twoSEq_1 * self.SEq_2 + twoSEq_3 * self.SEq_4 - a_y
		f_3 = 1.0 - twoSEq_2 * self.SEq_2 - twoSEq_3 * self.SEq_3 - a_z
		f_4 = twob_x * (0.5 - self.SEq_3 * self.SEq_3 - self.SEq_4 * self.SEq_4) + twob_z * (SEq_2SEq_4 - SEq_1SEq_3) - m_x
		f_5 = twob_x * (self.SEq_2 * self.SEq_3 - self.SEq_1 * self.SEq_4) + twob_z * (self.SEq_1 * self.SEq_2 + self.SEq_3 * self.SEq_4) - m_y
		f_6 = twob_x * (SEq_1SEq_3 + SEq_2SEq_4) + twob_z * (0.5 - self.SEq_2 * self.SEq_2 - self.SEq_3 * self.SEq_3) - m_z
		J_11or24 = twoSEq_3
		J_12or23 = 2.0 * self.SEq_4
		J_13or22 = twoSEq_1
		J_14or21 = twoSEq_2
		J_32 = 2.0 * J_14or21
		J_33 = 2.0 * J_11or24
		J_41 = twob_zSEq_3
		J_42 = twob_zSEq_4
		J_43 = 2.0 * twob_xSEq_3 + twob_zSEq_1
		J_44 = 2.0 * twob_xSEq_4 - twob_zSEq_2
		J_51 = twob_xSEq_4 - twob_zSEq_2
		J_52 = twob_xSEq_3 + twob_zSEq_1
		J_53 = twob_xSEq_2 + twob_zSEq_4
		J_54 = twob_xSEq_1 - twob_zSEq_3
		J_61 = twob_xSEq_3
		J_62 = twob_xSEq_4 - 2.0 * twob_zSEq_2
		J_63 = twob_xSEq_1 - 2.0 * twob_zSEq_3
		J_64 = twob_xSEq_2

		SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1 - J_41 * f_4 - J_51 * f_5 + J_61 * f_6
		SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3 + J_42 * f_4 + J_52 * f_5 + J_62 * f_6
		SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1 - J_43 * f_4 + J_53 * f_5 + J_63 * f_6
		SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2 - J_44 * f_4 - J_54 * f_5 + J_64 * f_6

		norm = math.sqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4)
		if norm == 0:
			SEqHatDot_1 = 0
			SEqHatDot_2 = 0
			SEqHatDot_3 = 0
			SEqHatDot_4 = 0
		else:
			SEqHatDot_1 = SEqHatDot_1 / norm
			SEqHatDot_2 = SEqHatDot_2 / norm
			SEqHatDot_3 = SEqHatDot_3 / norm
			SEqHatDot_4 = SEqHatDot_4 / norm

		w_err_x = twoSEq_1 * SEqHatDot_2 - twoSEq_2 * SEqHatDot_1 - twoSEq_3 * SEqHatDot_4 + twoSEq_4 * SEqHatDot_3
		w_err_y = twoSEq_1 * SEqHatDot_3 + twoSEq_2 * SEqHatDot_4 - twoSEq_3 * SEqHatDot_1 - twoSEq_4 * SEqHatDot_2
		w_err_z = twoSEq_1 * SEqHatDot_4 - twoSEq_2 * SEqHatDot_3 + twoSEq_3 * SEqHatDot_2 - twoSEq_4 * SEqHatDot_1

		self.w_bx += w_err_x * self.deltat * self.zeta
		self.w_by += w_err_y * self.deltat * self.zeta
		self.w_bz += w_err_z * self.deltat * self.zeta
		w_x -= self.w_bx
		w_y -= self.w_by
		w_z -= self.w_bz

		SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z
		SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y
		SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x
		SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x

		self.SEq_1 += (SEqDot_omega_1 - (self.beta * SEqHatDot_1)) * self.deltat
		self.SEq_2 += (SEqDot_omega_2 - (self.beta * SEqHatDot_2)) * self.deltat
		self.SEq_3 += (SEqDot_omega_3 - (self.beta * SEqHatDot_3)) * self.deltat
		self.SEq_4 += (SEqDot_omega_4 - (self.beta * SEqHatDot_4)) * self.deltat
		print SEqDot_omega_1,SEqDot_omega_2,SEqDot_omega_3,SEqDot_omega_4

		norm = math.sqrt(self.SEq_1 * self.SEq_1 + self.SEq_2 * self.SEq_2 + self.SEq_3 * self.SEq_3 + self.SEq_4 * self.SEq_4)
		if norm == 0:
			self.SEq_1 = 0
			self.SEq_2 = 0
			self.SEq_3 = 0
			self.SEq_4 = 0
		else:
			self.SEq_1 /= norm
			self.SEq_2 /= norm
			self.SEq_3 /= norm
			self.SEq_4 /= norm

		SEq_1SEq_2 = self.SEq_1 * self.SEq_2
		SEq_1SEq_3 = self.SEq_1 * self.SEq_3
		SEq_1SEq_4 = self.SEq_1 * self.SEq_4
		SEq_3SEq_4 = self.SEq_3 * self.SEq_4
		SEq_2SEq_3 = self.SEq_2 * self.SEq_3
		SEq_2SEq_4 = self.SEq_2 * self.SEq_4

		h_x = twom_x * (0.5 - self.SEq_3 * self.SEq_3 - self.SEq_4 * self.SEq_4) + twom_y * (SEq_2SEq_3 - SEq_1SEq_4) + twom_z * (SEq_2SEq_4 + SEq_1SEq_3)

		h_y = twom_x * (SEq_2SEq_3 + SEq_1SEq_4) + twom_y * (0.5 - self.SEq_2 * self.SEq_2 - self.SEq_4 * self.SEq_4) + twom_z * (SEq_3SEq_4 - SEq_1SEq_2)
		h_z = twom_x * (SEq_2SEq_4 - SEq_1SEq_3) + twom_y * (SEq_3SEq_4 + SEq_1SEq_2) + twom_z * (0.5 - self.SEq_2 * self.SEq_2 - self.SEq_3 * self.SEq_3)

		self.b_x = math.sqrt((h_x * h_x) + (h_y * h_y))
		self.b_z = h_z

		if self.first_update == 0:
			self.AEq_1 = self.SEq_1;
			self.AEq_2 = self.SEq_2;
			self.AEq_3 = self.SEq_3;
			self.AEq_4 = self.SEq_4;
			self.first_update = 1;
	

		self.to_euler()


	def to_euler(self):
		ESq_1 = self.SEq_1
		ESq_2 = -self.SEq_2
		ESq_3 = -self.SEq_3
		ESq_4 = -self.SEq_4

		ASq_1 = ESq_1 * self.AEq_1 - ESq_2 * self.AEq_2 - ESq_3 * self.AEq_3 - ESq_4 * self.AEq_4
		ASq_2 = ESq_1 * self.AEq_2 + ESq_2 * self.AEq_1 + ESq_3 * self.AEq_4 - ESq_4 * self.AEq_3
		ASq_3 = ESq_1 * self.AEq_3 - ESq_2 * self.AEq_4 + ESq_3 * self.AEq_1 + ESq_4 * self.AEq_2
		ASq_4 = ESq_1 * self.AEq_4 + ESq_2 * self.AEq_3 - ESq_3 * self.AEq_2 + ESq_4 * self.AEq_1

		self.phi = math.atan2(2 * ASq_3 * ASq_4 - 2 * ASq_1 * ASq_2, 2 * ASq_1 * ASq_1 + 2 * ASq_4 * ASq_4 - 1)
		self.theta = math.asin(2 * ASq_2 * ASq_3 - 2 * ASq_1 * ASq_3)
		self.psi = math.atan2(2 * ASq_2 * ASq_3 - 2 * ASq_1 * ASq_4, 2 * ASq_1 * ASq_1 + 2 * ASq_2 * ASq_2 - 1)

	def get_orientation(self):
		return (self.phi,self.theta,self.psi)

	def reset(self):
		self.first_update = 0

		self.AEq_1 = 1; self.AEq_2 = 0; self.AEq_3 = 0; self.AEq_4 = 0
		self.SEq_1 = 1; self.SEq_2 = 0; self.SEq_3 = 0; self.SEq_4 = 0

		self.b_x = 1; self.b_z

		self.w_bx = 0; self.w_by = 0; self.w_bz = 0


class MARG2():

	def __init__(self,period=1/256):

		self.period = period
		self.quaternion = [1, 0, 0, 0]
		self.beta = 0


	def update(self,gyro,accel,magn):
		q = self.quaternion

		norm = math.sqrt(accel[0]*accel[0]+accel[1]*accel[1]+accel[2]*accel[2])
		if norm == 0:
			accel[0] = 0; accel[1] = 0; accel[2] = 0
		else:
			accel[0] /= norm; accel[1] /= norm;	accel[2] /= norm

		norm = math.sqrt(magn[0]*magn[0]+magn[1]*magn[1]+magn[2]*magn[2])
		if norm == 0:
			magn[0] = 0; magn[1] = 0; magn[2] = 0
		else:
			magn[0] /= norm; magn[1] /= norm; magn[2] /= norm

		h = quatern_prod(q,quatern_prod([0, magn[0], magn[1], magn[2]], quatern_conj(q)))

		norm = math.sqrt(h[1]*h[1]+h[2]*h[2]+h[3]*h[3])
		if norm == 0:
			h[1] = 0; h[2] = 0; h[3] = 0
		else:
			h[1] /= norm; h[2] /= norm; h[3] /= norm
		
		b = [0, h[1], h[2], h[3]]

		F = [
			2*(q[1]*q[3] - q[0]*q[2]) - accel[0],
			2*(q[0]*q[1] + q[2]*q[3]) - accel[1],
			2*(0.5 - q[1]*q[1] - q[2]*q[2]) - accel[2],
			2*b[1]*(0.5 - q[2]*q[2] - q[3]*q[3]) + 2*b[3]*(q[1]*q[3]) - magn[0],
			2*b[1]*(q[1]*q[2] - q[0]*q[3]) + 2*b[3]*(q[0]*q[1] + q[2]*q[3]) - magn[1],
			2*b[1]*(q[0]*q[2] + q[1]*q[3]) + 2*b[3]*(0.5 - q[1]*q[1] - q[2]*q[2]) - magn[2]
		]

		J = [
			-2*q[2], 2*q[3], -2*q[0], 2*q[1],
			2*q[1], 2*q[0], 2*q[3], 2*q[2],
			0,-4*q[1],-4*q[2],0,
			-2*b[3]*q[2],2*b[3]*q[3],-4*b[1]*q[2]-2*b[3]*q[0],-4*b[1]*b[3]+2*b[3]*q[1],
			-2*b[1]*q[3]+2*b[3]*q[1],2*b[1]*q[2]+2*b[3]*q[0],2*b[1]*q[1]+2*b[3]*q[3],-2*b[1]*q[0]+2*b[3]*q[3],
			2*b[1]*q[2],2*b[1]*q[3]-4*b[3]*q[1],2*b[1]*q[0]-4*b[3]*q[2],2*b[1]*q[1]
		]


		step = F[0]*J[0] + F[1]*J[1] + F[2]*J[2] + F[3]*J[3] + F[4]*J[4] + F[5]*J[5]

		qDot = [0, 0, 0, 0]
		t = quatern_prod(q,[0,gyro[0],gyro[1],gyro[2]])
		qDot[0] = 0.5 * t[0] - self.beta*step
		qDot[1] = 0.5 * t[1] - self.beta*step
		qDot[2] = 0.5 * t[2] - self.beta*step
		qDot[3] = 0.5 * t[3] - self.beta*step
	
		q[0] += qDot[0] * self.period
		q[1] += qDot[1] * self.period
		q[2] += qDot[2] * self.period
		q[3] += qDot[3] * self.period

		norm = math.sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3])
		if norm == 0:
			q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 0
		else:
			q[0] /= norm; q[1] /= norm;	q[2] /= norm; q[3] /= norm

		self.quaternion = q

	def updateIMU(self,gyro,accel):

		q = self.quaternion

		norm = math.sqrt(accel[0]*accel[0]+accel[1]*accel[1]+accel[2]*accel[2])
		if norm == 0:
			accel[0] = 0; accel[1] = 0; accel[2] = 0
		else:
			accel[0] /= norm; accel[1] /= norm;	accel[2] /= norm

		F = [
			2*(q[1]*q[3] - q[0]*q[2]) - accel[0],
			2*(q[0]*q[1] + q[2]*q[3]) - accel[1],
			2*(0.5 - q[1]*q[1] - q[2]*q[2]) - accel[2]
		]

		J = [
			-2*q[2],  2*q[3], -2*q[0], 2*q[1],
			 2*q[1],  2*q[0],  2*q[3], 2*q[2],
			      0, -4*q[1], -4*q[2], 0
		]

		step = [
			[J[0]*F[0], J[0]*F[1], J[0]*F[2]],
			[J[1]*F[0], J[1]*F[1], J[1]*F[2]],
			[J[2]*F[0], J[2]*F[1], J[2]*F[2]],
			[J[3]*F[0], J[3]*F[1], J[3]*F[2]],
			[J[4]*F[0], J[4]*F[1], J[4]*F[2]],
			[J[5]*F[0], J[5]*F[1], J[5]*F[2]],
			[J[6]*F[0], J[6]*F[1], J[6]*F[2]],
			[J[7]*F[0], J[7]*F[1], J[7]*F[2]],
			[J[8]*F[0], J[8]*F[1], J[8]*F[2]],
			[J[9]*F[0], J[9]*F[1], J[9]*F[2]],
			[J[10]*F[0], J[10]*F[1], J[10]*F[2]],
			[J[11]*F[0], J[11]*F[1], J[11]*F[2]],
		]


	def transform(self):
		q = quatern_conj(self.quaternion)
		return quatern_2_euler(q)



def quatern_prod(a,b):
	r = [0, 0, 0, 0]

	r[0] = a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3]
	r[1] = a[0]*b[1] + a[1]*b[0] + a[2]*b[3] - a[3]*b[2]
	r[2] = a[0]*b[2] - a[1]*b[3] + a[2]*b[0] - a[3]*b[1]
	r[3] = a[0]*b[3] + a[1]*b[2] - a[2]*b[1] - a[3]*b[0]

	return r

def quatern_conj(a):
	return [a[0], -a[1], -a[2], -a[3]]

def quatern_2_euler(q):

	R11 = 2*q[0]*q[0] + 2*q[1]*q[1]
	R21 = 2*(q[1]*q[2] - q[0]*q[3])
	R31 = 2*(q[1]*q[3] + q[0]*q[2])
	R32 = 2*(q[2]*q[3] - q[0]*q[1])
	R33 = 2*q[0]*q[0] - 1 + 2*q[3]*q[3]

	phi = math.atan2(R32,R33)
	theta = -math.atan(R31 / math.sqrt(1-R31*R31))
	psi = math.atan2(R21,R11)

	return phi,theta,psi
