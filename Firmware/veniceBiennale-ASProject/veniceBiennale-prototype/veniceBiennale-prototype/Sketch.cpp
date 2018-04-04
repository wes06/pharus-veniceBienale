/*######################################################################




*/

#define LED1 8

#define FAN1_PIN 5		//connector by power input
#define FAN2_PIN 10		//connector in the center
#define FAN3_PIN 6		//connector far from power input

#define FAN1_DELAY 0
#define FAN2_DELAY 30
#define FAN3_DELAY 60

#define MUX_CONTROL 2	//ctrl pin for Analog Mux Switches (ABC)
#define MUX_CONTROL_DEFAULT LOW

#define DOPPLER_PIN A1	//

#define DOPPLER_READINGS_HISTORY 10 // amount of seconds to keep readings of the doppler sensor for
#define DOPPLER_READINGS_PERSEC 10
#define DOPPLER_ARRAY_LENGTH (DOPPLER_READINGS_HISTORY*DOPPLER_READINGS_PERSEC)

#define DIST1_THRES 340
#define DIST2_THRES 340

#define STATE0_DEBOUNCE_LENGTH 2000
#define STATE1_DEBOUNCE_LENGTH 2000




#define FAN1_INFHOLD_LENGTH 2000
#define FAN1_INFHOLD_INFLATEVAL 255
#define FAN1_INFHOLD_HOLDVAL 10
#define FAN1_INFHOLD_DELAY 0

#define FAN2_INFHOLD_LENGTH 2000
#define FAN2_INFHOLD_INFLATEVAL 255
#define FAN2_INFHOLD_HOLDVAL 10
#define FAN2_INFHOLD_DELAY 30

#define FAN3_INFHOLD_LENGTH 2000
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


#define FAN2_BREATHE_PERIOD 5000
#define FAN2_BREATHE_INFLATEVAL 255
#define FAN2_BREATHE_HOLDVAL 200
#define FAN2_BREATHE_DEFLATEVAL 0
#define FAN2_BREATHE_RESTARTVAL 100
#define FAN2_BREATHE_INFLATETIME 0.2
#define FAN2_BREATHE_HOLDTIME 0.3
#define FAN2_BREATHE_DEFLATETIME 0.3


#define FAN3_BREATHE_PERIOD 5000
#define FAN3_BREATHE_INFLATEVAL 255
#define FAN3_BREATHE_HOLDVAL 200
#define FAN3_BREATHE_DEFLATEVAL 0
#define FAN3_BREATHE_RESTARTVAL 100
#define FAN3_BREATHE_INFLATETIME 0.2
#define FAN3_BREATHE_HOLDTIME 0.3
#define FAN3_BREATHE_DEFLATETIME 0.3



//######################################################################

#include <Arduino.h>
#include "Doppler.h"
#include "FanChannel.h"
#include "PCB_LED.h"
#include "IRIn.h"





// find out how to declare a class and initialize it later
PCB_LED fbLED1(LED1);
PCB_LED fbLED2(PIN_LED_RXL);
//PCB_LED fbLED1;

FanChannel fan1(FAN1_PIN, FAN1_DELAY);
FanChannel fan2(FAN2_PIN, FAN2_DELAY);
FanChannel fan3(FAN3_PIN, FAN3_DELAY);

Doppler doppler(DOPPLER_PIN);

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



}


void switchStates(){
	switch (state)
	{
		case 0:
		holdStarted = true;
		if(fan1inf || fan3inf || fan2inf){
			if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_LENGTH, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL);
			if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_LENGTH, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL);
			if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_LENGTH, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL);
		}
		break;
		
		case 1:		fan1.breathe(		FAN1_BREATHE_PERIOD,		FAN1_BREATHE_INFLATEVAL,	FAN1_BREATHE_HOLDVAL,	FAN1_BREATHE_DEFLATEVAL,	FAN1_BREATHE_RESTARTVAL,		FAN1_BREATHE_INFLATETIME,	FAN1_BREATHE_HOLDTIME,	FAN1_BREATHE_DEFLATETIME);		fan2.breathe(
		FAN2_BREATHE_PERIOD,
		FAN2_BREATHE_INFLATEVAL,	FAN2_BREATHE_HOLDVAL,	FAN2_BREATHE_DEFLATEVAL,	FAN2_BREATHE_RESTARTVAL,
		FAN2_BREATHE_INFLATETIME,	FAN2_BREATHE_HOLDTIME,	FAN2_BREATHE_DEFLATETIME);		
		fan3.breathe(
		FAN3_BREATHE_PERIOD,
		FAN3_BREATHE_INFLATEVAL,	FAN3_BREATHE_HOLDVAL,	FAN3_BREATHE_DEFLATEVAL,	FAN3_BREATHE_RESTARTVAL,
		FAN3_BREATHE_INFLATETIME,	FAN3_BREATHE_HOLDTIME,	FAN3_BREATHE_DEFLATETIME);
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
		if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_LENGTH, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL);
		if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_LENGTH, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL);
		if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_LENGTH, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL);
	}

}


void loop()
{
	
	for(;;){
		// heartBeats, aka the board is running
		fbLED1.heartBeatAnalog(30,255,1,100);
		if(state == 0) fbLED2.heartBeatDigital(500,0.5);
		if(state == 1) fbLED2.heartBeatDigital(500,0);
		
// 		// add readings to the arrays
// 		dist1.addReading();
// 		dist2.addReading();

		doppler.addReading();

		// use the readings to decide what to do
		detectStates();

		// does what was decided in detectStates()
		switchStates();
		

	}	//  for(;;)

}	// loop()
