

#include "common_coffin.h"

#define HAS_IR
#define HAS_EEPROM
#define HAS_SoftSer
#define MEMSHOWHELP

#ifdef HAS_SoftSer
#include <SoftwareSerial.h>
#endif



#ifdef HAS_EEPROM

#include <EEPROM.h> 
#endif
#include "MotorServo.h"

#include "Fog.h"

#include "LEDFader.h" 
#ifdef HAS_IR
#include "IRremote.h"
#endif
 


#define     eadd_SitMotor_POT_MIN       1
//                                  lsb 2
#define     eadd_SitMotor_POT_MAX       3
//                                  lsb 4
#define     eadd_SitMotor_DOWN_SPEED    5
#define     eadd_SitMotor_UP_SPEED      6
#define     eadd_DoorMotor_POT_MIN      7
//                                  lsb 8
#define     eadd_DoorMotor_POT_MAX      9
//                                  lsb 10
#define     eadd_DoorMotor_UP_SPEED     11
#define     eadd_DoorMotor_DOWN_SPEED   12
#define     eadd_delay_fog              13
//                                  lsb 14
#define     eadd_fog_delay_on_time      15
//                                  lsb 16
#define     eadd_fog_on                 17

// next is 13    


#define NOVALUEINPUT -1

#define C_VERSION "V1.05"
#define NOVALUEINPUT -1
long inputvalue = NOVALUEINPUT;
long other_inputvalue = NOVALUEINPUT;
pr_stats otherbooard_pr_stats = pr_none;




///////////////////////////////////////////////// PINS
MotorServo SitMotor(A0,
	5,
	7,
	8);

MotorServo DoorMotor(A1,
	6,
	2,
	4);

#define rxPin 19
#define txPin 18
#define FOG_PIN 20
#define LED_PIN 13

//////////////////////

Fog FogMachine(FOG_PIN);

#define FADE_TIME 2000

#define LED_DIR_UP 1
#define LED_DIR_DOWN -1

LEDFader LedMain;
int LedMaindirection = LED_DIR_UP;


////////////////////////////////////////////////


////////// error vars 

boolean errordetect = false;
String errorstring = "";
#ifdef HAS_SoftSer
SoftwareSerial serial_otherboard(rxPin, txPin); // RX, TX


#endif

////////// ir global vars 
#ifdef HAS_IR 



#ifndef NOVALUEINPUT
#define NOVALUEINPUT -1
#endif
int receiver = 11;
int IRvalue = -1;

 IRrecv irrecv(receiver); //create a new instance of receiver
decode_results results;
#endif

/////////// program loop vars 


/*

enum pr_stats {
	pr_none,
	pr_open,
	pr_situp,
	pr_close,
	pr_sitdown,

};
*/
pr_stats programloopstat = pr_none;
////////////////////////////////////////////////////////////////////


void ledloop() {

	if (LedMain.enabled)
	{

	
	LedMain.update();
	 

	// LED no longer fading, switch direction
	if (!LedMain.is_fading()) {

		// Fade down
		if (LedMaindirection == LED_DIR_UP) {
			LedMain.fade(0, FADE_TIME);
			LedMaindirection = LED_DIR_DOWN;
		}
		// Fade up
		else {
			LedMain.fade(255, FADE_TIME);
			LedMaindirection = LED_DIR_UP;
		}
	}
	}
}

////////////////////////////////////////////////////////////////////
void write_to_eprom() {

#ifdef HAS_EEPROM
	byte savedummyvar = 1;


	EEPROM.put(0, savedummyvar);
	EEPROM.put(eadd_SitMotor_POT_MIN, SitMotor.POT_MIN);
	EEPROM.put(eadd_SitMotor_POT_MAX, SitMotor.POT_MAX);
	EEPROM.put(eadd_SitMotor_DOWN_SPEED, SitMotor.DOWN_SPEED);
	EEPROM.put(eadd_SitMotor_UP_SPEED, SitMotor.UP_SPEED);



	EEPROM.put(eadd_DoorMotor_POT_MIN, DoorMotor.POT_MIN);
	EEPROM.put(eadd_DoorMotor_POT_MAX, DoorMotor.POT_MAX);
	EEPROM.put(eadd_DoorMotor_DOWN_SPEED, DoorMotor.DOWN_SPEED);
	EEPROM.put(eadd_DoorMotor_UP_SPEED, DoorMotor.UP_SPEED);

	EEPROM.put(eadd_delay_fog, FogMachine.getdelay_fogsec());
	EEPROM.put(eadd_fog_delay_on_time, FogMachine.getdelay_on_timesec());
	EEPROM.put(eadd_fog_on, int(FogMachine.isfogstatuson()));

#endif
}

