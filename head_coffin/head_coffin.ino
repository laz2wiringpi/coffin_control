#include <VarSpeedServo.h> 
//////////////////////////////////////////////////////////
#include <common_coffin.h> 


#define HAS_SoftSer
 

#ifdef HAS_SoftSer
#include <SoftwareSerial.h>

#define rxPin 19
#define txPin 18

#endif
 

pr_stats programloopstat = pr_none;
 
long other_inputvalue = NOVALUEINPUT;



loop_stats loopstatus = LOOP_START;

pr_stats otherbooard_pr_stats = pr_none;

unsigned long start_loop_time = 0;

#ifdef HAS_SoftSer
SoftwareSerial serial_otherboard(rxPin, txPin); // RX, TX


#endif

////////////////////////////////////////////////////////
VarSpeedServo neckservoH;  // create servo object to control a servo 


#define totalneckseqcount 2
int neckValsH[totalneckseqcount] = { 0x2050, 0x206E  };


byte neckseqcount = 0;

void setup() {
#ifdef HAS_SoftSer
	serial_otherboard.begin(38400);
#endif

	neckservoH.attach(9);  // attaches the servo on pin 9 to the servo object
	neckservoH.write(90, 20, true); // set the intial position of the servo, as fast as possible, wait until done


}

/////////////////////////////////////////////////////////////////////////////////////// do_before_open_loop
byte do_before_open_loop() {



	return 	LOOP_FINISH;

}


/////////////////////////////////////////////////////////////////////////////////////// do_open_loop
byte do_open_loop() {
#ifdef HAS_SoftSer
	switch (loopstatus)
	{
	case LOOP_START:

		serial_otherboard.println('o');



		loopstatus = LOOP_RUN;

		break;
	case LOOP_RUN:

		if (otherbooard_pr_stats == pr_none) {
			loopstatus = LOOP_FINISH;
		}
		else {
			serial_otherboard.println('p');
		}

		break;
	case LOOP_FINISH:
		break;

	}

	return 	loopstatus;
#endif   
#ifndef HAS_SoftSer
	return 	LOOP_FINISH;
#endif   

}
/////////////////////////////////////////////////////////////////////////////////////// do_wakeup_loop
byte do_wakeup_loop() {

	return 	LOOP_FINISH;

}

/////////////////////////////////////////////////////////////////////////////////////// do_situp_loop
byte do_situp_loop() {

	return 	LOOP_FINISH;

}



/////////////////////////////////////////////////////////////////////////////////////// do_talk_loop
#define LOOK_RIGHT   120 
#define LOOK_RIGHT_SPEED   20 
byte do_talk_loop() {
	int curposH = -1;

	switch (loopstatus)
	{
	case LOOP_START:
		// look to the right 

		neckservoH.write(LOOK_RIGHT, LOOK_RIGHT_SPEED, false);
		loopstatus = LOOP_RUN;

		break;
	case LOOP_RUN:
		// play the sound 
		curposH = neckservoH.read();
		if (curposH == LOOK_RIGHT) {
			if (start_loop_time > start_loop_time + 5000)
				loopstatus = LOOP_FINISH;
		}


		break;
	case LOOP_FINISH:
		break;

	}

	return 	loopstatus;

}

/////////////////////////////////////////////////////////////////////////////////////// do_sitdown_loop
byte do_sitdown_loop() {

	return 	LOOP_FINISH;

}


/////////////////////////////////////////////////////////////////////////////////////// do_sleep_loop
#define LOOK_UP   90 
#define LOOK_UP_SPEED   40 
byte do_sleep_loop() {

	int curposH = -1;

	switch (loopstatus)
	{
	case LOOP_START:
		// look to the right 

		neckservoH.write(LOOK_UP, LOOK_UP_SPEED, false);
		loopstatus = LOOP_RUN;

		break;
	case LOOP_RUN:
		// play the sound 
		curposH = neckservoH.read();
		if (curposH == LOOK_UP) {

			loopstatus = LOOP_FINISH;
		}


		break;
	case LOOP_FINISH:
		break;

	}

	return 	loopstatus;

}

