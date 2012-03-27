#!/usr/bin/python

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

import sys
import board
import threading
import serial
import time
import ahrs

PORT = '/dev/ttyACM0'

class Graphics(threading.Thread):

	eng = board.Board(sys.argv,name="IMU")

	def run(self):
		self.eng.run()


class Comm(threading.Thread):

	terminate = False
	comm = serial.Serial(port=PORT)
	algorithm = ahrs.AHRS()
	eng = None

	def run(self):
		comm.open()
		while(not self.terminate):
			s = str(read(size=62))
			v = s.split(',')
			algorithm.update(
				float(v[0]), float(v[1]), float(v[2]),
				float(v[3]), float(v[4]), float(v[5]),
				float(v[6]), float(v[7]), float(v[8])
			)
			x,y,z = algorithm.transform()
			self.update(x,y,z)
		comm.close()

	def kill(self):
		self.terminate = True

	def register_engine(self,eng):
		self.eng = eng

	def update(self,x,y,z):
		if self.eng is not None:
			self.eng.update(x,y,z)

class NoComm(threading.Thread):

	terminate = False
	eng = None
	x = .001
	y = .001
	z = .001
	xf = False
	yf = False
	zf = False
	delim = 1

	def run(self):
		while(not self.terminate):
			if self.xf and self.yf and self.zf:
				pass
			elif self.xf and self.yf:
				if self.z == 90.001:
					self.delim = -1
					self.z -= 0.001
				if self.z == -90.0:
					self.delim = 1
				if self.delim == 1 and self.z == -1.0:
					self.zf = True
				self.z += self.delim
			elif self.xf:
				if self.y == 90.001:
					self.delim = -1
					self.y -= 0.001
				if self.y == -90.0:
					self.delim = 1
				if self.delim == 1 and self.y == -1.0:
					self.yf = True
				self.y += self.delim
			else:
				if self.x == 90.001:
					self.delim = -1
					self.x -= 0.001
				if self.x == -90.0:
					self.delim = 1
				if self.delim == 1 and self.x == -1.0:
					self.xf = True
				self.x += self.delim
			self.update(self.x,self.y,self.z)
			time.sleep(.05)

	def kill(self):
		self.terminate = True

	def register_engine(self,eng):
		self.eng = eng

	def update(self,x,y,z):
		if self.eng is not None:
			self.eng.update(x,y,z)


g = Graphics()
nc = NoComm()
nc.register_engine(g.eng)

g.start()
nc.start()