////////////////////////////////////////////////////// setup
void setup()
{



	LedMain = LEDFader(LED_PIN);
	LedMain.fade(255, FADE_TIME);


	errorstring.reserve(40);

	// initialize serial:
	Serial.begin(115200);
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	// comport 
#ifdef HAS_SoftSer
	serial_otherboard.begin(38400);
#endif

	/*
	  SitMotor.Current_pot :515

	  SitMotor.UP_SPEED  :110

	  SitMotor.DOWN_SPEED  :80

	  SitMotor.POT_MAX     :810

	  SitMotor.POT_MIN     :525

	  DoorMotor.Current_pot:101

	  DoorMotor.UPSPEED :190

	  DoorMotor.DOWNSPEED :55

	  DoorMotor.POT_MAX    :690

	  DoorMotor.POT_MIN    :120

	  */
#ifdef HAS_EEPROM
	if (EEPROM.read(0) == 0) {
#endif


		SitMotor.POT_MIN = 525;
		SitMotor.POT_MAX = 810;
		SitMotor.DOWN_SPEED = 70;

		SitMotor.UP_SPEED = 110;




		DoorMotor.POT_MIN = 120;
		DoorMotor.POT_MAX = 690;
		DoorMotor.UP_SPEED = 190;


		DoorMotor.DOWN_SPEED = 55;
#ifdef HAS_EEPROM
	}
	else {

		int tmpint = 0;
		int tmpint2 = 0;
		EEPROM.get(eadd_SitMotor_POT_MIN, SitMotor.POT_MIN);
		EEPROM.get(eadd_SitMotor_POT_MAX, SitMotor.POT_MAX);
		EEPROM.get(eadd_SitMotor_UP_SPEED, SitMotor.UP_SPEED);
		EEPROM.get(eadd_SitMotor_DOWN_SPEED, SitMotor.DOWN_SPEED);

		EEPROM.get(eadd_DoorMotor_POT_MIN, DoorMotor.POT_MIN);
		EEPROM.get(eadd_DoorMotor_POT_MAX, DoorMotor.POT_MAX);
		EEPROM.get(eadd_DoorMotor_UP_SPEED, DoorMotor.UP_SPEED);
		EEPROM.get(eadd_DoorMotor_DOWN_SPEED, DoorMotor.DOWN_SPEED);

		EEPROM.get(eadd_delay_fog, tmpint);
		EEPROM.get(eadd_fog_delay_on_time, tmpint2);

		FogMachine.setfogsec(tmpint, tmpint2);



		EEPROM.get(eadd_fog_on, tmpint);
		if (tmpint != 0)
			FogMachine.setfogstatuson();




	}
#endif




#ifdef HAS_IR
	irrecv.enableIRIn(); //start the receiver
#endif
	ShowHelp();
}

void doerror(String serror) {
	errordetect = true;
	errorstring = serror;

}

