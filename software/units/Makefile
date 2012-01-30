
CXX 		= /usr/bin/g++
CXXFLAGS 	= -Wall -g

all: testTicker testTimeout testPID testUtil

testTicker:	test_ticker.o
	$(CXX) $(CXXFLAGS) -o ticker test_ticker.o
	@rm -f *.o

testTimeout: test_timeout.o
	$(CXX) $(CXXFLAGS) -o timeout test_timeout.o
	@rm -f *.o

testPID: test_pid.o
	$(CXX) $(CXXFLAGS) -o PID test_pid.o
	@rm -f *.o

testUtil:	test_util.o
	$(CXX) $(CXXFLAGS) -o util test_util.o
	@rm -f *.o


clean:
	@rm -f *.o ticker timeout PID util
