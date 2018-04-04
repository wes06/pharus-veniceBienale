/*
* IRIn.h
*
* Created: 3/24/2018 3:17:11 PM
* Author: wes
*/

#include <Arduino.h>
#ifndef __IRIN_H__
#define __IRIN_H__

#define SAMPLE_COUNT 100

class IRIn
{
	//variables
	public:
	protected:
	private:
	const int inputPin;
	int sampleBeingUpdated = 0;
	int readings[SAMPLE_COUNT];
	
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


}; //IRIn

#endif //__IRIN_H__