#ifndef ADXL345_2_H
#define ADXL345_2_H

#include "I2C.h"
#include "util.h"
#include <iostream>

using namespace std;

//Register Addresses
#define ADXL345_DEVID				0x00
#define ADXL345_THRESH_TAP			0x1D
#define ADXL345_OFSX				0x1E
#define ADXL345_OFSY				0x1F
#define ADXL345_OFSZ				0x20
#define ADXL345_DUR					0x21
#define ADXL345_LATENT				0x22
#define ADXL345_WINDOW				0x23
#define ADXL345_THRESH_ACT			0x24
#define ADXL345_THRESH_INACT		0x25
#define ADXL345_TIME_INACT			0x26
#define ADXL345_ACT_INACT_CTL		0x27
#define ADXL345_THRESH_FF			0x28
#define ADXL345_TIME_FF				0x29
#define ADXL345_TAP_AXES			0x2A
#define ADXL345_ACT_TAP_STATUS		0x2B
#define ADXL345_BW_RATE				0x2C
#define ADXL345_POWER_CTL			0x2D
#define ADXL345_INT_ENABLE			0x2E
#define ADXL345_INT_MAP				0x2F
#define ADXL345_INT_SOURCE			0x30
#define ADXL345_DATA_FORMAT			0x31
#define ADXL345_DATAX0				0x32
#define ADXL345_DATAX1				0x33
#define ADXL345_DATAY0				0x34
#define ADXL345_DATAY1				0x35
#define ADXL345_DATAZ0				0x36
#define ADXL345_DATAZ1				0x37
#define ADXL345_FIFO_CTL			0x38
#define ADXL345_FIFO_STATUS			0x39

//Interrupts
#define ADXL345_INT1_PIN			0x00
#define ADXL345_INT2_PIN			0x01

//Interrupt Bit Positions
#define ADXL345_INT_DATA_READY_BIT	0x07
#define ADXL345_INT_SINGLE_TAP_BIT	0x06
#define ADXL345_INT_DOUBLE_TAP_BIT	0x05
#define ADXL345_INT_ACTIVITY_BIT	0x04
#define ADXL345_INT_INACTIVITY_BIT	0x03
#define ADXL345_INT_FREE_FALL_BIT	0x02
#define ADXL345_INT_WATERMARK_BIT	0x01
#define ADXL345_INT_OVERRUNY_BIT	0x00

//IDs
#define ADXL345_ID_R				0xA7
#define ADXL345_ID_W				0xA6


class ADXL345 : public I2C {

	private:
		unsigned char range;
	public:
		//Constructors
		ADXL345();
		ADXL345(int i);

		//Destructor
		~ADXL345();	