/////////////////////////////////////////////////////////////////////////////////////// do_close_loop
byte do_close_loop() {
#ifdef HAS_SoftSer
	switch (loopstatus)
	{
	case LOOP_START:

		serial_otherboard.println('c');



		loopstatus = LOOP_RUN;

		break;
	case LOOP_RUN:

		if (otherbooard_pr_stats == pr_none ) {
			loopstatus = LOOP_FINISH;
			 
		}
		else {
			serial_otherboard.println('p');
		}

		break;
	case LOOP_FINISH:
		break;

	}

	return 	loopstatus;
#endif   
#ifndef HAS_SoftSer
	return 	LOOP_FINISH;
#endif   

}

/////////////////////////////////////////////////////////////////////////////////////// do_after_close_loop
byte do_after_close_loop() {

	return 	LOOP_FINISH;

}




void   programloop() {



	switch (programloopstat)
	{
	case pr_none:
		start_loop_time = millis();

		break;

	case pr_before_open_loop:
		switch (do_before_open_loop()) {

		case LOOP_START:

			break;
		case LOOP_RUN:
			break;
		case LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_open;
			return;

		}

	case pr_open:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_situp;
			return;

		}


		break;

	case pr_situp:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_wake_up;
			return;

		}

		break;


	case pr_wake_up:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_talk;
			return;

		}


		break;
	case pr_talk:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case  LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_sitdown;
			return;

		}

		break;

	case pr_sitdown:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case  LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_sleep;
			return;

		}

		break;

	case pr_sleep:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case  LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_close;
			return;

		}

		break;

	case pr_close:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_after_close;
			return;

		}

		break;
	case pr_after_close:
		switch (do_before_open_loop()) {

		case LOOP_START:
			break;
		case LOOP_RUN:
			break;
		case  LOOP_FINISH:
			loopstatus = LOOP_START;
			start_loop_time = millis();
			programloopstat = pr_none;
			return;

		}


		break;
	default:
		break;
	}

}

void serialEventotherboard() {
	// always has last status ...

	while (Serial.available()) {

		// get the new byte:
		char inChar = (char)Serial.read();

		switch (inChar) {

		case 48 ... 57:

			if (other_inputvalue == NOVALUEINPUT) {
				other_inputvalue = 0;
			}
			other_inputvalue = 10 * other_inputvalue + inChar - '0';


			break;
		
		case'P':
			switch (other_inputvalue)
			{
			case pr_none :
				otherbooard_pr_stats = pr_none;
				break;
			case pr_open:
				otherbooard_pr_stats = pr_open;
				break;
			case pr_close:
				otherbooard_pr_stats = pr_close;
				break;

			case pr_sitdown :
				otherbooard_pr_stats = pr_sitdown;
				break;
			case pr_situp :
				otherbooard_pr_stats = pr_situp;
				break;
		 
			}
			 
 
			other_inputvalue = NOVALUEINPUT;
			break;
		}


#ifdef DEBUG
		Serial.print("NEW otherboard :  ");
		Serial.println(inChar);
#endif

		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

	}

}

void loop()
{

	serialEventotherboard(); //call the function

	// do a testing loop 
	if (programloopstat = pr_none)
		programloopstat = pr_before_open_loop;

	programloop();



	// convert to deg and speed 
	/*

	byte neckposH = lowByte(neckValsH[neckseqcount]);
	int curposH = neckservoH.read();
	//curposH = neckposH;

	byte neckposV = lowByte(neckValsV[neckseqcount]);
	int curposV = neckservoV.read();


	if ((curposH == neckposH)
		&& (curposV == neckposV)
		) {
		// next seq
		neckseqcount++;
		if (neckseqcount > totalneckseqcount)
			neckseqcount = 0;

		byte neckspeedV = highByte(neckValsV[neckseqcount]);
		byte neckspeedH = highByte(neckValsH[neckseqcount]);
		neckspeedH = 15;
		neckspeedV = 15;

		neckposH = lowByte(neckValsH[neckseqcount]);
		neckposV = lowByte(neckValsV[neckseqcount]);

		neckservoH.write(neckposH, neckspeedH, false);
		neckservoV.write(neckposV, neckspeedV, false);


	}

	*/

	/* add main program code here */

}