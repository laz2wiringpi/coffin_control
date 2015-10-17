/*
MotorServo.cpp - Library for flashing Morse code.
Created by David A. Mellis, November 2, 2007.
Released into the public domain.
*/

#include "Arduino.h"
#include "Fog.h"


////////////////////////////////////////
Fog::Fog(byte aPIN)
{




	_PIN = aPIN;
	_delay_fog = 10000;
	_delay_on_time = 5000;



	pinMode(_PIN, OUTPUT);
	digitalWrite(_PIN, 0);

}


////////////////////////////////////////

int  Fog::check(){



	unsigned long curtime = millis();

	switch (_fogstatus){
	case fs_off:
		if (FOGING) {
			FOGING = false;
			digitalWrite(_PIN, 0);

		}
		return 0;
		break;

	case fs_on:
		if (FOGING == true) {
			if (curtime > _last_fog + _delay_on_time) {
				_last_fog = curtime;
				FOGING = false;
				digitalWrite(_PIN, 0);
			}

		}
		else{
			if (curtime > _last_fog + _delay_fog) {
				_last_fog = curtime;
				FOGING = true;
				digitalWrite(_PIN, 1);
			}

		}

		break;

	case fs_now:
		if (FOGING == true)
		{

			if (curtime > (_last_fog + _delay_on_time)) {
				_last_fog = curtime;
				_fogstatus = fs_off;
				FOGING = false;
				digitalWrite(_PIN, 0);
			}
		}
		else {

			FOGING = true;
			digitalWrite(_PIN, 1);
			_last_fog = curtime;

		}

		break;

	}

}


int  Fog::Stop(){
	FOGING = false;
	_fogstatus = fs_off;
	digitalWrite(_PIN, 0);
}
void  Fog::FogStart(int adelay_fogsec, int  adelay_on_timesec){

	setfogsec(adelay_fogsec, adelay_on_timesec);

	_fogstatus = fs_on;
	_last_fog = millis();

}

void Fog::setfogsec(int adelay_fogsec, int  adelay_on_timesec){

	if (adelay_fogsec == 0)
		adelay_fogsec = int(_delay_fog / 1000);
	if (adelay_on_timesec == 0)
		adelay_on_timesec = int(_delay_on_time / 1000);

	_delay_fog = adelay_fogsec * 1000;
	_delay_on_time = adelay_on_timesec * 1000;


}
int Fog::getdelay_fogsec(){
	return int(_delay_fog / 1000);
}
int Fog::getdelay_on_timesec(){
	return int(_delay_on_time / 1000);
}

void Fog::FogNOW(){
	if (_fogstatus != fs_on)
		_fogstatus = fs_now;

	_last_fog = (millis() - _delay_fog);


}
boolean  Fog::isfogstatuson(){
	return  (_fogstatus == fs_on);
}
void  Fog::setfogstatuson(){
	_fogstatus = fs_on;
}









