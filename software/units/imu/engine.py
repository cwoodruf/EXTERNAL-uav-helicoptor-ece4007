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

try:
	from OpenGL.GLUT import *
	from OpenGL.GL import *
except:
	print 'Error: PyOpenGL not installed properly.'
	sys.exit()



class Engine():

	def __init__(self,argv,disp_mode=(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH),window=(500,500),name='new'):
		self.disp_mode = disp_mode
		self.window = window
		self.name = name
		
		glutInit(argv)
		glutInitDisplayMode(self.disp_mode)
		glutInitWindowSize(self.window[0],self.window[1])
		glutCreateWindow(self.name)

		glClearColor(0,0,0,0);
		glEnable(GL_DEPTH_TEST)

		glutReshapeFunc(self.reshape)
		glutKeyboardFunc(self.keyboard)
		glutDisplayFunc(self.display)

	def reshape(self,w,h):
		pass

	def keyboard(self,x,y):
		pass

	def display(self):
		pass

	def run(self):
		glutMainLoop()
