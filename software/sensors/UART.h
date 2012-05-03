#ifndef UART_H
#define UART_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

class UART {

	private:
		int fd;
		struct termios config;
		struct termios old;
		
	public:
		UART(const char *port, speed_t baud);

		int writeline(const char *line);
		int readline(char *line, int length);
		int teardown();

};

UART::UART(const char *port, speed_t baud) {
	if((fd = open(port, O_RDWR | O_NOCTTY)) < 0) {
		perror("Invalid serial port");
	}

	tcgetattr(fd,&old);
	bzero(&config,sizeof(config));
	config.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;
	config.c_iflag = IGNPAR | ICRNL;
	config.c_oflag = 0;
	config.c_lflag = ICANON;
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&config);
}

int UART::writeline(const char *line) {
	if(write(fd,line,strlen(line)) < 0) {
		cout << "Error writing to serial port" << endl;
		return -1;
	}

	return 0;
}

int UART::readline(char *line, int length) {
	int n = read(fd,line,length);
	line[n-1] = '\0';
	return n;
}

int UART::teardown() {
	tcsetattr(fd,TCSANOW,&old);
	return close(fd);
}

#endif
