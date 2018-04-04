/*
* PCB_LED.cpp
*
* Created: 3/28/2018 6:21:20 PM
* Author: wes
*/

#include <Arduino.h>
#include "PCB_LED.h"

// default constructor
PCB_LED::PCB_LED(int _ctrlPin)
{
	controlPin = _ctrlPin;
	pinMode(controlPin, OUTPUT);
} //PCB_LED





void PCB_LED::heartBeatDigital(int _period, float _dutyCycle)
//:int lowPeriod = static_cast< int >(static_cast< float >((_period)*_dutyCycle)),
//int highPeriod = static_cast< int >(static_cast< float >((_period)*(1.0f-_dutyCycle)),
{
	if(heartBeatDigitalState == 0){
		heartBeatDigitalState++;
		heartBeatDigitalStart = millis();
		//digitalWrite(controlPin, HIGH);
		digitalWrite(controlPin,HIGH);
	}

	if(heartBeatDigitalState == 1 &&  millis() - heartBeatDigitalStart > static_cast< float >((_period)*_dutyCycle))
	{
		heartBeatDigitalStart = millis();
		heartBeatDigitalState++;
		//digitalWrite(controlPin, LOW);
		digitalWrite(controlPin,LOW);
	}
	if(heartBeatDigitalState == 2 && millis() - heartBeatDigitalStart > static_cast< float >((_period)*(1.0f-_dutyCycle)))
	{
		heartBeatDigitalState = 0;
	}
	
}



void PCB_LED::heartBeatAnalog(int _minVal, int _maxVal, int _frameTime, int _waitTime){
	
	int maxVal = _maxVal;
	int minVal = _minVal;
	int frameTime = _frameTime;
	int waitTime = _waitTime;
	
	
	// heartBeat started, goes up
	if(heartBeatAnalogState == 0){
		if(millis() - heartBeatAnalogStart > frameTime || heartBeatBegin){
			heartBeatBegin = false;
			heartBeatAnalogStart = millis();
			heartBeatVal++;
			analogWrite(controlPin, gamma8[heartBeatVal]);
			if(heartBeatVal == maxVal)	heartBeatAnalogState++;
		}
	}
	
	
	// reached peak, stop, wait
	if(heartBeatAnalogState == 1 && millis() - heartBeatAnalogStart > waitTime)
	{
		heartBeatAnalogStart = millis();
		heartBeatAnalogState++;
	}
	
	// heartBeat goes down
	if(heartBeatAnalogState == 2)
	{
		if(millis() - heartBeatAnalogStart > frameTime)
		{
			heartBeatAnalogStart = millis();
			heartBeatVal--;
			analogWrite(controlPin, gamma8[heartBeatVal]);
		}
		if(heartBeatVal == minVal) heartBeatAnalogState++;
	}
	
	//reached bottom, stop, wait
	if(heartBeatAnalogState == 3 && millis() - heartBeatAnalogStart > waitTime)
	{
		heartBeatBegin = true;
		heartBeatAnalogState = 0;
	}
}