void   programloop() {

	FogMachine.check();
	ledloop();

	if (DoorMotor.check() < 0) {
		doerror(F("DoorMotor FAIL"));
	}
	if (SitMotor.check() < 0) {
		doerror(F("SitMotor FAIL"));
	}

	int DoorMotor_Current_pot = DoorMotor.Current_pot();
	int SitMotor_Current_pot = SitMotor.Current_pot();

/// broken pot ...

	if ((DoorMotor_Current_pot < 2 )||(DoorMotor_Current_pot > 1023))
		doerror(F("DoorMotor_Current_pot pot FAIL"));

	if ((SitMotor_Current_pot < 2) || (SitMotor_Current_pot > 1023))
		doerror(F("SitMotor_Current_pot pot FAIL"));
/////


	if (errordetect) return;


	 
	switch (programloopstat) {

	case pr_none:
		// ADD TIMER AND CHECK FOR FIRE HERE 
		break;

	case pr_open:



		if (DoorMotor_Current_pot > (DoorMotor.POT_MAX - 5)) {
			programloopstat = pr_situp;
		}
		else {
			if ((DoorMotor.GetDIRECTION() == DIRECTION_OFF) && (DoorMotor_Current_pot < (DoorMotor.POT_MAX - 5))) {
				FogMachine.FogStart(0, 0);
				LedMain.enabled = true;

				docmd('o');

			}
		}
		break;
	case pr_situp:



		if (DoorMotor_Current_pot < (DoorMotor.POT_MAX - 5)) {
			programloopstat = pr_open;
			return;
		}
		if (SitMotor_Current_pot > (SitMotor.POT_MAX - 5)) {
			programloopstat = stat_openandsitdone;
        docmd('P');

		}
		else {
			if ((SitMotor.GetDIRECTION() == DIRECTION_OFF) && (SitMotor_Current_pot < (SitMotor.POT_MAX - -5))) {
				docmd('U');
			 

			}
		}
		break;




	case pr_close:

		if (SitMotor_Current_pot > (SitMotor.POT_MIN + 30)) {
			programloopstat = pr_sitdown;
			return;
		}

		if (DoorMotor_Current_pot < (DoorMotor.POT_MIN + 5)) {
			programloopstat = stat_closeandsitdone;
        docmd('P');
		}
		else {
			if ((DoorMotor.GetDIRECTION() == DIRECTION_OFF) && (DoorMotor_Current_pot >(DoorMotor.POT_MIN - 5))) {
				FogMachine.Stop() ;
				LedMain.enabled = false;
				docmd('c');

			}
		}
		break;
	case pr_sitdown:

		if (SitMotor_Current_pot < (SitMotor.POT_MIN + 5)) {
			programloopstat = pr_close;
		}
		else {
			if ((SitMotor.GetDIRECTION() == DIRECTION_OFF) && (SitMotor_Current_pot >(SitMotor.POT_MAX - 5))) {
				docmd('D');

			}
		}
		break;




	}
}

////////////////////////////////////////////////////// loop
void loop()
{


	serialEvent(); //call the function
	serialEventotherboard(); //call the function
#ifdef HAS_IR
	doIRdata();
#endif


	if (errordetect) {
		Serial.print(errorstring);
		delay(1000);
	}
	else {



		programloop();


	}
}


