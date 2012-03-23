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

import engine
import sys

try:
	from OpenGL.GLUT import *
	from OpenGL.GLU import *
	from OpenGL.GL import *
except:
	print 'Error: PyOpenGL not installed properly.'
	sys.exit()


class Board(engine.Engine):

	x_angle = 0
	y_angle = 0
	z_angle = 0

	def display(self):
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		glLoadIdentity()

		glTranslatef(0,0,-10)
		glRotatef(self.x_angle,1,0,0)
		glRotatef(self.y_angle,0,1,0)
		glRotatef(self.z_angle,0,0,1)

		glBegin(GL_QUADS)

		glColor3f(0,0,0); glVertex3f(-.2,-1,-.025)
		glColor3f(0,0,1); glVertex3f(-.2,-1,.025)
		glColor3f(0,1,1); glVertex3f(-.2,1,.025)
		glColor3f(0,1,0); glVertex3f(-.2,1,-.025)

		glColor3f(1,0,0); glVertex3f(.2,-1,-.025)
		glColor3f(1,0,1); glVertex3f(.2,-1,.025)
		glColor3f(1,1,1); glVertex3f(.2,1,.025)
		glColor3f(1,1,0); glVertex3f(.2,1,-.025)

		glColor3f(0,0,0); glVertex3f(-.2,-1,-.025)
		glColor3f(0,0,1); glVertex3f(-.2,-1,.025)
		glColor3f(1,0,1); glVertex3f(.2,-1,.025)
		glColor3f(1,0,0); glVertex3f(.2,-1,-.025)

		glColor3f(0,1,0); glVertex3f(-.2,1,-.025)
		glColor3f(0,1,1); glVertex3f(-.2,1,.025)
		glColor3f(1,1,1); glVertex3f(.2,1,.025)
		glColor3f(1,1,0); glVertex3f(.2,1,-.025)

		glColor3f(0,0,0); glVertex3f(-.2,-1,-.025)
		glColor3f(0,1,0); glVertex3f(-.2,1,-.025)
		glColor3f(1,1,0); glVertex3f(.2,1,-.025)
		glColor3f(1,0,0); glVertex3f(.2,-1,-.025)

		glColor3f(0,0,1); glVertex3f(-.2,-1,.025)
		glColor3f(0,1,1); glVertex3f(-.2,1,.025)
		glColor3f(1,1,1); glVertex3f(.2,1,.025)
		glColor3f(1,0,1); glVertex3f(.2,-1,.025)

		glEnd()

		glFlush()
		glutSwapBuffers()

		glutPostRedisplay()

	def reshape(self,w,h):
		glMatrixMode(GL_PROJECTION)

		glLoadIdentity()
		gluPerspective(20,w/h,5,15)
		glViewport(0,0,w,h)

		glMatrixMode(GL_MODELVIEW)
		glutPostRedisplay()

	def keyboard(self,key,x,y):
		if key == 'q' or key == 'Q' or key == 27:
			sys.exit(0)

	def update(self,x,y,z):
		self.x_angle = x
		self.y_angle = y
		self.z_angle = z
