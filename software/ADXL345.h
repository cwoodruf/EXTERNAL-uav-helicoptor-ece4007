#ifndef ADXL345_H
#define ADXL345_H

#include "I2C.h"
#include "util.h"
#include <stdlib.h>


//TODO: Add SubFunctions for setting the bits in registers

class ADXL345 {

	private:
		I2C recvLine;
		I2C sendLine;

		unsigned char getByte(char addr);
		void sendByte(char addr, char value);

	public:
		ADXL345(unsigned char channel);
		ADXL345(unsigned char channel,bool alt);


		//Read Methods
		unsigned char getDevId();
		unsigned char getTapThreshold();
		char getXaxisOffset();
		char getYaxisOffset();
		char getZaxisOffset();
		unsigned char getTapDuration();
		unsigned char getTapLatency();
		unsigned char getTapWindow();
		unsigned char getActivityThreshold();
		unsigned char getInactivityThreshold();
		unsigned char getInactivityTime();
		unsigned char getActInactControl();
		unsigned char getFreeFallThresh();
		unsigned char getFreeFallTime();
		unsigned char getTapAxes();
		unsigned char getTapStatus();
		unsigned char getBitrate();
		unsigned char getPowerControl();
		unsigned char getInterruptEnable();
		unsigned char getInterruptMap();
		unsigned char getInterruptSource();
		unsigned char getDataFormat();
		unsigned short int getXAccel();
		unsigned short int getYAccel();
		unsigned short int getZAccel();
		unsigned char getFIFOControl();
		unsigned char getFIFOStatus();

		//Write Methods
		void setTapThreshold(unsigned char value);
		void setXaxisOffset(char value);
		void setYaxisOffset(char value);
		void setZaxisOffset(char value);
		void setTapDuration(unsigned char value);
		void setTapLatency(unsigned char value);
		void setTapWindow(unsigned char value);
		void setActivityThreshold(unsigned char value);
		void setInactivityThreshold(unsigned char value);
		void setInactivityTime(unsigned char value);
		void setActInactControl(unsigned char value);
		void setFreeFallThresh(unsigned char value);
		void setFreeFallTime(unsigned char value);
		void setTapAxes(unsigned char value);
		void setBitrate(unsigned char value);
		void setPowerControl(unsigned char value);
		void setInterruptEnable(unsigned char value);
		void setInterruptSource(unsigned char value);
		void setDataFormat(unsigned char value);
		void setFIFOControl(unsigned char value);

};

ADXL345::ADXL345(unsigned char channel) {
	recvLine.init(channel,0x3B);
	sendLine.init(channel,0x3A);
};

ADXL345::ADXL345(unsigned char channel,bool alt) {
	if(alt) {
		recvLine.init(channel,0x3B);
		sendLine.init(channel,0x3A);
	} else {
		recvLine.init(channel,0xA7);
		sendLine.init(channel,0xA6);
	}
};


unsigned char ADXL345::getByte(char addr) {
	char buf[2];
	buf[0] = addr; buf[1] = '\0';
	sendLine.send(buf,1);
	recvLine.recv(buf,1);
	return buf[0];
}

void ADXL345::sendByte(char addr, char value) {
	char buf[3];
	buf[0] = 0x1D; buf[1] = value; buf[2] = '\0';
	sendLine.send(buf,2);
}


//Read Methods
unsigned char ADXL345::getDevId() {
	return getByte(0x00);
}

unsigned char ADXL345::getTapThreshold() {
	return getByte(0x1D);
}

char ADXL345::getXaxisOffset() {
	return (char)getByte(0x1E);
}


char ADXL345::getYaxisOffset() {
	return (char)getByte(0x1F);
}


char ADXL345::getZaxisOffset() {
	return (char)getByte(0x20);
}


unsigned char ADXL345::getTapDuration() {
	return getByte(0x21);
}


unsigned char ADXL345::getTapLatency() {
	return getByte(0x22);
}


unsigned char ADXL345::getTapWindow() {
	return getByte(0x23);
}


unsigned char ADXL345::getActivityThreshold() {
	return getByte(0x24);
}


