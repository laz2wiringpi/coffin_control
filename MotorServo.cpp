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

	readPotIndex = 0;              // the index of the current reading
	totalPot = 0;                  // the running total
	for (int thisReading = 0; thisReading < numPotReadings; thisReading++) {
		Pot_readings[thisReading] = 0;
	}


	stop_after_run = false;

	POT_MIN = 500;
	POT_MAX = 800;
	MOTOR_START = 100;
	POT_DTG = 10;
	//	MOTOR_MIN = 1;
	MOTOR_MAX = 250;
	target_vol = 50;
	VOL_FACTOR = 1;
        MAX_BREAK = 5;
        _cur_volavgcnt = 0;
        _cur_lastvolavgcnt = 0; 
	// _PIN_POT = DEFAULT_PIN_POT;
	// _PIN_MOTOR = DEFAULT_PIN_MOTOR;
	// _PIN_MOTOR_UP = DEFAULT_PIN_MOTOR_UP;
	// _PIN_MOTOR_DOWN = DEFAULT_PIN_MOTOR_DOWN;



	_last_potavg = 0;
	_last_speed = 0;
	_last_ms = 0;

	_DIRECTION = DIRECTION_OFF;
	_last_direction = DIRECTION_OFF;
	_target_pos = POT_MIN;


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
int MotorServo::Stop(){

	// wee need to seend a stop 
	//if (_last_speed > 0) {

	_last_speed = 0;
	_last_ms = 0;
	_DIRECTION = DIRECTION_OFF;
	_last_direction = _DIRECTION;
	analogWrite(_PIN_MOTOR, 0);
	digitalWrite(_PIN_MOTOR_UP, LOW);
	digitalWrite(_PIN_MOTOR_DOWN, LOW);
	//}

	_last_potavg = analogRead(_PIN_POT);
	return _last_potavg;
}

void  MotorServo::donot_burn_out_hbridge(int mpin, int dummyvalue){

	if (mpin == _PIN_MOTOR_UP) {
		digitalWrite(_PIN_MOTOR_DOWN, LOW);
		digitalWrite(_PIN_MOTOR_UP, HIGH);
	}
	if (mpin == _PIN_MOTOR_DOWN){
		digitalWrite(_PIN_MOTOR_UP, LOW);
		digitalWrite(_PIN_MOTOR_DOWN, HIGH);
	}

}


