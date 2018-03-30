#include <Arduino.h>
#include "FanChannel.h"
#include "PCB_LED.h"
#include "IRIn.h"


/*######################################################################



*/

#define LED1 8

#define FAN_PIN1 5		//connector by power input
#define FAN_PIN2 10		//connector in the center
#define FAN_PIN3 6		//connector far from power input

#define MUX_CONTROL 2	//ctrl pin for Analog Mux Switches (ABC)
#define MUX_CONTROL_DEFAULT LOW

#define IR_PIN1 A5	//
#define IR_PIN2 A1	//

#define DIST1_THRES 400
#define DIST2_THRES 400

#define STATE0_DEBOUNCE_LENGTH 2000
#define STATE1_DEBOUNCE_LENGTH 2000




#define FAN1_INFHOLD_PERIOD 2000
#define FAN1_INFHOLD_INFLATEVAL 255
#define FAN1_INFHOLD_HOLDVAL 10
#define FAN1_INFHOLD_DELAY 0

#define FAN2_INFHOLD_PERIOD 2000
#define FAN2_INFHOLD_INFLATEVAL 255
#define FAN2_INFHOLD_HOLDVAL 10
#define FAN2_INFHOLD_DELAY 30

#define FAN3_INFHOLD_PERIOD 2000
#define FAN3_INFHOLD_INFLATEVAL 255
#define FAN3_INFHOLD_HOLDVAL 10
#define FAN3_INFHOLD_DELAY 60


#define FAN1_BREATHE_PERIOD 5000
#define FAN1_BREATHE_INFLATEVAL 255
#define FAN1_BREATHE_HOLDVAL 200
#define FAN1_BREATHE_DEFLATEVAL 0
#define FAN1_BREATHE_RESTARTVAL 100
#define FAN1_BREATHE_INFLATETIME 0.2
#define FAN1_BREATHE_HOLDTIME 0.3
#define FAN1_BREATHE_DEFLATETIME 0.3
#define FAN1_BREATHE_DELAY 0


#define FAN2_BREATHE_PERIOD 5000
#define FAN2_BREATHE_INFLATEVAL 255
#define FAN2_BREATHE_HOLDVAL 200
#define FAN2_BREATHE_DEFLATEVAL 0
#define FAN2_BREATHE_RESTARTVAL 100
#define FAN2_BREATHE_INFLATETIME 0.2
#define FAN2_BREATHE_HOLDTIME 0.3
#define FAN2_BREATHE_DEFLATETIME 0.3
#define FAN2_BREATHE_DELAY 30


#define FAN3_BREATHE_PERIOD 5000
#define FAN3_BREATHE_INFLATEVAL 255
#define FAN3_BREATHE_HOLDVAL 200
#define FAN3_BREATHE_DEFLATEVAL 0
#define FAN3_BREATHE_RESTARTVAL 100
#define FAN3_BREATHE_INFLATETIME 0.2
#define FAN3_BREATHE_HOLDTIME 0.3
#define FAN3_BREATHE_DEFLATETIME 0.3
#define FAN3_BREATHE_DELAY 60



//######################################################################


// find out how to declare a class and initialize it later
PCB_LED fbLED1(LED1);
PCB_LED fbLED2(PIN_LED_RXL);
//PCB_LED fbLED1;

IRIn dist1(IR_PIN1,10);
IRIn dist2(IR_PIN2,10);

FanChannel fan1(FAN_PIN1);
FanChannel fan2(FAN_PIN2);
FanChannel fan3(FAN_PIN3);

boolean fan1inf = true;
bool fan2inf = true;
bool fan3inf = true;

//detectState vars
int state = 0;
bool state0Waiting = false;
unsigned long state0Millis;
bool state1Waiting = false;
unsigned long state1Millis;

bool holdStarted = false;


