/*
* FanChannel.cpp
*
* Created: 3/20/2018 10:03:16 PM
* Author: wes
*/
#include <Arduino.h>

#include "FanChannel.h"

// default constructor
FanChannel::FanChannel(int _ctrlPin){
	controlPin = _ctrlPin;
	pinMode(controlPin, OUTPUT);
} //FanChannel

// default destructor
// FanChannel::~FanChannel()
// {
// } //~FanChannel

	bool inflating = false;
	long inflateStartedMillis;
// turns fan on with _inflateValue for _period millis, then turns the fan on _holdValue permanently
bool FanChannel::inflateAndHold(int _period, int _inflateValue, int _holdValue)
{


	
	int bla = _inflateValue;
	
	if (!inflating){
		inflateStartedMillis = millis();
		if(_inflateValue == 255){
			digitalWrite(controlPin, HIGH);
		}
		else{
			analogWrite(controlPin, _inflateValue);
		}
		inflating = true;
	}
	else{
		if(millis() - inflateStartedMillis > _period){
			analogWrite(controlPin, _holdValue);
			inflating = false;
			return false; // done inflating, now holding
		}
	}
	return true;
}
// 
// void FanChannel::breathe(int _inflateValue, int _periodo)
// {
// 	int breathingState;
// 	long breathStart;
// 	
// 	if(breathingState == 0){
// 		breathingState++;
// 		breathStart = millis();
// 		if(_inflateValue == 255)
// 		{
// 			digitalWrite(controlPin, HIGH);
// 		}
// 		else
// 		{
// 			analogWrite(controlPin, _inflateValue);
// 		}
// 		
// 	}
// 	
// 	if(breathingState == 1 && millis() - breathStart > _periodo/2)
// 	{
// 		breathingState++;
// 		digitalWrite(controlPin, LOW);
// 	}
// 	if(breathingState == 2 && millis() - breathStart > _periodo)
// 	{
// 		breathingState = 0;
// 	}
// }
// 
// void FanChannel::breatheErratically()
// {
// 	int breathingErraticallyState;
// 	long breatheErraticallyStart;
// 	
// 	if (breathingErraticallyState == 0)
// 	{ // turn fan on
// 		breathingErraticallyState++;
// 		breatheErraticallyStart = millis();
// 		digitalWrite(controlPin, HIGH);
// 	}
// 	if(breathingErraticallyState == 1 && millis() - breatheErraticallyStart > 1000)
// 	{ // if __ time has passed, turn it off
// 		digitalWrite(controlPin, LOW);
// 		breathingErraticallyState++;
// 	}
// 	if(breathingErraticallyState == 2 && millis() - breathingErraticallyState > 200)
// 	{ // wait some time
// 		breathingErraticallyState = 0;
// 	}
// 	
// }