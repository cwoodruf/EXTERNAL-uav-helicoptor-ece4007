#ifndef PID_H
#define PID_H

#include "../util.h"
#include <stdint.h>

class PID {

	private:
		uint16_t P;
		uint16_t I;
		uint16_t D;
		uint16_t ts;
		uint16_t max_output;
		uint16_t old_output;
		int32_t G0;
		int32_t G1;
		int32_t G2;
		int16_t Ew1;
		int16_t Ew2;
		
		int status;

	public:
		PID();

		int init(uint16_t oP, uint16_t oI, uint16_t oD, uint16_t ots, uint16_t omax_output);
		int regulate(uint16_t setpoint, uint16_t actual, uint16_t &output);

};

PID::PID() : status(-100) {
}

int PID::init(uint16_t oP, uint16_t oI, uint16_t oD, uint16_t ots, uint16_t omax_output) {
	status = 0;

	P = oP; I = oI; D = oD; ts = ots; max_output = omax_output;
	Ew1 = 0;
	Ew2 = 0;

	G0 = P * (100 + (int32_t)D * 100 / ts) / 100;
	G2 = ((int32_t)P * D * 100 / ts) / 100;

	if((D == 0) && (P == 0)) {
		//I Only
		G1 = (int32_t) ts * 100 / I;
	} else {
		if((D == 0) && (I == 0)) {
			//P Only
			G1 = 0;
		} else {
			G1 = (P * (((int32_t)ts * 100) / I - 200 * (int32_t) D / ts - 100)) / 100;
		}
	}

	//Check for overflow
	if( (G0 > 0x7FFF) ||
		(G1 > 0x7FFF) ||
		(G2 > 0x7FFF) ||
		(G0 < -0x7FFF) ||
		(G1 < -0x7FFF) ||
		(G2 < -0x7FFF)) {
		status = -1;
		return status;
	}

	if(((ABSV(G1) < G0) && (G0 != 0) && (G1 != 0) && (G2 != 0)) ||
	   ((G1 > 0) && (G0 != 0)) ||
	   ((ABSV(G0+G1) > G2) && (G0 != 0) && (G1 != 0) && (G2 != 0))) {

		status = -2;
		return status;
	}


	return status;
}

int PID::regulate(uint16_t setpoint, uint16_t actual, uint16_t &output) {

	int32_t newV;
	int16_t EwNew;
	int32_t diffNew;
	

	if(setpoint > 0x7FFF) {
		return -1;
	}


	EwNew = (int16_t)setpoint - (int16_t)actual;
	diffNew = (G0 * (int32_t) EwNew + 
			   G1 * (int32_t) Ew1 +
			   G2 * (int32_t) Ew2) / 100;

	if((G1 == 0) && (G2 == 0)) {
		newV = diffNew;
	} else {
		newV = old_output + diffNew;
	}

	newV = LIMIT(0,newV,max_output);

	Ew2 = Ew1;
	Ew1 = EwNew;
	old_output = newV;
	output = newV;

	return 0;
}

#endif
