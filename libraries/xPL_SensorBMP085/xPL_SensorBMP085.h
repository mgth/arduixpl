// xPL_Sensor_Dallas.h

#ifndef _XPL_SENSORBMP085_h
#define _XPL_SENSORMPM085_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "utility/BMP085.h"#include "../xPL_Sensor/xPL_Sensor.h"#include "../xPL/utility/VString.h"class xPL_SensorBMP085:public xPL_NodeParent {
BMP085 _bmp085;int _pin;unsigned long _time; byte _type;xPL_SensorTemp _temp;xPL_SensorPressure _pressure;public:	xPL_SensorBMP085(const VString & tName, const VString & hName);	virtual bool parseMessage(xPL_MessageIn& msg) { return true; }	virtual bool loop();};
#endif

