
/*######################################################################

*/

#define LED1 8

#define FAN1_PIN 5    //connector by power input
#define FAN2_PIN 10   //connector in the center
#define FAN3_PIN 6    //connector far from power input

#define FAN1_DELAY 0
#define FAN2_DELAY 30
#define FAN3_DELAY 60

#define MUX_CONTROL 2 //ctrl pin for Analog Mux Switches (ABC)
#define MUX_CONTROL_DEFAULT LOW

#define IR_PIN1 A1  //
#define IR_PIN2 A5  //

#define DIST1_THRES 120
#define DIST2_THRES 400

#define STATE0_DEBOUNCE_LENGTH 1000
#define STATE1_DEBOUNCE_LENGTH 200




#define FAN1_INFHOLD_LENGTH 3000
#define FAN1_INFHOLD_INFLATEVAL 0
#define FAN1_INFHOLD_HOLDVAL 255
#define FAN1_INFHOLD_DELAY 0

#define FAN2_INFHOLD_LENGTH 3000
#define FAN2_INFHOLD_INFLATEVAL 0
#define FAN2_INFHOLD_HOLDVAL 120
#define FAN2_INFHOLD_DELAY 100

#define FAN3_INFHOLD_LENGTH 3000
#define FAN3_INFHOLD_INFLATEVAL 0
#define FAN3_INFHOLD_HOLDVAL 120
#define FAN3_INFHOLD_DELAY 200


#define FAN1_BREATHE_PERIOD 8000
#define FAN1_BREATHE_INFLATEVAL 255
#define FAN1_BREATHE_HOLDVAL 150
#define FAN1_BREATHE_DEFLATEVAL 0
#define FAN1_BREATHE_RESTARTVAL 120
#define FAN1_BREATHE_INFLATETIME 0.2
#define FAN1_BREATHE_HOLDTIME 0.5
#define FAN1_BREATHE_DEFLATETIME 0.1


#define FAN2_BREATHE_PERIOD 8000
#define FAN2_BREATHE_INFLATEVAL 200
#define FAN2_BREATHE_HOLDVAL 200
#define FAN2_BREATHE_DEFLATEVAL 0
#define FAN2_BREATHE_RESTARTVAL 120
#define FAN2_BREATHE_INFLATETIME 0.2
#define FAN2_BREATHE_HOLDTIME 0.2
#define FAN2_BREATHE_DEFLATETIME 0.5


#define FAN3_BREATHE_PERIOD 8000
#define FAN3_BREATHE_INFLATEVAL 255
#define FAN3_BREATHE_HOLDVAL 150
#define FAN3_BREATHE_DEFLATEVAL 0
#define FAN3_BREATHE_RESTARTVAL 150
#define FAN3_BREATHE_INFLATETIME 0.2
#define FAN3_BREATHE_HOLDTIME 0.4
#define FAN3_BREATHE_DEFLATETIME 0.3



//######################################################################

#include <Arduino.h>
#include "FanChannel.h"
#include "PCB_LED.h"
#include "IRIn.h"



// find out how to declare a class and initialize it later
PCB_LED fbLED1(LED1);
PCB_LED fbLED2(PIN_LED_RXL);
//PCB_LED fbLED1;

IRIn dist1(IR_PIN1,10);
IRIn dist2(IR_PIN2,10);

FanChannel fan1(FAN1_PIN, FAN1_DELAY);
FanChannel fan2(FAN2_PIN, FAN2_DELAY);
FanChannel fan3(FAN3_PIN, FAN3_DELAY);

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


unsigned long lastInflated;


