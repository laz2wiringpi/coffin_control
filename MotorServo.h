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

	    MotorServo(int aPIN_POT ,
		int aPIN_MOTOR ,
		int aPIN_MOTOR_UP ,
		int aPIN_MOTOR_DOWN );

		int StopMotor();
		int Goto(int target);
	    int check();
		 
    //    int MAX_BREAK;
		int POT_MIN;
		int POT_MAX;
		int UP_SPEED;
		int DOWN_SPEED;
	 

		int Current_pot();
		


   
   
private:
	DIRECTIONS _DIRECTION;
	DIRECTIONS _lastdirection;
	int _PIN_POT ;
	int _PIN_MOTOR ;
	int _PIN_MOTOR_UP ;
	int _PIN_MOTOR_DOWN  ;
	int  _target;

  void   donot_burn_out_hbridge( int mpin , int mspeed  );
};


 

#endif
