#ifndef ITG3200_H
#define ITG3200_H

#include "I2C.h"
#include <iostream>

using namespace std;

//ADDRs
#define ITG3200_ADR_ADO_HIGH	0x69
#define ITG3200_ADR_ADO_LOW		0x68

//Register Addresses
#define ITG3200_WHO_AM_I		0x00
#define ITG3200_SMPLRT_DIV		0x15
#define ITG3200_DLPF_FS			0x16
#define ITG3200_INT_CFG			0x17
#define ITG3200_INT_STATUS		0x1A
#define ITG3200_TEMP_OUT		0x1B
#define ITG3200_GYRO_XOUT		0x1D
#define ITG3200_GYRO_YOUT		0x1F
#define ITG3200_GYRO_ZOUT		0x21
#define ITG3200_PWR_MGM			0x2E


class ITG3200 : public I2C {

	private:

	public:
		ITG3200();
		ITG3200(int channel);
		ITG3200(int channel,int addr);
		int reconnect();
		int reconnect(int channel);
		int reconnect(int channel, int addr);
};

ITG3200::ITG3200() : I2C(2,ITG3200_ADR_ADO_HIGH) {

}

ITG3200::ITG3200(int channel) : I2C(channel,ITG3200_ADR_ADO_HIGH) {

}

ITG3200::ITG3200(int channel,int addr) : I2C(channel,addr) {

}

int ITG3200::reconnect() {
	return bus_init(2,ITG3200_ADR_ADO_HIGH);
}

int ITG3200::reconnect(int channel) {
	return bus_init(channel,ITG3200_ADR_ADO_HIGH);
}

int ITG3200::reconnect(int channel, int addr) {
	return bus_init(channel,addr);
}

#endif
