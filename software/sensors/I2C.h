#ifndef I2C_H
#define I2C_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

class I2C {

	protected:
		int fdr;
		int fdw;
		char buf[10];
		int status;

	public:
		I2C(int i,int addr_r,int addr_w);

		int get_status();
		int write_address(unsigned char reg);
		int write_byte(unsigned char reg, unsigned char data);
		int write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask);
		int read_current_byte(unsigned char &data);
		int read_byte(unsigned char reg, unsigned char &data);

};

I2C::I2C(int channel, int addr_r, int addr_w=-1) {
	char file[40];
	sprintf(file,"/dev/i2c-%d",channel);

	if((fdr = open(file,O_RDWR)) < 0) {
		cout << "ERROR Opening File" << endl;
		status = -1;
	}

	if(ioctl(fdr,I2C_SLAVE,addr_r) < 0) {
		cout << "ERROR Opening Device" << endl;
		status = -1;
	}

	if(addr_w == -1) {
		fdw = fdr;
	} else {
		if((fdw = open(file,O_RDWR)) < 0) {
			cout << "ERROR Opening File" << endl;
			status = -1;
		}

		if(ioctl(fdw,I2C_SLAVE,addr_w) < 0) {
			cout << "ERROR Opening Device" << endl;
			status = -1;
		}

	}

	status = 0;
}


int I2C::get_status() {
	return status;
}

int I2C::write_address(unsigned char reg) {
	buf[0] = reg;

	if(write(fdw,buf,1) != 1) {
		return -1;
	}
	return 0;
}

int I2C::write_byte(unsigned char reg, unsigned char data) {
	buf[0] = reg;
	buf[1] = data;

	if(write(fdw,buf,2) != 2) {
		return -1;
	}
	return 0;
}

int I2C::write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask) {
	unsigned char currentData;

	if(write_address(reg) != 0) {
		return -1;
	}

	if(read_current_byte(currentData) != 0) {
		return -2;
	}

	data = (currentData & ~mask) | (data & mask);
	return write_byte(reg,data);
}


int I2C::read_current_byte(unsigned char &data) {
	if(read(fdr,buf,1) != 1) {
		return -1;
	}

	data = buf[0];
	return 0;
}

int I2C::read_byte(unsigned char reg, unsigned char &data) {
	if(write_address(reg) != 0) {
		return -1;
	}

	return read_current_byte(data);
}


#endif
