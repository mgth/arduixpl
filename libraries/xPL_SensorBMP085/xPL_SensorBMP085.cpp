// 
// 
// 

#include "xPL_SensorBMP085.h"

xPL_SensorBMP085::xPL_SensorBMP085(const VString & tName, const VString & pName){	_temp.setId(tName);	_temp.setBuffer(10);	_pressure.setId(pName);	_pressure.setBuffer(10);	_pressure.setThreshold(25);	addChild(&_temp);	addChild(&_pressure);	_bmp085.begin(); // TODO : set mode configurable}void xPL_SensorBMP085::loop(){			if (millis()-_time<2000) return;	_temp.setValue(_bmp085.readTemperature());	_pressure.setValue(_bmp085.readPressure());	_time = millis();}