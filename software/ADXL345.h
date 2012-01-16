#ifndef ADXL345_H
#define ADXL345_H

#include "I2C.h"
#include "util.h"
#include <stdlib.h>


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
		unsigned short int getTapThreshold();
		short int getXaxisOffset();
		short int getYaxisOffset();
		short int getZaxisOffset();
		unsigned int getTapDuration();
		unsigned short int getTapLatency();
		unsigned short int getTapWindow();
		unsigned int getActivityThreshold();
		unsigned int getInactivityThreshold();
		unsigned char getInactivityTime();
		unsigned char getActInactControl();
			bool isActCoupledAC();
			bool isActXEnabled();
			bool isActYEnabled();
			bool isActZEnabled();
			bool isInactCoupledAC();
			bool isInactXEnabled();
			bool isInactYEnabled();
			bool isInactZEnabled();
		unsigned int getFreeFallThresh();
		unsigned short int getFreeFallTime();
		unsigned char getTapAxes();
			bool hasSuppressedDoubleTap();
			bool isTapXEnabled();
			bool isTapYEnabled();
			bool isTapZEnabled();
		unsigned char getTapStatus();
			bool isActXSource();
			bool isActYSource();
			bool isActZSource();
			bool isAsleep();
			bool isTapXSource();
			bool isTapYSource();
			bool isTapZSource();
		unsigned char getBitrateRegister();
			bool isLowPowerMode();
			unsigned char getRate();
		unsigned char getPowerControl();
			bool isLinkMode();
			bool isAutoSleepMode();
			bool isMeasurementMode();
			bool isSleepMode();
			unsigned char getReadFreq();
		unsigned char getInterruptEnable();
			bool isDataReadyIntEnabled();
			bool isSingleTapIntEnabled();
			bool isDoubleTapIntEnabled();
			bool isActivityIntEnabled();
			bool isInactivityIntEnabled();
			bool isFreeFallIntEnabled();
			bool isWatermarkIntEnabled();
			bool isOverrunIntEnabled();
		unsigned char getInterruptMap();
			bool isDataReadyInt2();
			bool isSingleTapInt2();
			bool isDoubleTapInt2();
			bool isActivityInt2();
			bool isInactivityInt2();
			bool isFreeFallInt2();
			bool isWatermarkInt2();
			bool isOverrunInt2();
		unsigned char getInterruptSource();
			bool hasDataReadyIntTriggered();
			bool hasSingleTapIntTriggered();
			bool hasDoubleTapIntTriggered();
			bool hasActivityIntTriggered();
			bool hasInactivityIntTriggered();
			bool hasFreeFallIntTriggered();
			bool hasWatermarkIntTriggered();
			bool hasOverrunIntTriggered();
		unsigned char getDataFormat();
			bool isSelfTestEnabled();
			bool whichSPI();
			bool isIntActiveLow();
			bool isFullResMode();
			bool isJustifiedMSB();
			unsigned char getDataRange();
		unsigned short int getXAccel();
		unsigned short int getYAccel();
		unsigned short int getZAccel();
		unsigned char getFIFOControl();
			unsigned char getFIFOMode();
			bool isFIFOEventInt2();
			unsigned char getSamples();
		unsigned char getFIFOStatus();
			bool hasFIFOTriggered();
			unsigned char getEntries();

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

//Constructor
//channel = I2C Channel to connect to
ADXL345::ADXL345(unsigned char channel) {
	recvLine.init(channel,0x3B);
	sendLine.init(channel,0x3A);
};

//Constructor
//channel = I2C Channel to connect to
//alt = Alternate read/write chip devices
ADXL345::ADXL345(unsigned char channel,bool alt) {
	if(alt) {
		recvLine.init(channel,0x3B);
		sendLine.init(channel,0x3A);
	} else {
		recvLine.init(channel,0xA7);
		sendLine.init(channel,0xA6);
	}
};


//Read a register from the device
unsigned char ADXL345::getByte(char addr) {
	char buf[2];
	buf[0] = addr; buf[1] = '\0';
	sendLine.send(buf,1);
	recvLine.recv(buf,1);
	return buf[0];
}

