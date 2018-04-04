/*
* FanChannel.cpp
*
* Created: 3/20/2018 10:03:16 PM
* Author: wes
*/
#include <Arduino.h>
#include "FanChannel.h"

// default constructor
FanChannel::FanChannel(int _ctrlPin, int _fanDelay)
: controlPin(_ctrlPin), fanDelay(_fanDelay)
{
	pinMode(controlPin, OUTPUT);
} //FanChannel




// turns fan on with '_inflateValue' for '_period' time, then turns the fan on '_holdValue' permanently
// _delay is a delay on start up time, should not influence _period
bool FanChannel::inflateAndHold(int _period, int _inflateValue, int _holdValue)
{
	if (inflateAndHoldState == 0){
		inflateStartedMillis = millis();
		inflateAndHoldState = 1;
	}
	
	if(inflateAndHoldState == 1 && millis() - inflateStartedMillis >= fanDelay)
	{
		analogWrite(controlPin, _inflateValue);
		inflateAndHoldState = 2;
	}
	
	if(inflateAndHoldState == 2 && millis() - inflateStartedMillis > _period - fanDelay){
		analogWrite(controlPin, _holdValue);
		inflateAndHoldState = 3;
	}
	if(inflateAndHoldState == 3 && millis() - inflateStartedMillis > _period){
		inflateAndHoldState = 0;
		return false; // done inflating, now holding
	}
	return true;
}




//Fan turns on and off
void FanChannel::breathe(
int _periodo,
int _inflateVal, int _holdVal, int _deflateVal, int _restartVal,
float _inflateTime, float _holdTime, float _deflateTime)
{
	if(breathingState == 0){
		breathingState++;
		breathStart = millis();
	}
	
	if(breathingState == 1 && millis() - breathStart >= fanDelay)
	{
		analogWrite(controlPin, _inflateVal);
		breathingState++;
	}
	
	if(breathingState == 2 && millis() - breathStart > static_cast< float >(_periodo)*_inflateTime)
	{
		analogWrite(controlPin, _holdVal);
		breathingState++;
	}
	
	if(breathingState == 3 &&  millis() - breathStart > static_cast< float >(_periodo)*(_inflateTime + _holdTime) - fanDelay)
	{
		analogWrite(controlPin,_deflateVal);
		breathingState++;
	}
	
	if(breathingState == 4 && millis() - breathStart > static_cast< float >(_periodo)*(_inflateTime + _holdTime + _deflateTime) + fanDelay)
	{
		analogWrite(controlPin,_restartVal);
		breathingState++;
	}
	
	if(breathingState == 5 && millis() - breathStart > _periodo)
	{
		breathingState= 0;
	}
}
