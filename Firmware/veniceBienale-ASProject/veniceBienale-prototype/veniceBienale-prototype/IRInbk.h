/*
* IRIn.h
*
* Created: 3/24/2018 3:17:11 PM
* Author: wes
*/


#ifndef __IRIN_H__
#define __IRIN_H__


class IRIn
{
	//variables
	public:
	protected:
	private:
		int inputPin;
		
		const int sampleCount = 100;
		int sampleBeingUpdated = 0;
		int readings[100];
		
		long readingsSum;
		int readingsAvg;
		
		int triggerVal;
		bool triggerBool = true;
		
		bool newTrigger = true;
		long debounceTime;
		int debounceLength = 100;

	//functions
	public:
		IRIn();
		IRIn(int _inputPin);
		IRIn(int _inputPin, int _delay);
		void fillArray(int _delay);
		void addReading();
		int getAverage();
		void setTriggerval(int _triggerVal);
		void setTriggerBool(bool _triggerBool);
		void setDebounceLength(int _debounceLength);
		bool debouncedInput();
	//~IRIn();
	protected:
	private:
	//IRIn( const IRIn &c );
	//IRIn& operator=( const IRIn &c );

}; //IRIn

#endif //__IRIN_H__
