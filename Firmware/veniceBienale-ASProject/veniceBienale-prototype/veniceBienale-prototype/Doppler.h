/*
* Doppler.h
*
* Created: 3/30/2018 4:09:12 PM
* Author: wes
*/

#include <Arduino.h>

#ifndef __DOPPLER_H__
#define __DOPPLER_H__


class Doppler
{
	//variables
	public:
	protected:
	private:
	int inputPin;
	
	bool debouncedState = false;
	
	bool countingMinorDebounce = false;
	unsigned long minorDebounceTime;
	
	bool countingMajorDebounce = false;
	unsigned long majorDebounceTime;
	unsigned long recentMajorDebounceTime;

	//functions
	public:
	Doppler(int _inputPin);
	bool analogToBool(int _analogThreshold, bool _invert);
	bool getStateLongDebounce(bool inputState, unsigned long _minorDebounce, unsigned long _majorDebounceLowThres, unsigned long _majorDebounceHighThres);
	int getStateSumSamples();
	protected:

}; //Doppler

#endif //__DOPPLER_H__




