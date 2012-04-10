#ifndef MAIN_H
#define MAIN_H

//Defines
#define RATE 0.0012

//Enumerators
typedef enum _eSTATE {
	eSETUP,
	eGROUND,
	eTAKEOFF,
	eFLY,
	eLAND,
	eERR
}eSTATE;

typedef enum _eCONTROLLER {
	eLOCAL,
	eREMOTE
}eCONTROLLER;

//Prototypes
void fatal_err();
void error_log(const char *data);
void imu_loop();
bool sys_setup();
bool io_setup();
bool comm_setup();


#endif