////////////////////////////////////////////////////// ShowHelp
void 	ShowHelp() {
#ifdef MEMSHOWHELP



	Serial.print(F("Coffin  "));
	Serial.println(C_VERSION);



	Serial.println(F("     E Stop "));
	Serial.println("");
	Serial.println(F("<num>U    - SIT up POT_MAX "));
	Serial.println(F("<num>D    - SIT up POT_Min "));
	Serial.println("");
	Serial.println(F("<num>o    - DOOR Open POT_MAX "));
	Serial.println(F("<num>c    - DOOR Close POT_Min "));
	Serial.println("");

	Serial.println(F("<num>S(s) - UP_SPEED "));
	Serial.println(F("<num>W(w) - DOWN_SPEED"));
	Serial.println("");
	Serial.println(F("<num>A(a) - Pot MAX "));
	Serial.println(F("<num>I(i) - Pot MIN "));

	Serial.println(F("     n      FOG NOW "));
	Serial.println(F("     m      FOG start"));
	Serial.println(F("     b      FOG off  "));
	Serial.println(F("<num>.      FOG adelay_fogsec "));
	Serial.println(F("<num>,      FOG adelay_on_timesec   "));

	Serial.println(F("     Z      WRITE EPROM    :"));

	Serial.println(F("--------------------------------------"));

	Serial.print(F("      SitMotor.Current_pot :"));
	Serial.println(SitMotor.Current_pot());

	Serial.print(F("      SitMotor.UP_SPEED  :"));
	Serial.println(SitMotor.UP_SPEED);

	Serial.print(F("      SitMotor.DOWN_SPEED  :"));
	Serial.println(SitMotor.DOWN_SPEED);


	Serial.print(F("      SitMotor.POT_MAX     :"));
	Serial.println(SitMotor.POT_MAX);

	Serial.print(F("      SitMotor.POT_MIN     :"));
	Serial.println(SitMotor.POT_MIN);

	Serial.print(F("      SitMotor.direction     :"));
	Serial.println(SitMotor.GetDIRECTION());


	Serial.print(F("      DoorMotor.Current_pot:"));
	Serial.println(DoorMotor.Current_pot());

	Serial.print(F("      DoorMotor.UPSPEED :"));
	Serial.println(DoorMotor.UP_SPEED);


	Serial.print(F("      DoorMotor.DOWNSPEED :"));
	Serial.println(DoorMotor.DOWN_SPEED);



	Serial.print(F("      DoorMotor.POT_MAX    :"));
	Serial.println(DoorMotor.POT_MAX);

	Serial.print(F("      DoorMotor.POT_MIN    :"));
	Serial.println(DoorMotor.POT_MIN);

	Serial.print(F("      DoorMotor.direction     :"));
	Serial.println(SitMotor.GetDIRECTION());


	Serial.print(F("      FOG      :"));
	Serial.println(int(FogMachine.isfogstatuson()));


	Serial.print(F("      FOG getdelay_fogsec     :"));
	Serial.println(int(FogMachine.getdelay_fogsec()));

	Serial.print(F("      FOG   getdelay_on_timesec   :"));
	Serial.println(int(FogMachine.getdelay_on_timesec()));




	//	Serial.println("--------------------------------------");


#endif

}


