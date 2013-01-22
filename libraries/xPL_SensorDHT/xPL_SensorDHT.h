// xPL_Sensor_Dallas.h

#ifndef _XPL_SENSORDHT11_h
#define _XPL_SENSORDHT11_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "utility/DHT.h"#include "../xPL_Sensor/xPL_Sensor.h"#include "../xPL/utility/VString.h"class xPL_SensorDHT:public xPL_NodeParent {
dht _dht;int _pin;unsigned long _time; byte _type;xPL_SensorTemp _temp;xPL_SensorHumidity _humidity;public:	xPL_SensorDHT(uint8_t pin, byte type, const VString & tName, const VString & hName);	void loop();};
#endif

