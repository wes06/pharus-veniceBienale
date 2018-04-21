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


void FanChannel::deflate(unsigned int _deflateVal, unsigned int _holdDeflateVal)
{
	

	if(deflateAndHoldState == 0 && !deflating){
		analogWrite(controlPin, _deflateVal);
		deflateStartedMillis = millis();
		deflateAndHoldState++;
		deflating = true;
		SerialUSB.print(" ");
		SerialUSB.print("deflateState0");
		SerialUSB.print(" ");
	}
	if(deflateAndHoldState == 1 && millis() - deflateStartedMillis > 15000)
	{
		analogWrite(controlPin, 250);
		deflateAndHoldState++;
		SerialUSB.print(" ");
		SerialUSB.print("deflateState1");
		SerialUSB.print(" ");
	}
	if(deflateAndHoldState == 2 && millis() - deflateStartedMillis > 15200)
	{
		analogWrite(controlPin, _holdDeflateVal);
		deflateAndHoldState = 0;
		SerialUSB.print(" ");
		SerialUSB.print("deflateState1");
		SerialUSB.print(" ");
		
	}
}


//Fan turns on and off
void FanChannel::breathe(
int _periodo,
int _inflateVal, int _holdVal, int _deflateVal, int _restartVal,
float _inflateTime, float _holdTime, float _deflateTime)
{
	int breathVal;
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
		this->setBreathCount(this->getBreathCount()+1);
	}
}



//Fan turns on and off
void FanChannel::breathe2(unsigned int rampUpStep, unsigned int rampDownStep, unsigned int highWait, unsigned int lowWait, unsigned int highVal, unsigned int lowVal)
{
	this->deflating = false;
	deflateAndHoldState = 0;
	SerialUSB.print(" ");
	SerialUSB.print(breatheVal);
	SerialUSB.print(" ");
	// pulse it at 100%
	if(rampState == 0){
		breathe2Start = millis();
		analogWrite(controlPin, 230);
		rampState++;
	}
	
	if(rampState == 1 && millis() - breathe2Start > 200)
	{
		rampState++;
	}
	
	if(rampState == 2)
	{//ramp up
		
		if(millis() - breathe2Start > rampUpStep){
			analogWrite(controlPin, breatheVal);
			breathe2Start = millis();
			
			breatheVal++;
		}
		if(breatheVal >= highVal) {
			rampState++;
			breathe2Start = millis();
		}
	}

	if(rampState == 3 && millis() - breathe2Start > highWait){
		rampState++;
	}
	
	if (rampState == 4){
		
		if(millis() - breathe2Start > rampDownStep){
			analogWrite(controlPin, breatheVal);
			breathe2Start = millis();
			breatheVal--;
		}
		if(breatheVal <= lowVal) {
			rampState++;
			breathe2Start = millis();
		}
	}
	
	if(rampState == 5 && millis() - breathe2Start > lowWait){
		rampState = 0;
	}

}






unsigned int FanChannel::getBreathCount(){
	return breathCount;
}

void FanChannel::setBreathCount(int _breathCount){
	breathCount = _breathCount;
}