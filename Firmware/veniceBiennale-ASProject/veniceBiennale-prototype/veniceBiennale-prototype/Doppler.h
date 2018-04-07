/*
* Doppler.h
*
* Created: 3/30/2018 4:09:12 PM
* Author: wes
*/

#include <Arduino.h>

#ifndef __DOPPLER_H__
#define __DOPPLER_H__

#ifndef DOPPLER_ARRAY_LENGTH
#define DOPPLER_ARRAY_LENGTH 4000
#endif

#ifndef DOPPLER_READINGS_PERSEC
#define DOPPLER_READINGS_PERSEC 100
#endif


class Doppler
{
	//variables
	public:
	protected:
	private:
	const int inputPin;
	const int readingPeriod;
	
	bool nextReading = true;
	unsigned long dopplerCurrentReadingTime = 0;
	
	int dopplerSampleBeingUpdated = 0;
	bool dopplerArray[DOPPLER_ARRAY_LENGTH];
	
	bool debouncedState = false;
	
	bool countingMinorDebounce = false;
	unsigned long minorDebounceTime;
	
	bool countingMajorDebounce = false;
	unsigned long majorDebounceTime;
	unsigned long recentMajorDebounceTime;

	//functions
	public:
	Doppler(int _inputPin);
	void addReading();
	
	bool analogToBool(int _analogThreshold, bool _invert);
	bool getStateLongDebounce(bool inputState, unsigned long _minorDebounce, unsigned long _majorDebounceLowThres, unsigned long _majorDebounceHighThres);
	int getStateSumSamples();
	protected:

}; //Doppler

#endif //__DOPPLER_H__