void detectStates(){
	volatile int dist2log = dist2.getAverage();
	volatile int dist1log = dist1.getAverage();
	//nothing detected
	if(dist1.getAverage() < DIST1_THRES && dist2.getAverage() < DIST2_THRES)
	{
		
		// keep inflated after 2secs of debounce
		if (!state0Waiting)
		{
			state0Millis = millis();
			state0Waiting = true;
		}
		else if(millis() - state0Millis > STATE0_DEBOUNCE_LENGTH){
			state = 0;
			// if it hasn't started inflating, reset the vars
			// if it started inflating, will not set fanXinf to true
			if(!holdStarted){
				fan1inf = true;
				fan2inf = true;
				fan3inf = true;
			}
		}
	}
	else
	{
		state0Waiting = false;
		holdStarted = false;
	} // end of nothing detected
	
	// something detected
	if(dist1.getAverage() >= DIST1_THRES || dist2.getAverage() >= DIST2_THRES){
		
		// start breathing after 2 secs of debounce
		if (!state1Waiting)
		{
			state1Millis = millis();
			state1Waiting = true;
		}
		else if(millis() - state1Millis > STATE1_DEBOUNCE_LENGTH) state = 1;
	}
	else
	{
		state1Waiting = false;
	} // end of something detected
	
}


void switchStates(){
	switch (state)
	{
		case 0:
		holdStarted = true;
		if(fan1inf || fan3inf || fan2inf){
			if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_PERIOD, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL, FAN1_INFHOLD_DELAY);
			if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_PERIOD, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL, FAN2_INFHOLD_DELAY);
			if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_PERIOD, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL, FAN3_INFHOLD_DELAY);
		}
		break;
		


		case 1:		fan1.breathe(		FAN1_BREATHE_PERIOD,		FAN1_BREATHE_INFLATEVAL,	FAN1_BREATHE_HOLDVAL,	FAN1_BREATHE_DEFLATEVAL,	FAN1_BREATHE_RESTARTVAL,		FAN1_BREATHE_INFLATETIME,	FAN1_BREATHE_HOLDTIME,	FAN1_BREATHE_DEFLATETIME,		FAN1_BREATHE_DELAY);		fan2.breathe(
		FAN2_BREATHE_PERIOD,
		FAN2_BREATHE_INFLATEVAL,	FAN2_BREATHE_HOLDVAL,	FAN2_BREATHE_DEFLATEVAL,	FAN2_BREATHE_RESTARTVAL,
		FAN2_BREATHE_INFLATETIME,	FAN2_BREATHE_HOLDTIME,	FAN2_BREATHE_DEFLATETIME,
		FAN2_BREATHE_DELAY);		
		fan3.breathe(
		FAN3_BREATHE_PERIOD,
		FAN3_BREATHE_INFLATEVAL,	FAN3_BREATHE_HOLDVAL,	FAN3_BREATHE_DEFLATEVAL,	FAN3_BREATHE_RESTARTVAL,
		FAN3_BREATHE_INFLATETIME,	FAN3_BREATHE_HOLDTIME,	FAN3_BREATHE_DEFLATETIME,
		FAN3_BREATHE_DELAY);
		break;
		
		
		case 2:
		break;
		case 3:
		break;
	} // switch
}



void setup()
{
	//pinMode(LED1, OUTPUT); pin is initialized in PCB_LED fbLED1
	pinMode(PIN_LED_RXL,OUTPUT);
	digitalWrite(PIN_LED_RXL,LOW);
	
	pinMode(PIN_LED_TXL,OUTPUT);
	digitalWrite(PIN_LED_TXL,LOW);
	
	
	// set MUX to point to connectors instead of SMD pads
	pinMode(MUX_CONTROL,OUTPUT);
	digitalWrite(MUX_CONTROL,MUX_CONTROL_DEFAULT);


	// some blinking to signal boot
	for(int i = 0; i < 5; i ++){
		analogWrite(LED1, 255);
		delay(100);
		analogWrite(LED1, 0);
		delay(100);
	}
	
		if(fan1inf || fan3inf || fan2inf){
			if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_PERIOD, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL, FAN1_INFHOLD_DELAY);
			if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_PERIOD, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL, FAN2_INFHOLD_DELAY);
			if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_PERIOD, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL, FAN3_INFHOLD_DELAY);
		}

}


void loop()
{
	for(;;){
		// heartBeat, aka the board is running
		fbLED1.heartBeatAnalog(30,255,1,100);
		
		// add readings to the arrays
		dist1.addReading();
		dist2.addReading();

		// use the readings to decide what to do
		detectStates();

		// does what was decided in detectStates()
		switchStates();
		
		if(state == 0) fbLED2.heartBeatDigital(500,0.5);
		if(state == 1) fbLED2.heartBeatDigital(500,0);
	}	//  for(;;)
}	// loop()
