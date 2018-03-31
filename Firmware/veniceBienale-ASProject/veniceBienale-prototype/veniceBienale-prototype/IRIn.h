/*
* IRIn.h
*
* Created: 3/24/2018 3:17:11 PM
* Author: wes
*/

#include <Arduino.h>
#ifndef __IRIN_H__
#define __IRIN_H__


class IRIn
{
	//variables
	public:
	protected:
	private:
	const int inputPin;
	
	const int sampleCount = 100;
	int sampleBeingUpdated = 0;
	int readings[100];
	
	long readingsSum;
	int readingsAvg;
	
	int triggerVal;
	bool triggerBool = true;
	
	bool newTrigger = true;
	unsigned long debounceTime;
	int debounceLength = 100;

	//functions
	public:
	IRIn();
	IRIn(int _inputPin);
	IRIn(int _inputPin, int _delay);
	void fillArray(int _delay);
	void addReading();
	int getAverage();
//	void setTriggerVal(int _triggerVal);
//	void setTriggerBool(bool _triggerBool);
// 	void setDebounceLength(int _debounceLength);
// 	bool debouncedInput();


}; //IRIn

#endif //__IRIN_H__