		//Spec Implementations
		int get_dev_id(unsigned char &id);
		int get_thresh_tap(unsigned short int &thresh_tap);
		int get_thresh_tap_raw(unsigned char &thresh_tap);
		int set_thresh_tap(unsigned short int thresh_tap);
		int set_thresh_tap_raw(unsigned char thresh_tap);
		int get_offset_x(short int &offset_x);
		int get_offset_x_raw(unsigned char &offset_x);
		int set_offset_x(short int offset_x);
		int set_offset_x_raw(unsigned char offset_x);
		int get_offset_y(short int &offset_y);
		int get_offset_y_raw(unsigned char &offset_y);
		int set_offset_y(short int offset_y);
		int set_offset_y_raw(unsigned char offset_y);
		int get_offset_z(short int &offset_z);
		int get_offset_z_raw(unsigned char &offset_z);
		int set_offset_z(short int offset_z);
		int set_offset_z_raw(unsigned char offset_z);
		int get_dur(unsigned int &dur);
		int get_dur_raw(unsigned char &dur);
		int set_dur(unsigned int dur);
		int set_dur_raw(unsigned char dur);
		int get_latency(unsigned short int &latency);
		int get_latency_raw(unsigned char &latency);
		int set_latency(unsigned short int latency);
		int set_latency_raw(unsigned char latency);
		int get_window(unsigned short int &window);
		int get_window_raw(unsigned char &window);
		int set_window(unsigned short int window);
		int set_window_raw(unsigned char window);
		int get_thresh_act(unsigned short int &thresh_inact);
		int get_thresh_act_raw(unsigned char &thresh_inact);
		int set_thresh_act(unsigned short int thresh_inact);
		int set_thresh_act_raw(unsigned char thresh_inact);
		int get_thresh_inact(unsigned short int &thresh_inact);
		int get_thresh_inact_raw(unsigned char &thresh_inact);
		int set_thresh_inact(unsigned short int thresh_inact);
		int set_thresh_inact_raw(unsigned char thresh_inact);
		int get_time_inact(unsigned char &time);
		int set_time_inact(unsigned char time);
		int get_act_inact_ctl(bool &act_acdc, bool &act_x_en, bool &act_y_en, bool &act_z_en, bool &inact_acdc, bool &inact_x_en, bool &inact_y_en, bool &inact_z_en);
		int set_act_inact_ctl(bool act_acdc, bool act_x_en, bool act_y_en, bool act_z_en, bool inact_acdc, bool inact_x_en, bool inact_y_en, bool inact_z_en);
		int get_thresh_ff(unsigned short int &thresh_inact);
		int get_thresh_ff_raw(unsigned char &thresh_inact);
		int set_thresh_ff(unsigned short int thresh_inact);
		int set_thresh_ff_raw(unsigned char thresh_inact);
		int get_time_ff(unsigned short int &time);
		int get_time_ff_raw(unsigned char &time);
		int set_time_ff(unsigned short int time);
		int set_time_ff_raw(unsigned char time);
		int get_tap_axes(bool &suppress, bool &tap_x_en, bool &tap_y_en, bool &tap_z_en);
		int set_tap_axes(bool suppress, bool tap_x_en, bool tap_y_en, bool tap_z_en);
		int get_tap_status(bool &act_x_src, bool &act_y_src, bool &act_z_src, bool &asleep, bool &tap_x_src, bool &tap_y_src, bool &tap_z_src);
		int set_tap_status(bool act_x_src, bool act_y_src, bool act_z_src, bool asleep, bool tap_x_src, bool tap_y_src, bool tap_z_src);
		int get_bw_rate(bool &low_power, unsigned char &rate);
		int set_bw_rate(bool low_power, unsigned char rate);
		int get_power_ctl(bool &link, bool &auto_sleep, bool &measure, bool &sleep, unsigned char &wakeup);
		int set_power_ctl(bool link, bool auto_sleep, bool measure, bool sleep, unsigned char wakeup);
		int get_int_enable(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun);
		int set_int_enable(bool data_ready, bool single_tap, bool double_tap, bool activity, bool inactivity, bool free_fall, bool watermark, bool overrun);
		int get_int_map(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun);
		int set_int_map(bool data_ready, bool single_tap, bool double_tap, bool activity, bool inactivity, bool free_fall, bool watermark, bool overrun);
		int get_int_source(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun);
		int get_data_format(bool &self_test, bool &spi, bool &int_invert, bool &full_res, bool &justify, unsigned char &range);
		int set_data_format(bool self_test, bool spi, bool int_invert, bool full_res, bool justify, unsigned char range);
		//int get_data_x(short int &x);
		int get_data_x_raw(short int &x);
		//int get_data_y(short int &y);
		int get_data_y_raw(short int &y);
		//int get_data_z(short int &z);
		int get_data_z_raw(short int &z);


		//High Level Functions
		int set_standby_mode();
        int set_measurement_mode();
        int set_full_res();

};

ADXL345::ADXL345() : I2C(2,ADXL345_ID_R,ADXL345_ID_W) {
	bool dummy;
	get_data_format(dummy,dummy,dummy,dummy,dummy,this->range);
}

ADXL345::ADXL345(int i) : I2C(i,ADXL345_ID_R,ADXL345_ID_W) {
	bool dummy;
	get_data_format(dummy,dummy,dummy,dummy,dummy,this->range);
}

ADXL345::~ADXL345() {
}

