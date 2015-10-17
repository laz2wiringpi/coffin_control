/*
MotorServo.h
Version .01

*/
#ifndef MotorServo_h
#define MotorServo_h

 #define DEBUG


#include "Arduino.h"


enum DIRECTIONS {
	DIRECTION_OFF,
	DIRECTION_UP,
	DIRECTION_DOWN
	//,DIRECTION_UP_BRAKE  
	//,DIRECTION_DOWN_BRAKE  
};




class MotorServo
{
public:

	MotorServo(byte aPIN_POT,
		byte aPIN_MOTOR,
		byte aPIN_MOTOR_UP,
		byte aPIN_MOTOR_DOWN);

	int StopMotor();
	int Goto(int target);
	int check();

	//    int MAX_BREAK;
	int POT_MIN;
	int POT_MAX;
	byte UP_SPEED;
	byte DOWN_SPEED;


	int Current_pot();
	DIRECTIONS GetDIRECTION();
	int timepermove;




private:
	DIRECTIONS _DIRECTION;
	DIRECTIONS _lastdirection;
	byte _PIN_POT;
	byte _PIN_MOTOR;
	byte _PIN_MOTOR_UP;
	byte _PIN_MOTOR_DOWN;
	int  _target;
	

	int  _lastpot;
	unsigned long _last_time;
	int _last_speed;

	void   donot_burn_out_hbridge(byte mpin, byte mspeed);
};




#endif