//Write to a register
void ADXL345::sendByte(char addr, char value) {
	char buf[3];
	buf[0] = 0x1D; buf[1] = value; buf[2] = '\0';
	sendLine.send(buf,2);
}


//Read Methods 

//Should be decimal value 345
unsigned char ADXL345::getDevId() {
	return getByte(0x00);
}

//Return value in mg
//Range: 0 to +16000mg
unsigned short int ADXL345::getTapThreshold() {
	return 62.5*(unsigned short int)getByte(0x1D);
}

//Return value in mg
//Range: -2000mg to +2000mg
short int ADXL345::getXaxisOffset() {
	return 15.6*(short int)getByte(0x1E);
}

//Return value in mg
//Range: -2000mg to +2000mg
short int ADXL345::getYaxisOffset() {
	return 15.6*(short int)getByte(0x1F);
}

//Return value in mg
//Range: -2000mg to +2000mg
short int ADXL345::getZaxisOffset() {
	return 15.6*(short int)getByte(0x20);
}

//Return value in us
//Range: 0 to 160000us
unsigned int ADXL345::getTapDuration() {
	return 625*(unsigned int)getByte(0x21);
}

//Return value in ms
//Range: 0 to 320ms
unsigned short int ADXL345::getTapLatency() {
	return (125*(unsigned short int)getByte(0x22))/100;
}

//Return value in ms
//Range: 0 to 320ms
unsigned short int ADXL345::getTapWindow() {
	return (125*(unsigned short int)getByte(0x23))/100;
}

//Return value in us
//Range: 0 to 160000us
unsigned int ADXL345::getActivityThreshold() {
	return 625*(unsigned int)getByte(0x24);
}

//Return value in us
//Range: 0 to 160000us
unsigned int ADXL345::getInactivityThreshold() {
	return 625*(unsigned int)getByte(0x25);
}

//Return value in s
//Range: 0 to 255us
unsigned char ADXL345::getInactivityTime() {
	return getByte(0x26);
}

//Get the Whole Register
unsigned char ADXL345::getActInactControl() {
	return getByte(0x27);
}

	//Get a boolean based on if the register bits are set
	bool ADXL345::isActCoupledAC() {
		return (getActInactControl() && 0x80);
	}

	bool ADXL345::isActXEnabled() {
		return (getActInactControl() && 0x40);
	}

	bool ADXL345::isActYEnabled() {
		return (getActInactControl() && 0x20);
	}

	bool ADXL345::isActZEnabled() {
		return (getActInactControl() && 0x10);
	}

	bool ADXL345::isInactCoupledAC() {
		return (getActInactControl() && 0x08);
	}

	bool ADXL345::isInactXEnabled() {
		return (getActInactControl() && 0x04);
	}

	bool ADXL345::isInactYEnabled() {
		return (getActInactControl() && 0x02);
	}

	bool ADXL345::isInactZEnabled() {
		return (getActInactControl() && 0x01);
	}

//Return value in us
//Range: 0 to 160000us
unsigned int ADXL345::getFreeFallThresh() {
	return 625*(unsigned int)getByte(0x28);
}

//Return value in ms
//Range: 0 to 1280ms
unsigned short int ADXL345::getFreeFallTime() {
	return 5*(unsigned short int)getByte(0x29);
}

//Get Whole Register
unsigned char ADXL345::getTapAxes() {
	return getByte(0x2A);
}

	//Is the Double Tap Suppression Enabled
	bool ADXL345::hasSuppressedDoubleTap() {
		return (getByte(0x2A) && 0x08);
	}

	//Is the X Tap Enabled
	bool ADXL345::isTapXEnabled() {
		return (getByte(0x2A) && 0x04);
	}

	//Is the Y Tap Enabled
	bool ADXL345::isTapYEnabled() {
		return (getByte(0x2A) && 0x02);
	}

	//Is the Z Tap Enabled
	bool ADXL345::isTapZEnabled() {
		return (getByte(0x2A) && 0x01);
	}