unsigned char ADXL345::getInactivityThreshold() {
	return getByte(0x25);
}


unsigned char ADXL345::getInactivityTime() {
	return getByte(0x26);
}


unsigned char ADXL345::getActInactControl() {
	return getByte(0x27);
}


unsigned char ADXL345::getFreeFallThresh() {
	return getByte(0x28);
}


unsigned char ADXL345::getFreeFallTime() {
	return getByte(0x29);
}


unsigned char ADXL345::getTapAxes() {
	return getByte(0x2A);
}


unsigned char ADXL345::getTapStatus() {
	return getByte(0x2B);
}


unsigned char ADXL345::getBitrate() {
	return getByte(0x2C);
}


unsigned char ADXL345::getPowerControl() {
	return getByte(0x2D);
}


unsigned char ADXL345::getInterruptEnable() {
	return getByte(0x2E);
}


unsigned char ADXL345::getInterruptMap() {
	return getByte(0x2F);
}


unsigned char ADXL345::getInterruptSource() {
	return getByte(0x30);
}


unsigned char ADXL345::getDataFormat() {
	return getByte(0x31);
}


unsigned short int ADXL345::getXAccel() {
	unsigned char lsb = getByte(0x32);
	unsigned char msb = getByte(0x33);

	return (((unsigned short int)msb) << 8) + lsb;
}


unsigned short int ADXL345::getYAccel() {
	unsigned char lsb = getByte(0x34);
	unsigned char msb = getByte(0x35);

	return (((unsigned short int)msb) << 8) + lsb;
}


unsigned short int ADXL345::getZAccel() {
	unsigned char lsb = getByte(0x36);
	unsigned char msb = getByte(0x37);

	return (((unsigned short int)msb) << 8) + lsb;
}


unsigned char ADXL345::getFIFOControl() {
	return getByte(0x38);
}


unsigned char ADXL345::getFIFOStatus() {
	return getByte(0x39);
}



//Write Methods

//Scale Factor = 62.5mg/LSB
void ADXL345::setTapThreshold(unsigned char value) {
	sendByte(0x1D,value);
}

//Scale Factor = 15.6mg/LSB
void ADXL345::setXaxisOffset(char value) {
	sendByte(0x1E,value);
}


void ADXL345::setYaxisOffset(char value) {
	sendByte(0x1F,value);
}


void ADXL345::setZaxisOffset(char value) {
	sendByte(0x20,value);
}


void ADXL345::setTapDuration(unsigned char value) {
	sendByte(0x21,value);
}


void ADXL345::setTapLatency(unsigned char value) {
	sendByte(0x22,value);
}


void ADXL345::setTapWindow(unsigned char value) {
	sendByte(0x23,value);
}


void ADXL345::setActivityThreshold(unsigned char value) {
	sendByte(0x24,value);
}


void ADXL345::setInactivityThreshold(unsigned char value) {
	sendByte(0x25,value);
}


void ADXL345::setInactivityTime(unsigned char value) {
	sendByte(0x26,value);
}


void ADXL345::setActInactControl(unsigned char value) {
	sendByte(0x27,value);
}


void ADXL345::setFreeFallThresh(unsigned char value) {
	sendByte(0x28,value);
}


void ADXL345::setFreeFallTime(unsigned char value) {
	sendByte(0x29,value);
}


void ADXL345::setTapAxes(unsigned char value) {
	sendByte(0x2A,value);
	
}


void ADXL345::setBitrate(unsigned char value) {
	sendByte(0x2C,value);
}


void ADXL345::setPowerControl(unsigned char value) {
	sendByte(0x2D,value);
}


void ADXL345::setInterruptEnable(unsigned char value) {
	sendByte(0x2E,value);
}


void ADXL345::setInterruptSource(unsigned char value) {
	sendByte(0x2F,value);
}


void ADXL345::setDataFormat(unsigned char value) {
	sendByte(0x31,value);
}


void ADXL345::setFIFOControl(unsigned char value) {
	sendByte(0x38,value);
}



#endif
