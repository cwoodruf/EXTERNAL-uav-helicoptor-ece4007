import math

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

		gx *= 6; gy *= 6; gz *=6;
		mx /= 100; my /= 100; mz /=100;

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
