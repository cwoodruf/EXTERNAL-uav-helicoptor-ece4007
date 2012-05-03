# Copyright (c) 2012 Joey Yore
#
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.

CC			= /usr/bin/gcc
CFLAGS		= -Wall -g -lrt
CXX 		= /usr/bin/g++
CXXFLAGS 	= -Wall -g -lrt

all: i2c_bench testTonPulse testTimeoutPulse testTonSystime testTimeoutSystime

i2c_bench: i2c_bench.o
	$(CC) $(CFLAGS) -o i2c_bench i2c_bench.o
	@rm -f *.o

testTonPulse: test_ton_pulse.o
	$(CXX) $(CXXFLAGS) -o ton_pulse test_ton_pulse.o
	@rm -f *.o

testTimeoutPulse: test_timeout_pulse.o
	$(CXX) $(CXXFLAGS) -o timeout_pulse test_timeout_pulse.o
	@rm -f *.o

testTonSystime: test_ton_systime.o
	$(CXX) $(CXXFLAGS) -o ton_systime test_ton_systime.o
	@rm -f *.o

testTimeoutSystime: test_timeout_systime.o
	$(CXX) $(CXXFLAGS) -o timeout_systime test_timeout_systime.o
	@rm -f *.o

clean:
	@rm -f *.o i2c_bench ton_pulse timeout_pulse ton_systime timeout_systime
