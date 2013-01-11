// 
// 
// 

#include "xPL_SensorBMP085.h"

xPL_SensorBMP085::xPL_SensorBMP085(const VString & tName, const VString & pName){	_temp.setId(tName);	_temp.setBuffer(10);	_pressure.setId(pName);	_pressure.setBuffer(10);	_pressure.setThreshold(0.25);	addChild(&_temp);	addChild(&_pressure);	//_time = millis()-2000;	_bmp085.begin(); // TODO : set mode configurable}bool xPL_SensorBMP085::loop(){			//if (millis()-_time<2000) return false;	_time = millis();	_temp.setValue(_bmp085.readTemperature());	float pressure=_bmp085.readPressure();	_pressure.setValue(pressure/100.0);	return false;}