int ADXL345::get_dev_id(unsigned char &id) {
	return read_byte(ADXL345_DEVID,id);
}

//Each count is 62.5 mg
//Return value is 0 to 16000 mg
int ADXL345::get_thresh_tap(unsigned short int &thresh_tap) {
	unsigned char read;
	int status = read_byte(ADXL345_THRESH_TAP,read);
	thresh_tap = scale<unsigned char,unsigned short int>(read,62.5,true);
	return status;
}

//Returns value in counts
int ADXL345::get_thresh_tap_raw(unsigned char &thresh_tap) {
	return read_byte(ADXL345_THRESH_TAP,thresh_tap);
}

//Input should be between 0 and 16000 mg
int ADXL345::set_thresh_tap(unsigned short int thresh_tap) {
	thresh_tap = LIMIT(0,thresh_tap,16000);
	unsigned char value = scale<unsigned short int,unsigned char>(thresh_tap,62.5,false);
	return write_byte(ADXL345_THRESH_TAP,value);
}

//Input range 0x00 to 0xFF
int ADXL345::set_thresh_tap_raw(unsigned char thresh_tap) {
	return write_byte(ADXL345_THRESH_TAP,thresh_tap);
}

//Each count is 15.6 mg
//Return value is -2000mg to 2000mg
int ADXL345::get_offset_x(short int &offset_x) {
	unsigned char read;
	int status = read_byte(ADXL345_OFSX,read);
	offset_x = scale<unsigned char,short int>(read,15.6,true);
	return status;
}

int ADXL345::get_offset_x_raw(unsigned char &offset_x) {
	return read_byte(ADXL345_OFSX,offset_x);
}

//Input should be between -2000mg and 2000 mg
int ADXL345::set_offset_x(short int offset_x) {
	offset_x = LIMIT(-2000,offset_x,2000);
	unsigned char value = scale<short int,unsigned char>(offset_x,15.6,false);
	return write_byte(ADXL345_OFSX,value);
}

int ADXL345::set_offset_x_raw(unsigned char offset_x) {
	return write_byte(ADXL345_OFSX,offset_x);
}

//Each count is 15.6 mg
//Return value is -2000mg to 2000mg
int ADXL345::get_offset_y(short int &offset_y) {
	unsigned char read;
	int status = read_byte(ADXL345_OFSY,read);
	offset_y = scale<unsigned char,short int>(read,15.6,true);
	return status;
}

int ADXL345::get_offset_y_raw(unsigned char &offset_y) {
	return read_byte(ADXL345_OFSY,offset_y);
}

//Input should be between -2000mg and 2000 mg
int ADXL345::set_offset_y(short int offset_y) {
	offset_y = LIMIT(-2000,offset_y,2000);
	unsigned char value = scale<short int,unsigned char>(offset_y,15.6,false);
	return write_byte(ADXL345_OFSY,offset_y);
}

int ADXL345::set_offset_y_raw(unsigned char offset_y) {
	return write_byte(ADXL345_OFSY,offset_y);
}

//Each count is 15.6 mg
//Return value is -2000mg to 2000mg
int ADXL345::get_offset_z(short int &offset_z) {
	unsigned char read;
	int status = read_byte(ADXL345_OFSZ,read);
	offset_z = scale<unsigned char,short int>(read,15.6,true);
	return status;
}

int ADXL345::get_offset_z_raw(unsigned char &offset_z) {
	return read_byte(ADXL345_OFSZ,offset_z);
}

//Input should be between -2000mg and 2000 mg
int ADXL345::set_offset_z(short int offset_z) {
	offset_z = LIMIT(-2000,offset_z,2000);
	unsigned char value = scale<short int,unsigned char>(offset_z,15.6,false);
	return write_byte(ADXL345_OFSZ,offset_z);
}

int ADXL345::set_offset_z_raw(unsigned char offset_z) {
	return write_byte(ADXL345_OFSZ,offset_z);
}