void detectStates(){
	//  volatile int dist2log = dist2.getAverage();
	//  volatile int dist1log = dist1.getAverage();
	//nothing detected
	if(dist1.getAverage() < DIST1_THRES)// && dist2.getAverage() < DIST2_THRES)
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
	if(dist1.getAverage() >= DIST1_THRES)// || dist2.getAverage() >= DIST2_THRES)
	{
		
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

bool newBreath = true;

void switchStates(){
	switch (state)
	{
		case 1:
		SerialUSB.print("\tDeflating\t");
		fan1.deflate(0, 30);
		fan2.deflate(0, 30);
		fan3.deflate(0, 20);
		// 		if(millis() - lastInflated > 500){
		// 			fan1inf = true;
		// 			fan2inf = true;
		// 			fan3inf = true;
		// 		}
		// 		holdStarted = true;
		// 		if(fan1inf || fan3inf || fan2inf){
		// 			lastInflated = millis();
		// 			if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_LENGTH, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL);
		// 			if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_LENGTH, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL);
		// 			if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_LENGTH, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL);
		// 		}
		newBreath = true;
		break;
		


		case 0:
		SerialUSB.print("\tBreathing\t");
		
		if(newBreath){
			newBreath = false;
			analogWrite(5, 255);
			analogWrite(10, 255);
			analogWrite(6, 255);
			delay(4000);
		}
		
		
		fan1.breathe2(25,20, 3500, 2700, 255, 0);
		fan2.breathe2(28,17, 3700, 2500, 255, 0); // esquerda
		fan3.breathe2(25,20, 3100, 3100, 255, 0);
		
		if(false){
			
			if(fan1.getBreathCount() > 100){
				fan1.setBreathCount(0);
			}
			if(fan2.getBreathCount() > 100){
				fan2.setBreathCount(0);
			}
			if(fan3.getBreathCount() > 100){
				fan3.setBreathCount(0);
			}
			
			fan1.breathe(
			FAN1_BREATHE_PERIOD,
			FAN1_BREATHE_INFLATEVAL,  FAN1_BREATHE_HOLDVAL, FAN1_BREATHE_DEFLATEVAL,  FAN1_BREATHE_RESTARTVAL,
			FAN1_BREATHE_INFLATETIME, FAN1_BREATHE_HOLDTIME,  FAN1_BREATHE_DEFLATETIME);

			fan2.breathe(
			FAN2_BREATHE_PERIOD,
			FAN2_BREATHE_INFLATEVAL,  FAN2_BREATHE_HOLDVAL, FAN2_BREATHE_DEFLATEVAL,  FAN2_BREATHE_RESTARTVAL,
			FAN2_BREATHE_INFLATETIME, FAN2_BREATHE_HOLDTIME,  FAN2_BREATHE_DEFLATETIME);
			
			fan3.breathe(
			FAN3_BREATHE_PERIOD,
			FAN3_BREATHE_INFLATEVAL,  FAN3_BREATHE_HOLDVAL, FAN3_BREATHE_DEFLATEVAL,  FAN3_BREATHE_RESTARTVAL,
			FAN3_BREATHE_INFLATETIME, FAN3_BREATHE_HOLDTIME,  FAN3_BREATHE_DEFLATETIME);
		}
		break;
	} // switch
}



void setup()
{
	SerialUSB.begin(250000);
	//pinMode(LED1, OUTPUT); pin is initialized in PCB_LED fbLED1
	pinMode(PIN_LED_RXL,OUTPUT);
	digitalWrite(PIN_LED_RXL,LOW);
	
	pinMode(PIN_LED_TXL,OUTPUT);
	digitalWrite(PIN_LED_TXL,LOW);
	
	
	// set MUX to point to connectors
	pinMode(MUX_CONTROL,OUTPUT);
	digitalWrite(MUX_CONTROL,MUX_CONTROL_DEFAULT);

	// some blinking to signal boot
	for(int i = 0; i < 5; i ++){
		analogWrite(LED1, 127);
		delay(100);
		analogWrite(LED1, 0);
		delay(100);
	}
	
	if(fan1inf || fan3inf || fan2inf){
		if(fan1inf) fan1inf = fan1.inflateAndHold(FAN1_INFHOLD_LENGTH, FAN1_INFHOLD_INFLATEVAL,FAN1_INFHOLD_HOLDVAL);
		if(fan2inf) fan2inf = fan2.inflateAndHold(FAN2_INFHOLD_LENGTH, FAN2_INFHOLD_INFLATEVAL,FAN2_INFHOLD_HOLDVAL);
		if(fan3inf) fan3inf = fan3.inflateAndHold(FAN3_INFHOLD_LENGTH, FAN3_INFHOLD_INFLATEVAL,FAN3_INFHOLD_HOLDVAL);
	}
	analogWrite(5, 255);
	analogWrite(10, 255);
	analogWrite(6, 255);
	delay(20000);
}

uint8_t i=0;

unsigned long lastPrint = 0;
void loop()
{
	
	if(millis() - lastPrint > 100){
		SerialUSB.print(millis());
		SerialUSB.print("\t");
		SerialUSB.print(state);
		SerialUSB.print("\t");
		SerialUSB.print(analogRead(A1));
		SerialUSB.print("\t");
		SerialUSB.println(dist1.getAverage());
	}
	// 	// 	delay(20);
	// 	//for(;;){
	// 	// heartBeats, aka the board is running
	// 	fbLED1.heartBeatAnalog(30,255,1,100);
	// 	if(state == 0) fbLED2.heartBeatDigital(500,0.5);
	// 	if(state == 1) fbLED2.heartBeatDigital(500,0);
	//
	// 	// add readings to the arrays
	dist1.addReading();
	// 	//dist2.addReading();
	//
	//
	//state = 0;
	// 	// use the readings to decide what to do
	detectStates();
	//
	// 	// does what was decided in detectStates()
	switchStates();


	
	
} // loop()

