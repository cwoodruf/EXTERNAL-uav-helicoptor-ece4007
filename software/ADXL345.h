#ifndef ADXL345_H
#define ADXL345_H

#include "I2C.h"
#include "util.h"
#include <stdlib.h>

class ADXL345 {

	private:
		I2C recvLine;
		I2C sendLine;

	public:
		ADXL345(unsigned char channel);
		ADXL345(unsigned char channel,bool alt);

		

		//Read Methods
		int getDevId();
		int getTapThreshold();
		int getXaxisOffset();
		int getYaxisOffset();
		int getZaxisOffset();
		int getTapDuration();
		int getTapLatency();
		int getTapWindow();
		int getActivityThreshold();
		int getInactivityThreshold();
		int getInactivityTime();
		int getActInactControl();
		int getFreeFallThresh();
		int getFreeFallTime();
		int getTapAxes();
		int getTapStatus();
		int getBitrate();
		int getPowerControl();
		int getInterruptEnable();
		int getInterruptMap();
		int getInterruptSource();
		int getDataFormat();
		int getXAccel();
		int getYAccel();
		int getZAccel();
		int getFIFOControl();
		int getFIFOStatus();

		//Write Methods
		void setTapThreshold(int value);
		void setXaxisOffset(int value);
		void setYaxisOffset(int value);
		void setZaxisOffset(int value);
		void setTapDuration(int value);
		void setTapLatency(int value);
		void setTapWindow(int value);
		void setActivityThreshold(int value);
		void setInactivityThreshold(int value);
		void setInactivityTime(int value);
		void setActInactControl(int value);
		void setFreeFallThresh(int value);
		void setFreeFallTime(int value);
		void setTapAxes(int value);
		void setBitrate(int value);
		void setPowerControl(int value);
		void setInterruptEnable(int value);
		void setInterruptSource(int value);
		void setDataFormat(int value);
		void setFIFOControl(int value);

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


//Read Methods
int ADXL345::getDevId() {
	char buf[2];
	buf[0] = 0x00; buf[1] = '\0';
	sendLine.send(buf,1);
	recvLine.recv(buf,1);
	return atoi(buf);
}

int ADXL345::getTapThreshold() {
	char buf[2];
	buf[0] = 0x1D; buf[1] = '\0';
	sendLine.send(buf,1);
	recvLine.recv(buf,1);
	return atoi(buf);
}

int ADXL345::getXaxisOffset() {
	
}


int ADXL345::getYaxisOffset() {
	
}


int ADXL345::getZaxisOffset() {
	
}


int ADXL345::getTapDuration() {
	
}


int ADXL345::getTapLatency() {
	
}


int ADXL345::getTapWindow() {
	
}


int ADXL345::getActivityThreshold() {
	
}


int ADXL345::getInactivityThreshold() {
	
}


int ADXL345::getInactivityTime() {
	
}


int ADXL345::getActInactControl() {
	
}


int ADXL345::getFreeFallThresh() {
	
}


int ADXL345::getFreeFallTime() {
	
}


int ADXL345::getTapAxes() {
	
}


int ADXL345::getTapStatus() {
	
}


int ADXL345::getBitrate() {
	
}


int ADXL345::getPowerControl() {
	
}


int ADXL345::getInterruptEnable() {
	
}


int ADXL345::getInterruptMap() {
	
}


int ADXL345::getInterruptSource() {
	
}


int ADXL345::getDataFormat() {
	
}


int ADXL345::getXAccel() {
	
}


int ADXL345::getYAccel() {
	
}


int ADXL345::getZAccel() {
	
}


int ADXL345::getFIFOControl() {
	
}


int ADXL345::getFIFOStatus() {
	
}



//Write Methods

//Scale Factor = 62.5mg/LSB
//0-255 input
void ADXL345::setTapThreshold(int value) {
	char buf[3];
	buf[0] = 0x1D; buf[1] = (char) LIMIT(0,value,255);
	sendLine.send(buf,2);
}


void ADXL345::setXaxisOffset(int value) {
	
}


void ADXL345::setYaxisOffset(int value) {
	
}


void ADXL345::setZaxisOffset(int value) {
	
}


void ADXL345::setTapDuration(int value) {
	
}


void ADXL345::setTapLatency(int value) {
	
}


void ADXL345::setTapWindow(int value) {
	
}


void ADXL345::setActivityThreshold(int value) {
	
}


void ADXL345::setInactivityThreshold(int value) {
	
}


void ADXL345::setInactivityTime(int value) {
	
}


void ADXL345::setActInactControl(int value) {
	
}


void ADXL345::setFreeFallThresh(int value) {
	
}


void ADXL345::setFreeFallTime(int value) {
	
}


void ADXL345::setTapAxes(int value) {
	
}


void ADXL345::setBitrate(int value) {
	
}


void ADXL345::setPowerControl(int value) {
	
}


void ADXL345::setInterruptEnable(int value) {
	
}


void ADXL345::setInterruptSource(int value) {
	
}


void ADXL345::setDataFormat(int value) {
	
}


void ADXL345::setFIFOControl(int value) {
	
}



#endif