//Each count is 625us
//Return value is 0 to 160000us
int ADXL345::get_dur(unsigned int &dur) {
	unsigned char tmp;
	int status = read_byte(ADXL345_DUR,tmp);
	dur = scale<unsigned char,unsigned int>(tmp,625,true);
	return status;
}

int ADXL345::get_dur_raw(unsigned char &dur) {
	return read_byte(ADXL345_DUR,dur);
}

//Input value is 0 to 160000us
int ADXL345::set_dur(unsigned int dur) {
	dur = LIMIT(0,dur,160000);
	unsigned char value = scale<unsigned int,unsigned char>(dur,625,false);
	return write_byte(ADXL345_DUR,value);
}

int ADXL345::set_dur_raw(unsigned char dur) {
	return write_byte(ADXL345_DUR,dur);
}

//Each count is 1.25ms
//Return value is 0 to 320ms
int ADXL345::get_latency(unsigned short int &latency) {
	unsigned char tmp;
	int status = read_byte(ADXL345_LATENT,tmp);
	latency = scale<unsigned char,unsigned short int>(tmp,1.25,true);
	return status;	
}

int ADXL345::get_latency_raw(unsigned char &latency) {
	return read_byte(ADXL345_LATENT,latency);
}

//Input value is 0 to 320ms
int ADXL345::set_latency(unsigned short int latency) {
	latency = LIMIT(0,latency,320);
	unsigned char value = scale<unsigned short int,unsigned char>(latency,1.25,false);
	return write_byte(ADXL345_LATENT,value);
}

int ADXL345::set_latency_raw(unsigned char latency) {
	return write_byte(ADXL345_LATENT,latency);
}

//Each count is 1.25ms
//Return value is 0 to 320ms
int ADXL345::get_window(unsigned short int &window) {
	unsigned char tmp;
	int status = read_byte(ADXL345_WINDOW,tmp);
	window = scale<unsigned char,unsigned short int>(tmp,1.25,true);
	return status;	
}

int ADXL345::get_window_raw(unsigned char &window) {
	return read_byte(ADXL345_WINDOW,window);
}

//Input value is 0 to 320ms
int ADXL345::set_window(unsigned short int window) {
	window = LIMIT(0,window,320);
	unsigned char value = scale<unsigned short int, unsigned char>(window,1.25,false);
	return write_byte(ADXL345_WINDOW,value);
}

int ADXL345::set_window_raw(unsigned char window) {
	return write_byte(ADXL345_WINDOW,window);
}

//Each count is 62.5 mg
//Return value is 0 to 16000 mg
int ADXL345::get_thresh_act(unsigned short int &thresh_act) {
	unsigned char read;
	int status = read_byte(ADXL345_THRESH_ACT,read);
	thresh_act = scale<unsigned char,unsigned short int>(read,62.5,true);
	return status;
}

//Returns value in counts
int ADXL345::get_thresh_act_raw(unsigned char &thresh_act) {
	return read_byte(ADXL345_THRESH_ACT,thresh_act);
}

//Input should be between 0 and 16000 mg
int ADXL345::set_thresh_act(unsigned short int thresh_act) {
	thresh_act = LIMIT(0,thresh_act,16000);
	unsigned char value = scale<unsigned short int,unsigned char>(thresh_act,62.5,false);

	return write_byte(ADXL345_THRESH_ACT,value);
}

//Input range 0x00 to 0xFF
int ADXL345::set_thresh_act_raw(unsigned char thresh_act) {
	return write_byte(ADXL345_THRESH_ACT,thresh_act);
}

//Each count is 62.5 mg
//Return value is 0 to 16000 mg
int ADXL345::get_thresh_inact(unsigned short int &thresh_inact) {
	unsigned char read;
	int status = read_byte(ADXL345_THRESH_INACT,read);
	thresh_inact = scale<unsigned char,unsigned short int>(read,62.5,true);
	return status;
}

//Returns value in counts
int ADXL345::get_thresh_inact_raw(unsigned char &thresh_inact) {
	return read_byte(ADXL345_THRESH_INACT,thresh_inact);
}