////////////////////////////////////////////////////// serialEventotherboard
void serialEventotherboard() {

	while (serial_otherboard.available()) {

		// get the new byte:
		char inChar = (char)serial_otherboard.read();


		switch (inChar) {
    
		case 48 ... 57:

			if (other_inputvalue == NOVALUEINPUT) {
				other_inputvalue = 0;
			}
			other_inputvalue = 10 * other_inputvalue + inChar - '0';


			break;

		case'p':


			switch (other_inputvalue)
			{
			case pr_none:
				otherbooard_pr_stats = pr_none;
				break;
			case pr_before_open_loop:
				otherbooard_pr_stats = pr_before_open_loop;
			 
				break;
			case pr_open:
				otherbooard_pr_stats = pr_open;
			//	programloopstat = pr_open;
				break;
			case pr_wake_up:
				otherbooard_pr_stats = pr_wake_up;
		 
				break;
			case pr_situp:
				otherbooard_pr_stats = pr_situp;
			//	programloopstat = pr_situp;
				break;
			case pr_talk:
				otherbooard_pr_stats = pr_talk;
			//	programloopstat = pr_talk;
				break;
			case pr_sitdown:
				otherbooard_pr_stats = pr_sitdown;
				break;
			case pr_sleep:
				otherbooard_pr_stats = pr_sleep;
			//	programloopstat = pr_sleep;
				break;
			case pr_close:
				otherbooard_pr_stats = pr_close;
            programloopstat = pr_sitdown;
				break;
			case pr_after_close:
				otherbooard_pr_stats = pr_after_close;
			//	programloopstat = pr_after_close;
				break;
			case stat_openandsitdone:
				otherbooard_pr_stats = stat_openandsitdone;
			//	programloopstat = stat_openandsitdone;
				break;
			case stat_closeandsitdone:
				otherbooard_pr_stats = stat_closeandsitdone;
			//	programloopstat = stat_closeandsitdone;

				break;
			default:
				/// unknown 
				break;
        Serial.print('other board p');
Serial.println(other_inputvalue);


			}


			other_inputvalue = NOVALUEINPUT;
			break;
		}
	

		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

	}

}
////////////////////////////////////////////////////// doIRdata
void doIRdata() {
#ifdef HAS_IR


	if (irrecv.decode(&results)) { //we have received an IR
		int IRDIGIT = NOVALUEINPUT;

		switch (results.value) {
		case 0XC101E57B:

			IRDIGIT = 1;
			break;

		case 0X97483BFB:

			IRDIGIT = 2;
			break;

		case 0XF0C41643:
			IRDIGIT = 3;
			break;

		case 0X9716BE3F:
			IRDIGIT = 4;
			break;

		case 0X3D9AE3F7:
			IRDIGIT = 5;
			break;

		case 0X6182021B:
			IRDIGIT = 6;
			break;

		case 0X8C22657B:
			IRDIGIT = 7;
			break;


		case 0X488F3CBB:
			IRDIGIT = 8;
			break;


		case 0X449E79F:
			IRDIGIT = 9;
			break;


		case 0X1BC0157B:
			IRDIGIT = 0;
			break;

		case 0X32C6FDF7: //////////////////////////////////////////////////////// *
	 
			programloopstat = pr_situp;
#ifdef DEBUG
			Serial.println("*");
			Serial.println(IRvalue);
#endif

			IRvalue = NOVALUEINPUT;
			break;

		case 0X3EC3FC1B: //////////////////////////////////////////////////////// #
	 
			programloopstat = pr_sitdown;
#ifdef DEBUG
			Serial.println("#");
			Serial.println(IRvalue);
#endif

			IRvalue = NOVALUEINPUT;
			break;

		case 0X511DBB:  ////////////////////////////////////////////////////////UP
	 
			docmd('U');
#ifdef DEBUG
			Serial.println("UP");
			Serial.println(IRvalue);
#endif
			IRvalue = NOVALUEINPUT;
			break;

		case 0XA3C8EDDB: ////////////////////////////////////////////////////////DOWN
		 
			docmd('D');
#ifdef DEBUG 
			Serial.println("DOWN");
			Serial.println(IRvalue);
#endif
		 

			IRvalue = NOVALUEINPUT;
			break;

		case 0X52A3D41F: ////////////////////////////////////////////////////////LEFT
	 
			docmd('c');
#ifdef DEBUG
			Serial.println("LEFT");
			Serial.println(IRvalue);
#endif
			IRvalue = NOVALUEINPUT;
			break;

		case 0X20FE4DBB: ////////////////////////////////////////////////////////RIGHT
	 
			docmd('o');
#ifdef DEBUG
			Serial.println("RIGHT");
			Serial.println(IRvalue);
#endif
			IRvalue = NOVALUEINPUT;
			break;

		case 0XD7E84B1B: //////////////////////////////////////////////////////// OK


			switch (IRvalue) {
			case NOVALUEINPUT:
			 
				docmd('E');
				break;

			case 1:
				 
				docmd('n');
				break;

			case 2:
			 
				docmd('m');
				break;

			case 3:
			 
				docmd('b');
				break;


			case 4:
			 
				docmd('f');
				break;

			case 5:
			 
				docmd('g');
				break;
			}



#ifdef DEBUG

			Serial.println("OK");

			Serial.println(IRvalue);
#endif
			IRvalue = NOVALUEINPUT;

			break;

		default:
			// if nothing else matches, do the default
#ifdef DEBUG
			Serial.println(results.value, HEX); //display HEX
#endif
			break;
		}
		if (IRDIGIT != NOVALUEINPUT) {
			if (IRvalue == NOVALUEINPUT) {
				IRvalue = 0;
			}
			IRvalue = (10 * IRvalue) + IRDIGIT;
#ifdef DEBUG
			Serial.println(IRvalue);
#endif
		}
		irrecv.resume(); //next valuea
	}
#endif
}

