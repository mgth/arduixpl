// xPL_Sensor_Dallas.h

#ifndef _XPL_SENSORULTRASONIC_H
#define _XPL_SENSORULTRASONIC_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "../xPL_Sensor/xPL_Sensor.h"
	virtual const __FlashStringHelper* className() const { return S(distance); }