//Input should be between 0 and 16000 mg
int ADXL345::set_thresh_inact(unsigned short int thresh_inact) {
	thresh_inact = LIMIT(0,thresh_inact,16000);
	unsigned char value = scale<unsigned short int,unsigned char>(thresh_inact,62.5,false);

	return write_byte(ADXL345_THRESH_INACT,value);
}

//Input range 0x00 to 0xFF
int ADXL345::set_thresh_inact_raw(unsigned char thresh_inact) {
	return write_byte(ADXL345_THRESH_INACT,thresh_inact);
}

//1s per count
int ADXL345::get_time_inact(unsigned char &time) {
	return read_byte(ADXL345_TIME_INACT,time);
}

int ADXL345::set_time_inact(unsigned char time) {
	return write_byte(ADXL345_TIME_INACT,time);
}

int ADXL345::get_act_inact_ctl(bool &act_acdc, bool &act_x_en, bool &act_y_en, bool &act_z_en, bool &inact_acdc, bool &inact_x_en, bool &inact_y_en, bool &inact_z_en) {
	unsigned char value;
	int status = read_byte(ADXL345_ACT_INACT_CTL,value);

	act_acdc = (value & 0x80);
	act_x_en = (value & 0x40);
	act_y_en = (value & 0x20);
	act_z_en = (value & 0x10);
	inact_acdc = (value & 0x08);
	inact_x_en = (value & 0x04);
	inact_y_en = (value & 0x02);
	inact_z_en = (value & 0x01);

	return status;
}

int ADXL345::set_act_inact_ctl(bool act_acdc, bool act_x_en, bool act_y_en, bool act_z_en, bool inact_acdc, bool inact_x_en, bool inact_y_en, bool inact_z_en) {

	unsigned char value = (act_acdc << 7) |
						  (act_x_en << 6) |
						  (act_y_en << 5) |
						  (act_z_en << 4) |
						  (inact_acdc << 3) |
						  (inact_x_en << 2) |
						  (inact_y_en << 1) |
						  (inact_z_en);

	return write_byte(ADXL345_ACT_INACT_CTL,value);
}

//Each count is 62.5 mg
//Return value is 0 to 16000 mg
int ADXL345::get_thresh_ff(unsigned short int &thresh_ff) {
	unsigned char read;
	int status = read_byte(ADXL345_THRESH_FF,read);
	thresh_ff = scale<unsigned char,unsigned short int>(read,62.5,true);
	return status;
}

//Returns value in counts
int ADXL345::get_thresh_ff_raw(unsigned char &thresh_ff) {
	return read_byte(ADXL345_THRESH_FF,thresh_ff);
}

//Input should be between 0 and 16000 mg
int ADXL345::set_thresh_ff(unsigned short int thresh_ff) {
	thresh_ff = LIMIT(0,thresh_ff,16000);
	unsigned char value = scale<unsigned short int,unsigned char>(thresh_ff,62.5,false);

	return write_byte(ADXL345_THRESH_FF,value);
}

//Input range 0x00 to 0xFF
int ADXL345::set_thresh_ff_raw(unsigned char thresh_ff) {
	return write_byte(ADXL345_THRESH_FF,thresh_ff);
}

//Each count is 5ms
//Return value is 0 to 1280ms
int ADXL345::get_time_ff(unsigned short int &time) {
	unsigned char tmp;
	int status = read_byte(ADXL345_TIME_FF,tmp);
	time = scale<unsigned char,unsigned short int>(tmp,5,true);
	return status;
}

int ADXL345::get_time_ff_raw(unsigned char &time) {
	return read_byte(ADXL345_TIME_FF,time);
}

//Input is 0 to 1280ms
int ADXL345::set_time_ff(unsigned short int time) {
	time = LIMIT(0,time,1280);
	unsigned char tmp = scale<unsigned short int,unsigned char>(time,5,false);
	return write_byte(ADXL345_TIME_FF,tmp);
}

int ADXL345::set_time_ff_raw(unsigned char time) {
	return write_byte(ADXL345_TIME_FF,time);
}

