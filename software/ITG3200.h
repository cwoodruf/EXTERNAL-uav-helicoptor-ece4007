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

//Bit Mappings
#define ITG3200_DLPFFS_FS_SEL             0x18
#define ITG3200_DLPFFS_DLPF_CFG           0x07
#define ITG3200_INTCFG_ACTL               0x80
#define ITG3200_INTCFG_OPEN               0x40
#define ITG3200_INTCFG_LATCH_INT_EN       0x20
#define ITG3200_INTCFG_INT_ANYRD_2CLEAR   0x10
#define ITG3200_INTCFG_ITG_RDY_EN         0x04
#define ITG3200_INTCFG_RAW_RDY_EN         0x01
#define ITG3200_INTSTATUS_ITG_RDY         0x04
#define ITG3200_INTSTATUS_RAW_DATA_RDY    0x01
#define ITG3200_PWRMGM_HRESET             0x80
#define ITG3200_PWRMGM_SLEEP              0x40
#define ITG3200_PWRMGM_STBY_XG            0x20
#define ITG3200_PWRMGM_STBY_YG            0x10
#define ITG3200_PWRMGM_STBY_ZG            0x08
#define ITG3200_PWRMGM_CLK_SEL            0x07

//
// Register Parameters
//

//Bitrates and Sampling Rates
#define ITG3200_NO_SR_DIVIDER		0x00	//No Sample Rate Divider
#define ITG3200_RANGE_FULL			0x03	//Full Scale Range
#define ITG3200_BW356_SR8			0x00	//LPF @ 256kHz & 8kHz Fs
#define ITG3200_BW188_SR1			0x01	//LPF @ 188kHz & 1kHz Fs
#define ITG3200_BW098_SR1			0x02	//LPF @ 098kHz & 1kHz Fs
#define ITG3200_BW042_SR1			0x03	//LPF @ 042kHz & 1kHz Fs
#define ITG3200_BW020_SR1			0x04	//LPF @ 020kHz & 1kHz Fs
#define ITG3200_BW010_SR1			0x05	//LPF @ 010kHz & 1kHz Fs
#define ITG3200_BW005_SR1			0x06	//LPF @ 005kHz & 1kHz Fs

//Interrupts
#define ITG3200_INT_ACTIVE_HI		0x00	//Interrupts Active Hi
#define ITG3200_INT_ACTIVE_LO		0x01	//Interrupts Active Lo
#define ITG3200_INT_PUSH_PULL		0x00	//Interrupts Push/Pull Driver
#define ITG3200_INT_OPEN_DRAIN		0x01	//Interrupts OpenDrain Drive
#define ITG3200_INT_PULSE_50US		0x00	//Interrupts Latch 50us
#define ITG3200_INT_UNTIL_CLEAR		0x01	//Interrupts Latch til clear
#define ITG3200_INT_READ_STATUSREG	0x00	//Interrupts Latch Clear Metod
#define ITG3200_INT_READ_ANYREG		0x01	//Interrupts Latch Clear Method

//Power
#define ITG3200_PWR_NORMAL			0x00	//Power Operation Mode
#define ITG3200_PWR_STANDBY			0x01	//Power Operation Mode

//Clock
#define ITG3200_CLK_INTERNAL		0x00	//Internal Clock Source
#define ITG3200_CLK_XGYRO_REF		0x01	//x-Gyro Clock Source
#define ITG3200_CLK_YGYRO_REF		0x02	//Y-Gyro Clock Source
#define ITG3200_CLK_ZGYRO_REF		0x03	//Z-Gyro Clock Source
#define ITG3200_CLK_EXTERNAL32		0x04	//External 32kHz Clock Source
#define ITG3200_CLK_EXTERNAL19		0x05	//External 19Mhz Clock Source


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