////////////////////////////////////////
int MotorServo::Run(int apot_avrage){


	_last_speed = 0;
	_last_ms = millis();
	_DIRECTION = DIRECTION_RUN;

	analogWrite(_PIN_MOTOR, 0);
	digitalWrite(_PIN_MOTOR_UP, LOW);
	digitalWrite(_PIN_MOTOR_DOWN, LOW);


	// _last_direction = DIRECTION_RUN; // for startup speed removed 
 if (apot_avrage != 0) { 
	 
	 _last_potavg = analogRead(_PIN_POT);
 }else{
   _last_potavg = apot_avrage;
 }
	return _last_potavg;
}
////////////////////////////////////////
int  MotorServo::check(){

	if (_DIRECTION == DIRECTION_OFF){
		return 0;
	}
	int newspeed = _last_speed;

	int pot_current = analogRead(_PIN_POT);


	///////////////
	 


	/////////////////
	// use avrage of readings for vol 

	// subtract the last reading:
	totalPot = totalPot - Pot_readings[readPotIndex];
	// read from the sensor:
	Pot_readings[readPotIndex] = pot_current;
	// add the reading to the total:
	totalPot = totalPot + Pot_readings[readPotIndex];
	// advance to the next position in the array:
	readPotIndex = readPotIndex + 1;

	// if we're at the end of the array...
	if (readPotIndex >= numPotReadings) {
		// ...wrap around to the beginning:
		readPotIndex = 0;
	}

	// calculate the average:
	int  pot_avrage = totalPot / numPotReadings;


	if (abs(_target_pos - pot_current) <= POT_DTG) {


		if (stop_after_run) {
			Stop() ;
      return 1;
		}
		else
			Run(pot_avrage);

		return 0;
	}

	// calculate the speed do we go up or down 
	unsigned long current_ms = millis();
	/// if last ms less then 4 ms .. return 


	if ((current_ms - _last_ms) < 4) {
		return 0;
	}



	if (_target_pos > pot_current)  {
		// 
		if (_DIRECTION != DIRECTION_UP){

			_DIRECTION = DIRECTION_UP;
		}

	}
	if (_target_pos < pot_current)  {

		if (_DIRECTION != DIRECTION_DOWN){
			// neet to change direction 
			_DIRECTION = DIRECTION_DOWN;
		}

	}




	if (_last_direction == DIRECTION_OFF){
		//// START UP SPEED OF MOTOR 
		newspeed = MOTOR_START;
	}
	else{


		// caculate the time {disttrav} {timeval} {cur_vol}
		long  cur_vol = 0;
		long disttrav = _last_potavg - pot_avrage;
		disttrav = abs(disttrav);
		long timeval = current_ms - _last_ms;
		cur_vol = (disttrav * 1000) / timeval;

		//cur_vol = ((abs(_last_potavg - pot_avrage) * 1000) / (current_ms - _last_ms));



		///////////////////////////////////////////

		if (cur_vol == 0) {
  // use an avrage 
                    _cur_volavgcnt++;
                 //   if (_cur_lastvolavgcnt == 0 )
		 	newspeed = _last_speed + 1; // guess;
                 //   else{
                      // avg 
                 //     int inttrunc = _cur_lastvolavgcnt * _cur_volavgcnt;
                      
                 //      newspeed = newspeed + inttrunc;
                       
                      
                      
                //      }
                    }
		
                    
		else
		{
                      
                        
			if (cur_vol > target_vol) {
				// slow it down 
			//	newspeed = _last_speed - ((cur_vol - target_vol)  *  VOL_FACTOR);
				newspeed = _last_speed -   VOL_FACTOR;
			}
			if (cur_vol < target_vol) {

			//	newspeed = _last_speed + ((target_vol - cur_vol)  *  VOL_FACTOR);
				newspeed = _last_speed +      VOL_FACTOR;
			}

                         _cur_lastvolavgcnt = ((_last_speed - newspeed  ) / _cur_volavgcnt);
                        
                        _cur_volavgcnt = 0;
		}
		//////////////////////////////////////// 

		// keep the motor within the core speed 
		if (((newspeed > MOTOR_MAX) && (newspeed > 0)) || ((newspeed < (MOTOR_MAX * -1)) && (newspeed < 0)) ) {

			if (newspeed < (MOTOR_MAX * -1)) {
				// negitive 
				newspeed = (MOTOR_MAX * -1);

			}
			else {

				newspeed = MOTOR_MAX;
			}

		}


#ifdef DEBUG  

		Serial.print(" speed target  ");
		Serial.print(target_vol);
		Serial.print(" cur_vol ");
		Serial.println(cur_vol,DEC );
#endif



	}

	// now use that value ..


	/// if we are negitive add brake ... 
	if (newspeed < 0){

		if (_DIRECTION == DIRECTION_UP){
			_DIRECTION = DIRECTION_UP_BRAKE;
		}
		if (_DIRECTION == DIRECTION_DOWN){
			_DIRECTION = DIRECTION_DOWN_BRAKE;
		}
	}

	if (_last_direction != _DIRECTION){
#ifdef DEBUG
		Serial.println("CHANGE DIRECTION ........");
#endif
		analogWrite(_PIN_MOTOR, 0);
		digitalWrite(_PIN_MOTOR_UP, LOW);
		digitalWrite(_PIN_MOTOR_DOWN, LOW);
		//	newspeed = MOTOR_MIN;
		_last_speed = 0;
		delay(2); // dont want both on at same time 

		switch (_DIRECTION){


		case DIRECTION_OFF:
			Stop();
#ifdef DEBUG 	
			Serial.println("CHANGE DIRECTION DIRECTION_OFF");
#endif

			return 1;
			break;
		case DIRECTION_UP:

			donot_burn_out_hbridge(_PIN_MOTOR_UP, HIGH);






#ifdef DEBUG 
			Serial.println("CHANGE DIRECTION DIRECTION_UP");
#endif
			break;
		case DIRECTION_DOWN:


			donot_burn_out_hbridge(_PIN_MOTOR_DOWN, HIGH);
#ifdef DEBUG  
			Serial.println("CHANGE DIRECTION DIRECTION_DOWN");
#endif		
			break;

		case DIRECTION_DOWN_BRAKE:
      analogWrite(_PIN_MOTOR, 1);

			donot_burn_out_hbridge(_PIN_MOTOR_UP, HIGH);
#ifdef DEBUG  
			Serial.println("CHANGE DIRECTION DIRECTION_DOWN_BRAKE");
#endif    
			break;


		case DIRECTION_UP_BRAKE:

       analogWrite(_PIN_MOTOR, 1);
			donot_burn_out_hbridge(_PIN_MOTOR_DOWN, HIGH);
#ifdef DEBUG  
			Serial.println("CHANGE DIRECTION DIRECTION_UP_BRAKE");
#endif    
			break;

		case DIRECTION_RUN:

			Run(pot_avrage);

			return(0);
			break;

		}
	}

	// test for rev .brakeing 
	 
	 // get motor moving 
/*
	///
	if (_last_direction == DIRECTION_OFF){
		 
	  int startpot = analogRead(_PIN_POT);
	  while (abs(startpot - pot_avrage) < 5);
		{
		  analogWrite(_PIN_MOTOR, abs(newspeed));
		  startpot = analogRead(_PIN_POT);
		 
			
			if (newspeed >=  (MOTOR_MAX + 1)) {
			Stop();// not MOVING
       
			return(-1);

		    }
			newspeed++;

		} 
			
	  pot_avrage = analogRead(_PIN_POT);

	}
*/
// max break ... speed 

 if (( _DIRECTION == DIRECTION_DOWN_BRAKE ) || ( _DIRECTION == DIRECTION_UP_BRAKE ))
 {
  if (abs(newspeed) > MAX_BREAK)  
           newspeed = MAX_BREAK * -1 ;
   
 }



	analogWrite(_PIN_MOTOR, abs(newspeed));

	_last_speed = newspeed;
	_last_potavg = pot_avrage;
	_last_ms = current_ms;
	_last_direction = _DIRECTION;
#ifdef DEBUG 
	Serial.print(" _last_direction ");
	Serial.print(_last_direction);
	Serial.print(" _last_speed ");
	Serial.print(_last_speed);
	Serial.print(" _last_potavg ");
	Serial.println(_last_potavg);

#endif
	// {SIT_last_speed} {SIT_last_pot} {SIT_last_ms} {SIT_last_direction}

}

 

int MotorServo::GetDIRECTION(){

	return	int(_DIRECTION);



}
////////////////////////////////////////
int  MotorServo::Settarget_pos(int atarget_pos){



	_target_pos = atarget_pos;



	return	_target_pos;

}
////////////////////////////////////////
int  MotorServo::Gettarget_pos(){
	return	_target_pos;
}

int MotorServo::Current_pot(){

	return analogRead(_PIN_POT);
}