//Get the Whole register
unsigned char ADXL345::getTapStatus() {
	return getByte(0x2B);
}

	//Is the axis involved in an activity
	bool ADXL345::isActXSource() {
		return (getByte(0x2B) && 0x40);
	}

	//Is the axis involved in an activity
	bool ADXL345::isActYSource() {
		return (getByte(0x2B) && 0x20);
	}

	//Is the axis involved in an activity
	bool ADXL345::isActZSource() {
		return (getByte(0x2B) && 0x10);
	}

	//Is it asleep
	bool ADXL345::isAsleep() {
		return (getByte(0x2B) && 0x08);
	}

	//Is the axis involved in a tap
	bool ADXL345::isTapXSource() {
		return (getByte(0x2B) && 0x04);
	}

	//Is the axis involved in a tap
	bool ADXL345::isTapYSource() {
		return (getByte(0x2B) && 0x02);
	}

	//Is the axis involved in a tap
	bool ADXL345::isTapZSource() {
		return (getByte(0x2B) && 0x01);
	}


//Get the Whole Register
unsigned char ADXL345::getBitrateRegister() {
	return getByte(0x2C);
}

	//Check if it is in low pwr mode
	bool ADXL345::isLowPowerMode() {
		return (getByte(0x2C) && 0x10);
	}

	//Get the actual Bit rate
	unsigned char ADXL345::getRate() {
		return (getByte(0x2D) & 0x0F);
	}


//Get the Whole Register
unsigned char ADXL345::getPowerControl() {
	return getByte(0x2D);
}

	//Is the Link Mode Active
	bool ADXL345::isLinkMode() {
		return (getByte(0x2D) && 0x20);
	}

	//Is the Auto Sleep Mode Active
	bool ADXL345::isAutoSleepMode() {
		return (getByte(0x2D) && 0x10);
	}

	//Is the Auto Sleep Mode Active
	bool ADXL345::isMeasurementMode() {
		return (getByte(0x2D) && 0x08);
	}

	//Is the Auto Sleep Mode Active
	bool ADXL345::isSleepMode() {
		return (getByte(0x2D) && 0x04);
	}

	//Frequency of Readings
	unsigned char ADXL345::getReadFreq() {
		unsigned char v = getByte(0x2D);
		return (v & 0x03);
	}


//Get the Whole Register
unsigned char ADXL345::getInterruptEnable() {
	return getByte(0x2E);
}

	//Is Data Ready Interrupt Enabled?
	bool ADXL345::isDataReadyIntEnabled() {
		return (getByte(0x2E) && 0x80);
	}

	//Is Single Tap Interrupt Enabled?
	bool ADXL345::isSingleTapIntEnabled() {
		return (getByte(0x2E) && 0x40);
	}

	//Is Double Tap Interrupt Enabled?
	bool ADXL345::isDoubleTapIntEnabled() {
		return (getByte(0x2E) && 0x20);
	}

	//Is Activity Interrupt Enabled?
	bool ADXL345::isActivityIntEnabled() {
		return (getByte(0x2E) && 0x10);
	}

	//Is Inactivity Interrupt Enabled?
	bool ADXL345::isInactivityIntEnabled() {
		return (getByte(0x2E) && 0x08);
	}

	//Is Free Fall Interrupt Enabled?
	bool ADXL345::isFreeFallIntEnabled() {
		return (getByte(0x2E) && 0x04);
	}

	//Is Watermark Interrupt Enabled?
	bool ADXL345::isWatermarkIntEnabled() {
		return (getByte(0x2E) && 0x02);
	}

	//Is Overrun Interrupt Enabled?
	bool ADXL345::isOverrunIntEnabled() {
		return (getByte(0x2E) && 0x01);
	}


