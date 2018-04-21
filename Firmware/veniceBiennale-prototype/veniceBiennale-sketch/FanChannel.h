/*
* FanChannel.h
*
* Created: 3/23/2018 11:25:53 PM
* Author: wes
*/

#include <Arduino.h>

#ifndef __FANCHANNEL_H__
#define __FANCHANNEL_H__


class FanChannel
{
	//variables
	public:
	private:
	const int controlPin;
	const int fanDelay;
	
	//bool inflating = false;
	unsigned long inflateStartedMillis;
	int inflateAndHoldState = 0;
	
	int breathingState = 0;
	unsigned long breathStart;
	
	unsigned int breathCount = 0;


	bool deflating = false;
	int deflateAndHoldState = 0;
	unsigned long deflateStartedMillis = 0;
	


	//breathe2
	
	unsigned int rampState = 0;
	unsigned long breathe2Start = 0;
	unsigned int breatheVal = 0;
	//end of breathe2 vars


	//functions
	public:
	FanChannel();
	FanChannel(int _ctrlPin, int _fanDelay);//:controlPin(_ctrlPin), fanDelay(_fanDelay)
	
	bool inflateAndHold(int _period, int _inflateValue, int _holdValue);
	
	
	void deflate(unsigned int _deflateVal, unsigned int _holdDeflateVal);
	
	void setDeflateState(bool _deflateState);
	
	
	void breathe(
	int _periodo,
	int _inflateVal, int _holdVal, int _deflateVal, int _restartVal,
	float _inflateTime, float _holdTime, float _deflateTime);
	
	void breathe2(unsigned int rampUpStep, unsigned int rampDownStep, unsigned int highWait, unsigned int lowWait, unsigned int highVal, unsigned int lowVal);
	
	
	unsigned int getBreathCount();
	void setBreathCount(int _breathCount);
	//void breatheErratically();
	
	private:
	//FanChannel( const FanChannel &c );
	//FanChannel& operator=( const FanChannel &c );

}; //FanChannel

#endif //__FANCHANNEL_H__




