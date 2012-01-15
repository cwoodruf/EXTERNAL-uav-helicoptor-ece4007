#ifndef I2C_H
#define I2C_H

#include "CommunicationBus.h"
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;


//Class for I2C Devices
class I2C : public CommunicationBus {

	private:
		int fd;
		bool ready;
	

	public:
		I2C();
		I2C(unsigned char channel,int addr);

		bool init(unsigned char channel,int addr);
};

I2C::I2C() : fd(-1), ready(false) {

}

I2C::I2C(unsigned char channel,int addr) {
	init(channel,addr);	
}

//channel is the I2C channel to connect to
//addr is the address of the device to connect to
bool I2C::init(unsigned char channel,int addr) {
	char dev[40];

	sprintf(dev,"/dev/i2c-%c",channel); 
	
	if((fd = open(dev,O_RDWR)) < 0) {
		ready = false;
	} else {
		if(ioctl(fd,I2C_SLAVE,addr) < 0) {
			ready = false;	
		} else {
			ready = true;
		}
	}

	return ready;
}


#endif
