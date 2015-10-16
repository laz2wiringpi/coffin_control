/*
MotorServo.cpp - Library for flashing Morse code.
Created by David A. Mellis, November 2, 2007.
Released into the public domain.
*/

#include "Arduino.h"
#include "MotorServo.h"

////////////////////////////////////////
MotorServo::MotorServo(int aPIN_POT,
	int aPIN_MOTOR,
	int aPIN_MOTOR_UP,
	int aPIN_MOTOR_DOWN)
{

	 

	POT_MIN = 500;
	POT_MAX = 800;
	UP_SPEED = 100;
 
	DOWN_SPEED  = 250;
	 
	//MAX_BREAK = 5;
 
	  


	_PIN_POT = aPIN_POT;
	_PIN_MOTOR = aPIN_MOTOR;
	_PIN_MOTOR_UP = aPIN_MOTOR_UP;
	_PIN_MOTOR_DOWN = aPIN_MOTOR_DOWN;

	pinMode(_PIN_POT, INPUT);

	pinMode(_PIN_MOTOR, OUTPUT);
	analogWrite(_PIN_MOTOR, 0);

	pinMode(_PIN_MOTOR_UP, OUTPUT);
	digitalWrite(_PIN_MOTOR_UP, LOW);

	pinMode(_PIN_MOTOR_DOWN, OUTPUT);
	digitalWrite(_PIN_MOTOR_DOWN, LOW);
}


////////////////////////////////////////
int MotorServo::StopMotor(){

	// wee need to seend a stop 
	//if (_last_speed > 0) {

	_DIRECTION = DIRECTION_OFF;

	analogWrite(_PIN_MOTOR, 0);
	digitalWrite(_PIN_MOTOR_UP, LOW);
	digitalWrite(_PIN_MOTOR_DOWN, LOW);
 
	return analogRead(_PIN_POT);

}

void  MotorServo::donot_burn_out_hbridge(int mpin, int speed){
	analogWrite(_PIN_MOTOR, 0);
	if (mpin == _PIN_MOTOR_UP) {
		digitalWrite(_PIN_MOTOR_DOWN, LOW);
		delay(1);
		digitalWrite(_PIN_MOTOR_UP, HIGH);
	}
	if (mpin == _PIN_MOTOR_DOWN){
		digitalWrite(_PIN_MOTOR_UP, LOW);
		delay(1);
		digitalWrite(_PIN_MOTOR_DOWN, HIGH);
	}
	analogWrite(_PIN_MOTOR, speed);
}


////////////////////////////////////////
int MotorServo::Goto(int target)  {


	 


	analogWrite(_PIN_MOTOR, 0);
	digitalWrite(_PIN_MOTOR_UP, LOW);
	digitalWrite(_PIN_MOTOR_DOWN, LOW);
	if (target > POT_MAX){
		_target = POT_MAX;
	}
	else if (target < POT_MIN)
	{
		_target = POT_MIN;
	}
	else
	{
		_target = target;

	}

	int pospot = Current_pot();
	if (target > pospot){

		_DIRECTION = DIRECTION_UP;

	}
	else if (target < pospot){


		_DIRECTION = DIRECTION_DOWN ;
	}

	return	_target;


	 
}
////////////////////////////////////////
int  MotorServo::check(){

	switch (_DIRECTION)
	{
	case DIRECTION_OFF:
		if (_lastdirection != DIRECTION_OFF);
		StopMotor();
		return 0;
		break;

	case DIRECTION_UP:
		if (Current_pot() > POT_MAX) {
			StopMotor();
			return 0; 
		}
		else{

		if (_lastdirection != DIRECTION_UP);
		   donot_burn_out_hbridge(_PIN_MOTOR_UP,UP_SPEED);
		}

		break;
	case DIRECTION_DOWN:
	 
		if (Current_pot() < POT_MIN ) {
			StopMotor();
			return 0;
		}
		else{

			if (_lastdirection != DIRECTION_DOWN);
			donot_burn_out_hbridge(_PIN_MOTOR_UP, UP_SPEED);

		}

		break;
//	case DIRECTION_UP_BRAKE:
//		break;
//	case DIRECTION_DOWN_BRAKE:
//		break;
	default:
		break;
	}
	_lastdirection = _DIRECTION;

}

 

int MotorServo::Current_pot(){

	return analogRead(_PIN_POT);
}






