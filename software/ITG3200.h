#ifndef ITG3200_H
#define ITG3200_H

#include "I2C.h"
#include <iostream>

using namespace std;

//ADDRs
#define ITG3200_ADR_ADO_HI		0x69
#define ITG3200_ADR_ADO_LO		0x68

//Register Addresses
#define ITG3200_WHO_AM_I		0x00
#define ITG3200_SMPLRT_DIV		0x15
#define ITG3200_DLPF_FS			0x16
#define ITG3200_INT_CFG			0x17
#define ITG3200_INT_STATUS		0x1A
#define ITG3200_TEMP_OUT_HI		0x1B
#define ITG3200_TEMP_OUT_LO		0x1C
#define ITG3200_GYRO_XOUT_HI	0x1D
#define ITG3200_GYRO_XOUT_LO	0x1E
#define ITG3200_GYRO_YOUT_HI	0x1F
#define ITG3200_GYRO_YOUT_LO	0x20
#define ITG3200_GYRO_ZOUT_HI	0x21
#define ITG3200_GYRO_ZOUT_LO	0x22
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

		int get_whoami(unsigned char &addr);
		int get_sample_rate_divider(unsigned char &div);
		int set_sample_rate_divider(unsigned char value);
		int get_dlpf_full_scale(unsigned char &dlpf,unsigned char &fs);
		int set_dlpf_full_scale(unsigned char dlpf,unsigned char fs);
		int get_interrupt_conf(bool &actl, bool &open, bool &latch_int_en, bool &int_anyrd_2clear, bool &itg_rdy_en, bool &raw_rdy_en);
		int set_interrupt_conf(bool actl, bool open, bool latch_int_en, bool int_anyrd_2clear, bool itg_rdy_en, bool raw_rdy_en);
		int get_interrupt_status(bool &itg_rdy, bool &raw_data_rdy);
		int get_temp_out(short int &temp);
		int get_x_out(short int &x);
		int get_y_out(short int &y);
		int get_z_out(short int &z);
		int get_power_management(bool &h_reset, bool &sleep, bool &stby_xg, bool &stby_yg, bool &stby_zg, unsigned char &clk_sel);
		int set_power_management(bool h_reset, bool sleep, bool stby_xg, bool stby_yg, bool stby_zg, unsigned char clk_sel);

};

ITG3200::ITG3200() : I2C(2,ITG3200_ADR_ADO_HI) {

}

ITG3200::ITG3200(int channel) : I2C(channel,ITG3200_ADR_ADO_HI) {

}

ITG3200::ITG3200(int channel,int addr) : I2C(channel,addr) {

}

int ITG3200::get_whoami(unsigned char &addr) {
	return read_byte(ITG3200_WHO_AM_I,addr);
}

int ITG3200::get_sample_rate_divider(unsigned char &div) {
	return read_byte(ITG3200_SMPLRT_DIV,div);
}

int ITG3200::set_sample_rate_divider(unsigned char value) {
	return write_byte(ITG3200_SMPLRT_DIV,value);
}

int ITG3200::get_dlpf_full_scale(unsigned char &dlpf,unsigned char &fs) {
	int status = read_byte(ITG3200_DLPF_FS,dlpf);
	fs = dlpf >> 3;
	dlpf = dlpf & ITG3200_DLPFFS_DLPF_CFG;
	return status;
}

int ITG3200::set_dlpf_full_scale(unsigned char dlpf,unsigned char fs) {
	unsigned char value = (fs << 3) | (dlpf & ITG3200_DLPFFS_DLPF_CFG);	
	return write_byte(ITG3200_DLPF_FS,value);
}