////////////////////////////////////////////////////// serialEvent
void serialEvent() {

	while (Serial.available()) {

		// get the new byte:
		char inChar = (char)Serial.read();

		//		inputString += inChar;
#ifdef DEBUG
		//    Serial.print("NEW inputString :  ");
		//    Serial.println(inputString);
#endif
		docmd(inChar);
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
	}
}
void docmd(char inChar) {

	switch (inChar) {

	case 48 ... 57:
		if (inputvalue == NOVALUEINPUT) {
			inputvalue = 0;
		}
		inputvalue = 10 * inputvalue + inChar - '0';


		break;
	case 'H':
		ShowHelp();
 

		inputvalue = NOVALUEINPUT;
		break;

	case 'Z':
		write_to_eprom();
		inputvalue = NOVALUEINPUT;
		break;

	case 'E':
		errordetect = false;
		SitMotor.StopMotor();
		DoorMotor.StopMotor();


		inputvalue = NOVALUEINPUT;
		break;
	case 'e':
		errordetect = false;
		SitMotor.StopMotor();
		DoorMotor.StopMotor();


		inputvalue = NOVALUEINPUT;
		break;




	case 'U':
		/// check if we will bang head 


		if (inputvalue == NOVALUEINPUT) {
			inputvalue = SitMotor.POT_MAX;
		}

		if (DoorMotor.Current_pot() > (DoorMotor.POT_MIN) - 5) {
#ifdef DEBUG
			Serial.print(inputvalue);
#endif

			SitMotor.Goto(inputvalue);
		}
		else {
			// will bag head 
#ifdef DEBUG
			Serial.print(F("sit will bang head"));
#endif
		}

		inputvalue = NOVALUEINPUT;


		//  Serial.println("<num>D - up POT_Min ");

		break;

	case 'D':


		if (inputvalue == NOVALUEINPUT) {
			inputvalue = SitMotor.POT_MIN;
		}

		SitMotor.Goto(inputvalue);




		inputvalue = NOVALUEINPUT;


		break;
		//  Serial.println("<num>O - Open POT_MAX ");

	case  'o':


		if (inputvalue == NOVALUEINPUT) {
			inputvalue = DoorMotor.POT_MAX;
		}
		//  DoorMotor.POT_MIN = inputvalue;
		DoorMotor.Goto(inputvalue);

		inputvalue = NOVALUEINPUT;







		break;

		//  Serial.println("<num>C - Close POT_Min ");
	case  'c':

		if (inputvalue == NOVALUEINPUT) {
			inputvalue = DoorMotor.POT_MIN;
		}
		if (SitMotor.Current_pot() > (SitMotor.POT_MIN + 20)) {

#ifdef DEBUG
			Serial.print(F("sit will bang head"));
#endif
		}
		else {

			DoorMotor.Goto(inputvalue);
		}



		inputvalue = NOVALUEINPUT;

		break;



		//  Serial.println("<num>Y - Door target_vol ");



		//  Serial.println("<num>T - Sit target_vol ");



		//  Serial.println("<num>V - Volosity factor SIT ");



	case 'S':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.UP_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 's':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.UP_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'W':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.DOWN_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'w':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.DOWN_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;



	case 'A':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.POT_MAX = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'a':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.POT_MAX = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'I':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.POT_MIN = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'i':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.POT_MIN = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;


		//////////////////////////////

	case 'n':


		FogMachine.FogNOW();

		inputvalue = NOVALUEINPUT;

		break;
	case 'b':

		FogMachine.Stop();

		inputvalue = NOVALUEINPUT;

		break;
	case 'm':


		//  FogMachine.FogStart(FogMachine.getdelay_fogsec() ,FogMachine.getdelay_on_timesec());
		FogMachine.FogStart(0, 0);

		inputvalue = NOVALUEINPUT;

		break;

	case ',':


		FogMachine.setfogsec(0, inputvalue);

		inputvalue = NOVALUEINPUT;

		break;
	case '.':


		FogMachine.setfogsec(inputvalue, 0);

		inputvalue = NOVALUEINPUT;

		break;


	case 'f':


		LedMain.set_value(255);

		inputvalue = NOVALUEINPUT;

		break;

	case 'g':


		LedMain.set_value(0);

		inputvalue = NOVALUEINPUT;

		break;

		// off to other board serial1 .. 

	case 'P':
#ifdef HAS_SoftSer
		serial_otherboard.print(programloopstat);
		serial_otherboard.println('P');
#endif      

		break;

 

	default:
/*

#ifdef HAS_SoftSer
		//  case 'J':
		if (inputvalue != NOVALUEINPUT) {
			serial_otherboard.print(inputvalue);
		}

		serial_otherboard.println(inChar);
#endif   
*/
		inputvalue = NOVALUEINPUT;


		return;
		break;




	}



}
