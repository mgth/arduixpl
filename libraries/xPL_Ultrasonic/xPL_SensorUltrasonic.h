// xPL_Sensor_Dallas.h

#ifndef _XPL_SENSORULTRASONIC_H
#define _XPL_SENSORULTRASONIC_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "../xPL_Sensor/xPL_Sensor.h"#include "../xPL/utility/VString.h"class xPL_SensorUltrasonic:public xPL_SensorGeneric {
	virtual const __FlashStringHelper* className() const { return S(distance); }	virtual const __FlashStringHelper* units() const { return F("m"); }byte _pinTrig;volatile bool _waiting;volatile byte _pinEcho;volatile float _distance;volatile unsigned long _time; volatile float _speedRatio;	unsigned long _timeA;	unsigned long _timeB;	unsigned long _timeC;	unsigned long _timeD;volatile int _count;static const unsigned long _timeOut=200000;public:	xPL_SensorUltrasonic(uint8_t pinTrig, uint8_t pinEcho, const VString& name);	void setTemp(float temp) { _speedRatio=2000000.0 / (331.5 + 0.607*temp); }	virtual void loop();	virtual void interrupt(uint8_t pin, unsigned long time);	virtual size_t printCurrentTo(Print& p) const;};#endif