int ITG3200::get_interrupt_conf(bool &actl, bool &open, bool &latch_int_en, bool &int_anyrd_2clear, bool &itg_rdy_en, bool &raw_rdy_en) {
	unsigned char value;
	int status = read_byte(ITG3200_INT_CFG,value);

	actl = value & ITG3200_INTCFG_ACTL;
	open = value & ITG3200_INTCFG_OPEN;
	latch_int_en = value & ITG3200_INTCFG_LATCH_INT_EN;
	int_anyrd_2clear = value & ITG3200_INTCFG_INT_ANYRD_2CLEAR;
	itg_rdy_en = value & ITG3200_INTCFG_ITG_RDY_EN;
	raw_rdy_en = value & ITG3200_INTCFG_RAW_RDY_EN;

	return status;
}

int ITG3200::set_interrupt_conf(bool actl, bool open, bool latch_int_en, bool int_anyrd_2clear, bool itg_rdy_en, bool raw_rdy_en) {

	unsigned char value = 0;
	value = (actl << 7) |
			(open << 6) |
			(latch_int_en << 5) |
			(0 << 3) |
			(int_anyrd_2clear << 4) |
			(itg_rdy_en << 2) |
			(0 << 1) |
			raw_rdy_en & ITG3200_INTCFG_RAW_RDY_EN;


	return write_byte(ITG3200_INT_CFG,value);
}

int ITG3200::get_interrupt_status(bool &itg_rdy, bool &raw_data_rdy) {
	unsigned char value;
	int status = read_byte(ITG3200_INT_STATUS,value);

	itg_rdy = value & ITG3200_INTSTATUS_ITG_RDY; 
	raw_data_rdy = value & ITG3200_INTSTATUS_RAW_DATA_RDY;

	return status;
} 

int ITG3200::get_temp_out(short int &temp) {
	unsigned char v;
	int status = read_byte(ITG3200_TEMP_OUT_HI,v);

	temp = v;
	status += read_byte(ITG3200_TEMP_OUT_LO,v);
	temp = (temp << 8) | v;

	return status;
}

int ITG3200::get_x_out(short int &x) {
	unsigned char v;
	int status = read_byte(ITG3200_GYRO_XOUT_HI,v);

	x = v;
	status += read_byte(ITG3200_GYRO_XOUT_LO,v);
	x = (x << 8) | v;

	return status;
}

int ITG3200::get_y_out(short int &y) {
	unsigned char v;
	int status = read_byte(ITG3200_GYRO_YOUT_HI,v);

	y = v;
	status += read_byte(ITG3200_GYRO_YOUT_LO,v);
	y = (y << 8) | v;

	return status;
}

int ITG3200::get_z_out(short int &z) {
	unsigned char v;
	int status = read_byte(ITG3200_GYRO_ZOUT_HI,v);

	z = v;
	status += read_byte(ITG3200_GYRO_ZOUT_LO,v);
	z = (z << 8) | v;

	return status;
}

int ITG3200::get_power_management(bool &h_reset, bool &sleep, bool &stby_xg, bool &stby_yg, bool &stby_zg, unsigned char &clk_sel) {
	unsigned char value;
	int status = read_byte(ITG3200_PWR_MGM,value);

	h_reset = value & ITG3200_PWRMGM_HRESET;
	sleep = value & ITG3200_PWRMGM_SLEEP;
	stby_xg = value & ITG3200_PWRMGM_STBY_XG;
	stby_yg = value & ITG3200_PWRMGM_STBY_YG;
	stby_zg = value & ITG3200_PWRMGM_STBY_ZG;
	clk_sel = value & ITG3200_PWRMGM_CLK_SEL;

	return status;
}

int ITG3200::set_power_management(bool h_reset, bool sleep, bool stby_xg, bool stby_yg, bool stby_zg, unsigned char clk_sel) {

	unsigned char value = 0;
	value = (h_reset << 7) |
			(sleep << 6) |
			(stby_xg << 5) |
			(stby_yg << 4) |
			(stby_zg << 3) |
			(clk_sel & 0x07);

	return write_byte(ITG3200_PWR_MGM,value);
}

#endif
