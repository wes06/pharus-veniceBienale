/*
* Doppler.cpp
*
* Created: 3/30/2018 4:09:12 PM
* Author: wes
*/

#include <Arduino.h>
#include "Doppler.h"

// default constructor
Doppler::Doppler(int _inputPin)
:inputPin(_inputPin), readingPeriod(1000/DOPPLER_READINGS_PERSEC) 
{
	pinMode(inputPin, INPUT);
	//this->fillArray(0);
} //Doppler


bool Doppler::analogToBool(int _analogThreshold, bool _invert){
	return (analogRead(inputPin) > _analogThreshold);
}





// ##### minor debounce
// reads pin, check if it has detected movement
// if yes, set wait for reading the pin again in n ms
// ##### major debounce
// if movement is still detected, start countingMajorDebounce
// inside a window of 10 and 20 seconds, if it detects movement, change state
// if not, reset countingMajorDebounce to last movement detected prior to window
bool Doppler::getStateLongDebounce(bool inputState, unsigned long _minorDebounce, unsigned long _majorDebounceLowThres, unsigned long _majorDebounceHighThres){
	
	if(!countingMajorDebounce)
	{
		if(inputState && !countingMinorDebounce){
			countingMinorDebounce = true;
			minorDebounceTime = millis();
		}
		
		if(inputState && countingMinorDebounce && millis() - minorDebounceTime > _minorDebounce){
			majorDebounceTime = minorDebounceTime;
			countingMajorDebounce = true;
			countingMinorDebounce = false;
		}
	}
	else if(millis() - majorDebounceTime < _majorDebounceLowThres){
		if(inputState) recentMajorDebounceTime = millis();
		else recentMajorDebounceTime = majorDebounceTime;
	}
	else if (millis() - majorDebounceTime > _majorDebounceLowThres && millis() - majorDebounceTime < _majorDebounceHighThres )
	{
		debouncedState ^= inputState;
	}
	else if(millis() - majorDebounceTime > _majorDebounceHighThres)
	{
		countingMajorDebounce = false;
	}
	else
	{
		while(1)
		{
			//catch errors
		}
	}
	
	return debouncedState;
}



void Doppler::addReading(){	
	if(millis() - dopplerCurrentReadingTime > readingPeriod){
		dopplerCurrentReadingTime = millis();
		dopplerArray[dopplerSampleBeingUpdated] = this->analogToBool(300, false);
		
		dopplerSampleBeingUpdated++;
		if (dopplerSampleBeingUpdated==DOPPLER_ARRAY_LENGTH) dopplerSampleBeingUpdated;
	}
	
}


// sample every 100ms for example
// for the last 10 seconds, sum how many of the samples were positive
// if it passes a threshold, change state
int Doppler::getStateSumSamples(){
	
	
	
	
}