//Get the Whole Register
unsigned char ADXL345::getInterruptMap() {
	return getByte(0x2F);
}

	//Is Data Ready Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isDataReadyInt2() {
		return (getByte(0x2F) && 0x80);
	}

	//Is Single Tap Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isSingleTapInt2() {
		return (getByte(0x2F) && 0x40);
	}

	//Is Double Tap Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isDoubleTapInt2() {
		return (getByte(0x2F) && 0x20);
	}

	//Is Activity Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isActivityInt2() {
		return (getByte(0x2F) && 0x10);
	}

	//Is Inactivity Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isInactivityInt2() {
		return (getByte(0x2F) && 0x08);
	}

	//Is Free Fall Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isFreeFallInt2() {
		return (getByte(0x2F) && 0x04);
	}

	//Is Watermark Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isWatermarkInt2() {
		return (getByte(0x2F) && 0x02);
	}

	//Is Overrun Interrupt On INT1(F), or INT2(T)?
	bool ADXL345::isOverrunInt2() {
		return (getByte(0x2F) && 0x01);
	}

//Get the whole register
unsigned char ADXL345::getInterruptSource() {
	return getByte(0x30);
}

	//Has Data Ready Interrupt Triggered?
	bool ADXL345::hasDataReadyIntTriggered() {
		return (getByte(0x30) && 0x80);
	}

	//Has Single Tap Interrupt Triggered?
	bool ADXL345::hasSingleTapIntTriggered() {
		return (getByte(0x30) && 0x40);
	}

	//Has Double Tap Interrupt Triggered?
	bool ADXL345::hasDoubleTapIntTriggered() {
		return (getByte(0x30) && 0x20);
	}

	//Has Activity Interrupt Triggered?
	bool ADXL345::hasActivityIntTriggered() {
		return (getByte(0x30) && 0x10);
	}

	//Has Inactivity Interrupt Triggered?
	bool ADXL345::hasInactivityIntTriggered() {
		return (getByte(0x30) && 0x08);
	}

	//Has Free Fall Interrupt Triggered?
	bool ADXL345::hasFreeFallIntTriggered() {
		return (getByte(0x30) && 0x04);
	}

	//Has Watermark Interrupt On Triggered?
	bool ADXL345::hasWatermarkIntTriggered() {
		return (getByte(0x30) && 0x02);
	}

	//Has Overrun Interrupt On Triggered?
	bool ADXL345::hasOverrunIntTriggered() {
		return (getByte(0x30) && 0x01);
	}


//Get the whole register
unsigned char ADXL345::getDataFormat() {
	return getByte(0x31);
}

	//Is the self test enabled?
	bool ADXL345::isSelfTestEnabled() {
		return (getByte(0x31) && 0x80);
	}

	//Which SPI Type?
	bool ADXL345::whichSPI() {
		return (getByte(0x31) && 0x40);
	}

	//Is the interrupts active low?
	bool ADXL345::isIntActiveLow() {
		return (getByte(0x31) && 0x20);
	}

	//Is the chip in full resolution mode?
	bool ADXL345::isFullResMode() {
		return (getByte(0x31) && 0x08);
	}

	//Is the Justify Bit, Most significant?
	bool ADXL345::isJustifiedMSB() {
		return (getByte(0x31) && 0x04);
	}

	//Is the Justify Bit, Most significant?
	unsigned char ADXL345::getDataRange() {
		return (getByte(0x31) & 0x03);
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

	//Get the FIFO Mode
	unsigned char ADXL345::getFIFOMode() {
		return (getByte(0x38) & 0xB0);
	}

	//Is the trigger event int2?
	bool ADXL345::isFIFOEventInt2() {
		return (getByte(0x38) && 0x10);
	}

	//Get the FIFO Samples
	unsigned char ADXL345::getSamples() {
		return (getByte(0x38) & 0x0F);
	}

unsigned char ADXL345::getFIFOStatus() {
	return getByte(0x39);
}

	//Has the FIFO Int Triggered?
	bool ADXL345::hasFIFOTriggered() {
		return (getByte(0x39) && 0x80);
	}

	//Get the num Entrie in FIFO?
	unsigned char ADXL345::getEntries() {
		return (getByte(0x30) & 0x3F);
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
