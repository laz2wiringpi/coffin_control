
#include <SoftwareSerial.h>
#include "MotorServo.h"

#define MEMSHOWHELP
#define HAS_IR
#ifdef HAS_IR
#include "IRremote.h"
#endif


#ifdef DEBUG

#endif



#define NOVALUEINPUT -1

#define C_VERSION "V1.05"

long inputvalue = NOVALUEINPUT;




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



////////// error vars 

boolean errordetect = false;
String errorstring = "";
SoftwareSerial serial_otherboard(rxPin, txPin); // RX, TX

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

int testloop = 1;


enum pr_stats {
	pr_none,
	pr_open,
	pr_situp,
	pr_close,
	pr_sitdown
};

pr_stats programloopstat = pr_none;




////////////////////////////////////////////////////// setup
void setup()
{
	errorstring.reserve(40);

	// initialize serial:
	Serial.begin(115200);
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	// comport 
	serial_otherboard.begin(38400);




	 
	SitMotor.POT_MIN = 525;
	SitMotor.POT_MAX = 810;
	SitMotor.DOWN_SPEED = 140;
	 
	SitMotor.UP_SPEED = 140;
 
 

 
	DoorMotor.POT_MIN = 335;
	DoorMotor.POT_MAX = 890;
	DoorMotor.UP_SPEED = 140;
 
 
	DoorMotor.DOWN_SPEED = 140;
 
 
 

#ifdef HAS_IR
	irrecv.enableIRIn(); //start the receiver
#endif
	ShowHelp();
}

void doerror(String serror){
	errordetect = true;
	errorstring = serror;

}
/*
void   programloop(){

	if (DoorMotor.check() < 0) {
		doerror(F("DoorMotor FAIL"));
	}
	if (SitMotor.check() < 0) {
		doerror(F("SitMotor FAIL"));
	}

	if (errordetect) return;

	switch (programloopstat){
	case pr_open:


		if (DoorMotor.Current_pot() > ((DoorMotor.POT_MAX - DoorMotor.POT_DTG) - 5)) {
			programloopstat = pr_situp;
		}
		else {
	 		if ((DoorMotor.GetDIRECTION() == DIRECTION_OFF) && (DoorMotor.Current_pot() < (DoorMotor.POT_MAX - DoorMotor.POT_DTG))) {
 			docmd('o');

			}
		}
		break;
	case pr_situp:
		if (DoorMotor.Current_pot() < ((DoorMotor.POT_MAX - DoorMotor.POT_DTG) - 5)) {
			programloopstat = pr_open;
			return;
		}
		if (SitMotor.Current_pot() > ((SitMotor.POT_MAX - DoorMotor.POT_DTG) - 5)) {
			programloopstat = pr_none;
		}
		else {
			if ((SitMotor.GetDIRECTION() == DIRECTION_OFF) && (SitMotor.Current_pot() < (SitMotor.POT_MAX - SitMotor.POT_DTG))) {
				docmd('U');

			}
		}
		break;

	case pr_close:

		if (SitMotor.Current_pot() > ((SitMotor.POT_MIN + SitMotor.POT_DTG) + 5)) {
			programloopstat = pr_sitdown;
			return;
		}

		if (DoorMotor.Current_pot() < ((DoorMotor.POT_MIN + DoorMotor.POT_DTG) + 5)) {
			programloopstat = pr_none;
		}
		else {
			if ((DoorMotor.GetDIRECTION() == DIRECTION_OFF) && (DoorMotor.Current_pot() > (DoorMotor.POT_MIN + DoorMotor.POT_DTG  ))) {
				docmd('c');

			}
		}
		break;
	case pr_sitdown:

		if (SitMotor.Current_pot() < ((SitMotor.POT_MIN + SitMotor.POT_DTG) + 5)) {
			programloopstat = pr_close;
		}
		else {
			if ((SitMotor.GetDIRECTION() == DIRECTION_OFF) && (SitMotor.Current_pot() > (SitMotor.POT_MAX - SitMotor.POT_DTG ))) {
				docmd('D');

			}
		}
		break;



	}
}
*/
////////////////////////////////////////////////////// loop
void loop()
{


	serialEvent(); //call the function
	serialEventotherboard(); //call the function
#ifdef HAS_IR
	doIRdata();
#endif


	if (errordetect){
		Serial.print(errorstring);
		delay(1000);
	}
	else{

		if (testloop){



			if (DoorMotor.check() < 0) {
				doerror(F("DoorMotor FAIL"));
			}

			if (SitMotor.check() < 0) {
				doerror(F("SitMotor FAIL"));
			}

		}
		else {

		//	programloop();
		}


		//  Serial.print("---- :  "  );
		// int d = SitMotor.GetDIRECTION();
		//  Serial.print(d);
	}
}


