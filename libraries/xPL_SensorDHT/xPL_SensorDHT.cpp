// 
// 
// 

#include "xPL_SensorDHT.h"

xPL_SensorDHT::xPL_SensorDHT(uint8_t pin,byte type, const VString & tName, const VString & hName){	_pin=pin;	_type = type;	_temp.setId(tName);	_temp.setBuffer(10);	_humidity.setId(hName);	_humidity.setBuffer(10);	_humidity.setThreshold(0.25);	addChild(&_temp);	addChild(&_humidity);	_time = millis()-2000;}bool xPL_SensorDHT::loop(){			if (_type==22)	{		if (millis()-_time<2000) return false;	} else if (millis()-_time<1000) return false;	_time = millis();	int res = (_type==22)?_dht.read22(_pin):_dht.read11(_pin);	if (res==DHTLIB_OK)	{		_temp.setValue(_dht.temperature);		_humidity.setValue(_dht.humidity);	}		return false;}