int ADXL345::get_tap_axes(bool &suppress, bool &tap_x_en, bool &tap_y_en, bool &tap_z_en) {
	unsigned char value;
	int status = read_byte(ADXL345_TAP_AXES,value);

	suppress = (value & 0x08);
	tap_x_en = (value & 0x04);
	tap_y_en = (value & 0x02);
	tap_z_en = (value & 0x01);

	return status;
}

int ADXL345::set_tap_axes(bool suppress, bool tap_x_en, bool tap_y_en, bool tap_z_en) {
	unsigned char value = 0;

	value = (suppress << 3) | (tap_x_en << 2) | (tap_y_en << 1) | tap_z_en;

	return write_byte(ADXL345_TAP_AXES,value);
}

int ADXL345::get_tap_status(bool &act_x_src, bool &act_y_src, bool &act_z_src, bool &asleep, bool &tap_x_src, bool &tap_y_src, bool &tap_z_src) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_ACT_TAP_STATUS,value);

	act_x_src = (value & 0x40);
	act_y_src = (value & 0x20);
	act_z_src = (value & 0x10);
	asleep = (value & 0x08);
	tap_x_src = (value & 0x04);
	tap_y_src = (value & 0x02);
	tap_z_src = (value & 0x01);

	return status;
}

int ADXL345::set_tap_status(bool act_x_src, bool act_y_src, bool act_z_src, bool asleep, bool tap_x_src, bool tap_y_src, bool tap_z_src) {
	unsigned char value = 0;

	value = (act_x_src << 6) |
			(act_y_src << 5) |
			(act_z_src << 4) |
			(asleep << 3) |
			(tap_x_src << 2) |
			(tap_y_src << 1) |
			(tap_z_src);

	return write_byte(ADXL345_ACT_TAP_STATUS,value);
}

int ADXL345::get_bw_rate(bool &low_power,unsigned char &rate) {
	unsigned char tmp;
	int status = read_byte(ADXL345_BW_RATE,tmp);

	low_power = (tmp & 0x10);
	rate = (tmp & 0x0F);

	return status;
}

int ADXL345::set_bw_rate(bool low_power, unsigned char rate) {

	unsigned char value = 0;
	rate &= 0x0F;
	
	value = (low_power << 4) | rate;
	return write_byte(ADXL345_BW_RATE,value);
}

int ADXL345::get_power_ctl(bool &link, bool &auto_sleep, bool &measure, bool &sleep, unsigned char &wakeup) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_POWER_CTL,value);

	link = (value & 0x20);
	auto_sleep = (value & 0x10);
	measure = (value & 0x08);
	sleep = (value & 0x04);
	wakeup = (value & 0x03);

	return status;
}

int ADXL345::set_power_ctl(bool link, bool auto_sleep, bool measure, bool sleep, unsigned char wakeup) {
	unsigned char value = 0;

	value = (link << 5) | (auto_sleep << 4) | (measure << 3) | (sleep << 2) | (wakeup & 0x03);

	return write_byte(ADXL345_POWER_CTL,value);
}

int ADXL345::get_int_enable(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_INT_ENABLE,value);

	data_ready = (value & 0x80);
	single_tap = (value & 0x40);
	double_tap = (value & 0x20);
	activity = (value & 0x10);
	inactivity = (value & 0x08);
	free_fall = (value & 0x04);
	watermark = (value & 0x02);
	overrun = (value & 0x01);

	return status;
}

int ADXL345::set_int_enable(bool data_ready, bool single_tap, bool double_tap, bool activity, bool inactivity, bool free_fall, bool watermark, bool overrun) {
	unsigned char value = 0;

	value = (data_ready << 7) |
			(single_tap << 6) |
			(double_tap << 5) |
			(activity << 4) |
			(inactivity << 3) |
			(free_fall << 2) |
			(watermark << 1) |
			(overrun);

	return write_byte(ADXL345_INT_ENABLE,value);
}

