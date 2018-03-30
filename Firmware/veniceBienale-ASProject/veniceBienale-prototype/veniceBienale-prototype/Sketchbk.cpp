#include <Arduino.h>
#include "FanChannel.h"


/*######################################################################

Pin Mapping on PCB

*/

#define LED1 8

#define FAN_PIN1 5		//connector by power input
#define FAN_PIN2 10		//connector in the center
#define FAN_PIN3 6		//connector far from power input

#define MUX_CONTROL 2	//ctrl pin for Analog Mux Switches (ABC)
#define MUX_CONTROL_DEFAULT LOW

#define IR_PIN1 A5	//
#define IR_PIN2 A4	//
//######################################################################



class IRIn{
	public:
	IRIn(int _inputPin)
	{
		inputPin = _inputPin;
		pinMode(inputPin, INPUT);
		this->fillArray(0);
	}
	
	IRIn(int _inputPin, int _delay)
	{
		this->fillArray(_delay);
	}
	
	void fillArray(int _delay)
	{
		for (int i = 0; i < sampleCount; i++)
		{
			readings[i] = analogRead(inputPin);
			if(delay!=0) delay(_delay);
		}
	}
	
	void addReading()
	{
		readings[sampleBeingUpdated] = analogRead(inputPin);
		sampleBeingUpdated++;
		if(sampleBeingUpdated >= sampleCount) sampleBeingUpdated = 0;
	}
	
	int getAverage()
	{
		readingsSum = 0;
		for (int i = 0; i < sampleCount; i++)
		{
			readingsSum += readings[i];
		}
		readingsAvg = int(readingsSum/sampleCount);
		
		return readingsAvg;
	}
	
	void setTriggerVal(int _triggerVal)
	{
		triggerVal = _triggerVal;
	}
	
	void setTriggerBool(bool _triggerBool)
	{
		triggerBool = _triggerBool;
	}
	
	void setDebounceLength(int _debounceLength)
	{
		debounceLength = _debounceLength;
	}
	
	bool debouncedInput(){
		bool _debouncedInput = false;
		if ((getAverage() > triggerVal)==triggerBool && newTrigger)
		{
			debounceTime = millis();
			newTrigger = false;
		}
		
		if((getAverage() > triggerVal)==triggerBool && (millis() - debounceTime > debounceLength)){
			_debouncedInput = true;
		}
		
		if((getAverage() <= triggerVal)==triggerBool){
			newTrigger = true;
			_debouncedInput = false;
		}
		return _debouncedInput;
	}
	
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
};

boolean fan1inf, fan2inf, fan3inf;

void setup() {
	pinMode(LED1, OUTPUT);
	
	pinMode(MUX_CONTROL,OUTPUT);
	digitalWrite(MUX_CONTROL,MUX_CONTROL_DEFAULT);
	
	IRIn dist1(IR_PIN1,10);
	IRIn dist2(IR_PIN2,10);

	FanChannel fan1(FAN_PIN1);
	FanChannel fan2(FAN_PIN2);
	FanChannel fan3(FAN_PIN3);
	
	for(int i = 0; i < 20; i ++){
		digitalWrite(LED1, 255);
		delay(100);
		digitalWrite(LED1, 0);
		delay(100);
	}

	fan1inf = true;
	fan2inf = true;
	fan3inf = true;
	
	while(fan1inf || (fan3inf) || fan2inf){
		if(fan1inf)
		{
			fan1inf = fan1.inflateAndHold(2000, 255, 10);
		}
		if(fan2inf)
		{
			fan2inf = fan2.inflateAndHold(5000, 255, 100);
		}
		if(fan3inf)
		{
			fan3inf = fan3.inflateAndHold(3000, 255, 50);
		}
	}

 }


int bla= 0;

void loop() {
	bla = dist1.getAverage();
	digitalWrite(8, HIGH);
	delay(100);
	digitalWrite(8,LOW);
	delay(90);
}
