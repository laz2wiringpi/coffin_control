/*
fog.h
Version .01

*/
#ifndef Fog_h
#define Fog_h

#include "Arduino.h"



enum fogstatus {
	fs_off,
	fs_on,
	fs_now
};



class Fog
{
public:

	Fog(byte aPIN);
	int check();
	int Stop();
	int getdelay_fogsec();
	int getdelay_on_timesec();
	void  setfogsec(int adelay_fogsec, int  adelay_on_timesec);

	void FogStart(int adelay_fogsec, int  adelay_on_timesec);
	void FogNOW();
	boolean  isfogstatuson();
	void  setfogstatuson();
 

private:

	byte _PIN;
	boolean FOGING;

	fogstatus _fogstatus;
	unsigned long _last_fog;
	unsigned long _delay_fog;
	unsigned long _delay_on_time;

};




#endif
