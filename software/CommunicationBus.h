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

		virtual int recv(char *buf, int len);
		virtual int send(const char *buf, int len);
};

CommunicationBus::CommunicationBus() {
	ready = false;
	fd = -1;
}

CommunicationBus::CommunicationBus(const char *filename) {
	if((fd = open(filename,O_RDWR)) < 0) {
		ready = false;
	} else {
		ready = true;
	}
}

int CommunicationBus::recv(char *buf, int len) {
	if(ready) {
		return read(fd,buf,len);
	} else {
		return -1;
	}
}

int CommunicationBus::send(const char *buf, int len) {
	if(ready) {
		return write(fd,buf,len);
	} else {
		return -1;
	}
}

#endif
