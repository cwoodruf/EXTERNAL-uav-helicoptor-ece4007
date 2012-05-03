#ifndef MAIN_H
#define MAIN_H

//Defines
#define TAKEOFF_ALTITUDE 	48
#define ORIENT_DEADBAND 	1
#define MIN_ORIENT			-15
#define MAX_ORIENT			15
#define MIN_CRIT_ANGLE		-30
#define MAX_CRIT_ANGLE		30
#define ALTITUDE_DEADBAND 	4
#define MIN_ALTITUDE		24
#define MAX_ALTITUDE		144
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

//Prototypes
void fatal_err();
void error_log(const char *data);
void imu_loop();
bool sys_setup();
bool io_setup();
bool comm_setup();
void get_motors();
void set_motors();
void get_orient();
void get_altitude();
bool takeoff();
bool landing();
bool flight_altitude(unsigned short int desired);
void flight_stabilize(int *desired);
void safety_checks();

#endif
