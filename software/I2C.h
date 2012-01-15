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


//Base Class for I2C Devices
class I2C : public CommunicationBus {

	public:
		I2C(unsigned char channel,int addr);
};

//channel is the I2C channel to connect to
//addr is the address of the device to connect to
I2C::I2C(unsigned char channel,int addr) {
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
}


#endif