int ADXL345::get_int_map(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_INT_MAP,value);

	data_ready = (value & 0x80);
	single_tap = (value & 0x40);
	double_tap = (value & 0x20);
	activity = (value & 0x10);
	inactivity = (value & 0x08);
	free_fall = (value & 0x04);
	watermark = (value & 0x02);
	overrun = (value & 0x01);

	return status;
}

int ADXL345::set_int_map(bool data_ready, bool single_tap, bool double_tap, bool activity, bool inactivity, bool free_fall, bool watermark, bool overrun) {
	unsigned char value = 0;

	value = (data_ready << 7) |
			(single_tap << 6) |
			(double_tap << 5) |
			(activity << 4) |
			(inactivity << 3) |
			(free_fall << 2) |
			(watermark << 1) |
			(overrun);

	return write_byte(ADXL345_INT_MAP,value);
}

int ADXL345::get_int_source(bool &data_ready, bool &single_tap, bool &double_tap, bool &activity, bool &inactivity, bool &free_fall, bool &watermark, bool &overrun) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_INT_SOURCE,value);

	data_ready = (value & 0x80);
	single_tap = (value & 0x40);
	double_tap = (value & 0x20);
	activity = (value & 0x10);
	inactivity = (value & 0x08);
	free_fall = (value & 0x04);
	watermark = (value & 0x02);
	overrun = (value & 0x01);

	return status;
}

int ADXL345::get_data_format(bool &self_test, bool &spi, bool &int_invert, bool &full_res, bool &justify, unsigned char &range) {
	unsigned char value = 0;
	int status = read_byte(ADXL345_DATA_FORMAT,value);

	self_test = (value & 0x80);
	spi = (value & 0x40);
	int_invert = (value & 0x20);
	full_res = (value & 0x08);
	justify = (value & 0x04);
	range = (value & 0x03);

	return status;
}

int ADXL345::set_data_format(bool self_test, bool spi, bool int_invert, bool full_res, bool justify, unsigned char range) {
	unsigned char value = 0;

	value = (self_test << 7) |
			(spi << 6) |
			(int_invert << 5) |
			(full_res << 3) |
			(justify << 2) |
			(range & 0x03);

	return write_byte(ADXL345_DATA_FORMAT,value);
}

int ADXL345::get_data_x_raw(short int &x) {
	unsigned char lsb = 0;
	unsigned char hsb = 0;

	int status = read_byte(ADXL345_DATAX0,lsb);
	status += read_byte(ADXL345_DATAX0,hsb);

	x = ((short int)hsb << 8) | lsb;

	return status;
} 

int ADXL345::get_data_y_raw(short int &y) {
	unsigned char lsb = 0;
	unsigned char hsb = 0;

	int status = read_byte(ADXL345_DATAY0,lsb);
	status += read_byte(ADXL345_DATAY0,hsb);

	y = ((short int)hsb << 8) | lsb;

	return status;
}

int ADXL345::get_data_z_raw(short int &z) {
	unsigned char lsb = 0;
	unsigned char hsb = 0;

	int status = read_byte(ADXL345_DATAZ0,lsb);
	status += read_byte(ADXL345_DATAZ0,hsb);

	z = ((short int)hsb << 8) | lsb;

	return status;
}
//FIFO_STATUS

int ADXL345::set_standby_mode() {
	bool link, auto_sleep, measure, sleep;
	unsigned char wakeup;
	int status = get_power_ctl(link,auto_sleep,measure,sleep,wakeup);
	status += set_power_ctl(link,auto_sleep,false,sleep,wakeup);
	return status;
}

int ADXL345::set_measurement_mode() {
	bool link, auto_sleep, measure, sleep;
	unsigned char wakeup;
	int status = get_power_ctl(link,auto_sleep,measure,sleep,wakeup);
	status += set_power_ctl(link,auto_sleep,true,sleep,wakeup);
	return status;
}

int ADXL345::set_full_res() {
	bool self_test, spi, int_invert, full_res, justify;
	unsigned char range;
	int status = get_data_format(self_test,spi,int_invert,full_res,justify,range);
	status += set_data_format(self_test,spi,int_invert,true,justify,0x3);
	return status;
}
#endif
