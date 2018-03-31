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

	//functions
	public:
	FanChannel();
	FanChannel(int _ctrlPin, int _fanDelay):controlPin(_ctrlPin), fanDelay(_fanDelay){};
	
	bool inflateAndHold(int _period, int _inflateValue, int _holdValue);
	void breathe
	(
	int		_periodo,
	int		_inflateVal,		int			_holdVal,		int		 _deflateVal,		int _restartVal,
	float	_inflateTime,		float		_holdTime,		float	_deflateTime,
	);
	//void breatheErratically();
	
	private:
	//FanChannel( const FanChannel &c );
	//FanChannel& operator=( const FanChannel &c );

}; //FanChannel

#endif //__FANCHANNEL_H__




