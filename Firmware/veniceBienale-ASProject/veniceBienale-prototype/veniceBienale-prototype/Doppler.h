/*
* Doppler.h
*
* Created: 3/30/2018 4:09:12 PM
* Author: wes
*/


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
	bool getState(bool inputState, unsigned long _minorDebounce, unsigned long _majorDebounceLowThres, unsigned long _majorDebounceHighThres);
	
	protected:

}; //Doppler

#endif //__DOPPLER_H__




// ##### minor debounce
// reads pin, check if it has detected movement
// if yes, set wait for reading the pin again in n ms
// ##### major debounce
// if movement is still detected, start countingMajorDebounce
// inside a window of 10 and 20 seconds, if it detects movement, change state
// if not, reset countingMajorDebounce to last movement detected prior to window