////////////////////////////////////////////////////// ShowHelp
void 	ShowHelp()  {
#ifdef MEMSHOWHELP



	Serial.print("Coffin  ");
	Serial.println(C_VERSION);

	/*

	Serial.println(F("E Stop "));
	Serial.println("");
	Serial.println(F("1 - Sit Motor"));
	Serial.println(F("2 - coffin Door mode"));
	Serial.println("");
	Serial.println(F("(1|2)R    - Run "));
	Serial.println("");
	Serial.println(F("<num>U    - SIT up POT_MAX "));
	Serial.println(F("<num>D    - SIT up POT_Min "));
	Serial.println("");
	Serial.println(F("<num>o    - DOOR Open POT_MAX "));
	Serial.println(F("<num>c    - DOOR Close POT_Min "));
	Serial.println("");
	Serial.println(F("<num>T(t) - target_vol "));
	Serial.println("");
	Serial.println(F("<num>V(v) - Vol"));


	Serial.println(F("<num>S(s) - Start power "));


	Serial.println(F("<num>A(a) - Pot MAX "));


	Serial.println(F("<num>I(i) - Pot MIN "));
	Serial.println(F("<num>B(b) - Break Max "));

	//	Serial.println("--------------------------------------");

	Serial.print(F("      SitMotor.Current_pot :"));
	Serial.println(SitMotor.Current_pot());

	Serial.print(F("      SitMotor.target_vol  :"));
	Serial.println(SitMotor.target_vol);

	Serial.print(F("      SitMotor.VOL_FACTOR  :"));
	Serial.println(SitMotor.VOL_FACTOR);

	Serial.print(F("      SitMotor.MOTOR_START :"));
	Serial.println(SitMotor.MOTOR_START);

	Serial.print(F("      SitMotor.POT_MAX     :"));
	Serial.println(SitMotor.POT_MAX);

	Serial.print(F("      SitMotor.POT_MIN     :"));
	Serial.println(SitMotor.POT_MIN);

	Serial.print(F("    SitMotor.MAX_BREAK    :"));
	Serial.println(SitMotor.MAX_BREAK);


	Serial.print(F("      DoorMotor.Current_pot:"));
	Serial.println(DoorMotor.Current_pot());

	Serial.print(F("      DoorMotor.target_vol :"));
	Serial.println(DoorMotor.target_vol);


	Serial.print(F("      DoorMotor.VOL_FACTOR :"));
	Serial.println(DoorMotor.VOL_FACTOR);

	Serial.print(F("      DoorMotor.MOTOR_START:"));
	Serial.println(DoorMotor.MOTOR_START);

	Serial.print(F("      DoorMotor.POT_MAX    :"));
	Serial.println(DoorMotor.POT_MAX);

	Serial.print(F("      DoorMotor.POT_MIN    :"));
	Serial.println(DoorMotor.POT_MIN);

	Serial.print(F("    DoorMotor.MAX_BREAK    :"));
	Serial.println(DoorMotor.MAX_BREAK);


*/

	//	Serial.println("--------------------------------------");


#endif

}


////////////////////////////////////////////////////// serialEventotherboard
void serialEventotherboard() {

	while (Serial.available()) {

		// get the new byte:
		char inChar = (char)Serial.read();


#ifdef DEBUG
		Serial.print("NEW otherboard :  ");
		Serial.println(inChar);
#endif

		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

	}

}
////////////////////////////////////////////////////// doIRdata
void doIRdata() {
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

		case 0X32C6FDF7:
			testloop = false;
			programloopstat = pr_situp;

			Serial.println("*");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0X3EC3FC1B:
			testloop = false;
			programloopstat = pr_sitdown;
			Serial.println("#");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0X511DBB:
			testloop = true;
			docmd('U');

			Serial.println("UP");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0XA3C8EDDB:
			testloop = true;
			docmd('D');
			Serial.println("DOWN");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0X52A3D41F:
			testloop = true;
			docmd('c');
			Serial.println("LEFT");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0X20FE4DBB:
			testloop = true;
			docmd('o');
			Serial.println("RIGHT");
			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;
			break;

		case 0XD7E84B1B:
			testloop = true;
			docmd('E');

			Serial.println("OK");

			Serial.println(IRvalue);
			IRvalue = NOVALUEINPUT;

			break;

		default:
			// if nothing else matches, do the default
			Serial.println(results.value, HEX); //display HEX
			break;
		}
		if (IRDIGIT != NOVALUEINPUT) {
			if (IRvalue == NOVALUEINPUT) {
				IRvalue = 0;
			}
			IRvalue = (10 * IRvalue) + IRDIGIT;
			Serial.println(IRvalue);
		}
		irrecv.resume(); //next value
	}

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
void docmd(char inChar){

	switch (inChar){

	case 48 ... 57:
		if (inputvalue == NOVALUEINPUT){
			inputvalue = 0;
		}
		inputvalue = 10 * inputvalue + inChar - '0';


		break;
	case 'H':
		ShowHelp();
		serial_otherboard.println('H');

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
		//Serial.println(DoorMotor.Current_pot() );  
		//  Serial.println( ( (DoorMotor.POT_MAX - DoorMotor.POT_DTG) - 5 ));





		if (inputvalue == NOVALUEINPUT) {
			inputvalue = SitMotor.POT_MAX;
		}
#ifdef DEBUG
		Serial.print(inputvalue);
#endif
		 
		SitMotor.Goto(inputvalue);
	 
		 
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
		 
		DoorMotor.Goto(inputvalue);
	 
		 
 
		inputvalue = NOVALUEINPUT;

		break;



		//  Serial.println("<num>Y - Door target_vol ");

	 

		//  Serial.println("<num>T - Sit target_vol ");
 


		//  Serial.println("<num>V - Volosity factor SIT ");
 


	case 'S':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.UP_SPEED  = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 's':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.UP_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'T':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.DOWN_SPEED = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 't':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.DOWN_SPEED  = inputvalue;
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
		/*
	
	case 'B':


		if (inputvalue != NOVALUEINPUT) {

			SitMotor.MAX_BREAK = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	case 'b':


		if (inputvalue != NOVALUEINPUT) {

			DoorMotor.MAX_BREAK = inputvalue;
		}

		inputvalue = NOVALUEINPUT;

		break;

	*/


		//  Serial.println("<num>V - Volosity factor SIT ");



		// off to other board serial1 .. 

	default:

		//  case 'J':
		if (inputvalue != NOVALUEINPUT) {
			serial_otherboard.print(inputvalue);
		}
		serial_otherboard.println(inChar);
		inputvalue = NOVALUEINPUT;


		return;
		break;




	}



}
