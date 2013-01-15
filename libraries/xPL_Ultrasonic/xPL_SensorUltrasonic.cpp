// 
// 
// 

#include "xPL_SensorUltrasonic.h"
#include "pins_arduino.h"

xPL_SensorUltrasonic::xPL_SensorUltrasonic(uint8_t pinTrig, uint8_t pinEcho, const VString & name){	_pinTrig=pinTrig;	_pinEcho=pinEcho;	setId(name);	setTemp(18.0);//	setBuffer(10);	//setThreshold(0.01);	setThreshold(0.01);	_timeB=micros();}size_t xPL_SensorUltrasonic::printCurrentTo(Print& p) const{	size_t len=0;	len += xPL_SensorGeneric::printCurrentTo(p);/*	len += xPL_Message::printKeyTo(p,F("A"),int(_timeA-_time));	len += xPL_Message::printKeyTo(p,F("B"),int(_timeB-_timeA));	len += xPL_Message::printKeyTo(p,F("C"),int(_timeC-_timeB));	len += xPL_Message::printKeyTo(p,F("D"),int(_timeD-_timeC));*/	return len;}void xPL_SensorUltrasonic::loop(){	if (micros()-_timeB<15000) return;	uint8_t bit = digitalPinToBitMask(_pinEcho);
	uint8_t port = digitalPinToPort(_pinEcho);
	uint8_t stateMask = (HIGH ? bit : 0);
	pinMode(_pinEcho, INPUT);	if(digitalRead(_pinEcho)) return;	//	detachInterrupt(_pinEcho);	pinMode(_pinTrig, OUTPUT);
	digitalWrite(_pinTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(_pinTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pinTrig, LOW);	pinMode(_pinEcho, INPUT);/*	_time = micros();	while(!digitalRead(_pinEcho)) {		if (micros()-_time>_timeOut)			return false;	}	_time = micros();	_count=0;	bool async = false; //xPL.attachInterrupt(_pinEcho,FALLING );	if (!async)	{		cli();		unsigned long pulse = pulseIn(_pinEcho,HIGH,_timeOut);		_distance = pulse / _speedRatio;		sei();	}*/	//	cli();	_time = micros();	while ((*portInputRegister(port) & bit) == stateMask);	while ((*portInputRegister(port) & bit) != stateMask) {		if (micros()-_time>500)		{			_distance=-1.0;			return;		}	}//	sei();	_time = micros();	while ((*portInputRegister(port) & bit) == stateMask);	_distance = (micros()-_time) / _speedRatio;		setValue(_distance);}void xPL_SensorUltrasonic::interrupt(uint8_t pin, unsigned long time){	if(pin==_pinEcho) 	{		float duration = time - _time;		if (duration<_timeOut)		{			_distance = duration / _speedRatio;			_count++;		}	}}