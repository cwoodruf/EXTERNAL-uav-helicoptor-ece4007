
CXX 		= /usr/bin/g++
CXXFLAGS 	= -Wall -g

all: testTicker testTimeout testPID

testTicker:	test_ticker.o
	$(CXX) -g -o ticker test_ticker.o
	@rm -f *.o

testTimeout: test_timeout.o
	$(CXX) -g -o timeout test_timeout.o
	@rm -f *.o

testPID:	test_pid.o
	$(CXX) -g -o PID test_pid.o
	@rm -f *.o

clean:
	@rm -f *.o ticker timeout PID
