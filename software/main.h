#ifndef MAIN_H
#define MAIN_H

//Defines
#define IMU_RATE 			0.0012
#define TAKEOFF_ALTITUDE 	48
#define ORIENT_DEADBAND 	1
#define ALTITUDE_DEADBAND 	4
#define REGULATION_RATE		0.05

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
void get_motors();
void set_motors();
bool takeoff();
bool flight_altitude(unsigned short int desired);
void flight_stabilize(Vector3 desired);

#endif
