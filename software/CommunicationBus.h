#ifndef COMMUNICATIONBUSINTERFACE_H
#define COMMUNICATIONBUSINTERFACE_H

#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

//Base Class for Communication Busses
class CommunicationBus {

	protected:
		int fd;
		bool ready;

	public:
		CommunicationBus();
		CommunicationBus(const char *filename);

		virtual int read_raw(char *buf, int len);
		virtual int write_raw(const char *buf, int len);
};

CommunicationBus::CommunicationBus() {
	ready = false;
	fd = -1;
}

CommunicationBus::CommunicationBus(const char *filename) {
	if(fd = open(filename,O_RDWR) < 0) {
		ready = false;
	}
	ready = true;
}

int CommunicationBus::read_raw(char *buf, int len) {
	if(ready) {
		return read(fd,buf,len);
	} else {
		return -1;
	}
}

int CommunicationBus::write_raw(const char *buf, int len) {
	if(ready) {
		return write(fd,buf,len);
	} else {
		return -1;
	}
}

#endif
