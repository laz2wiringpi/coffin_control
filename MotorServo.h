/*
MotorServo.h
Version .01

*/
#ifndef MotorServo_h
#define MotorServo_h

 #define DEBUG


#include "Arduino.h"
/*


#define DEFAULT_PIN_POT A0
#define DEFAULT_PIN_MOTOR 5
#define DEFAULT_PIN_MOTOR_UP 7
#define DEFAULT_PIN_MOTOR_DOWN 8
*/
#define numPotReadings 5


//#define DEFAULT_PIN_POT_MIN 500
//#define DEFAULT_PIN_POT_MAX 800
//#define DEFAULT_PIN_POT_DTG  20
//#define DEFAULT_PIN_MOTOR_START  150


#define DIRECTION_OFF 0
#define DIRECTION_RUN  1 
#define DIRECTION_UP  2
#define DIRECTION_DOWN  3 
#define DIRECTION_UP_BRAKE  4
#define DIRECTION_DOWN_BRAKE  5  


class MotorServo
{
public:

	    MotorServo(int aPIN_POT ,
		int aPIN_MOTOR ,
		int aPIN_MOTOR_UP ,
		int aPIN_MOTOR_DOWN );

		int Stop();
		int Run(int apot_avrage);
	    int check();
		int GetDIRECTION();
		int Settarget_pos(int atarget_pos);
		int Gettarget_pos();
    int MAX_BREAK;
		int POT_MIN;
		int POT_MAX;
 		int MOTOR_START;
		int POT_DTG;
	//	int MOTOR_MIN;
		int MOTOR_MAX;

		int Current_pot();


        long target_vol ;
		int VOL_FACTOR ;
	  boolean stop_after_run;

   // avg pot readings 
   
   int Pot_readings[numPotReadings];      // the readings from the analog input
   int readPotIndex;              // the index of the current reading
   int totalPot;                  // the running total
    
   
private:
 
	int _PIN_POT ;
	int _PIN_MOTOR ;
	int _PIN_MOTOR_UP ;
	int _PIN_MOTOR_DOWN  ;

 
	
	int _last_potavg  ;
	int _last_speed  ;
	unsigned long _last_ms  ;
	int _DIRECTION   ;
	int _last_direction  ;
	int _target_pos  ;
  void   donot_burn_out_hbridge( int mpin , int dummyvalue  );
};


 

#endif
