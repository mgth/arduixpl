/*  ArduixPL - xPL for arduino  Copyright (c) 2012 Mathieu GRENET.  All right reserved.  This file is part of ArduixPL.    ArduixPL is free software: you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation, either version 3 of the License, or    (at your option) any later version.    ArduixPL is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.	  Modified Dec 23, 2012 by Mathieu GRENET*/#ifndef XPL_SENSOR_H#define XPL_SENSOR_H#include "utility/xPL_Schema.h"#include "utility/xPL_Device.h"#include "utility/VString.h"#include "utility/xPL_Numeric.h"/*****************************************************************************xPL_Sensor*****************************************************************************/class xPL_Sensor:public xPL_Device {private:public:	virtual const __FlashStringHelper* className() const { return S(sensor); }		xPL_Sensor(const VString& name):xPL_Device(name) {  }	virtual size_t printCurrentTo(Print& p) const=0;	virtual void parseMessage(xPL_MessageIn& msg);	void trig();};class xPL_Sensor_Message : public xPL_Message {	enum {none, current,name} _request;public:	xPL_Sensor_Message(xPL_Sensor& sensor,const VString& request);	size_t printContentTo(Print& p) const;};class xPL_Sensor_MessageTrig : public xPL_Sensor_Message {public:	xPL_Sensor_MessageTrig(xPL_Sensor& sensor):xPL_Sensor_Message(sensor,S(current)) {};	const __FlashStringHelper* msgType() const { return S(trig); }};/********************************************************************Sensor Generic (float)********************************************************************/#include <float.h>class xPL_SensorGeneric:public xPL_Sensor {protected:	mutable float _publicValue;	float _currentValue;	int _count;	int _maxCount;#ifdef XPL_SENSOR_EXTRA	float  _highestValue;	float  _lowestValue;#endif	float _threshold;	mutable struct {		bool reset :1;	} _status;protected:	xPL_SensorGeneric(const VString& name=VString());	virtual const __FlashStringHelper* className() const { return S(generic); }	virtual const __FlashStringHelper* units() const { return F(""); }	virtual size_t printCurrentTo(Print& p) const;public:	void setValue(float v);	void setThreshold(float t) { _threshold=t; }	void setBuffer(int size) {		_maxCount = size;		#ifdef XPL_SENSOR_EXTRA		_buffer.setSize(size);		#endif	}};class xPL_SensorTemp:public xPL_SensorGeneric {
public:	virtual const __FlashStringHelper* className() const { return S(temp); }	virtual const __FlashStringHelper* units() const { return F("c"); }};class xPL_SensorHumidity:public xPL_SensorGeneric {
public:	virtual const __FlashStringHelper* className() const { return S(humidity); }	virtual const __FlashStringHelper* units() const { return F("%"); }};
class xPL_SensorPressure:public xPL_SensorGeneric {
public:	virtual const __FlashStringHelper* className() const { return S(pressure); }	virtual const __FlashStringHelper* units() const { return F("Pa"); }};class xPL_SensorDistance:public xPL_SensorGeneric {
public:	virtual const __FlashStringHelper* className() const { return S(distance); }	virtual const __FlashStringHelper* units() const { return F("m"); }};class xPL_SchemaSensor:public xPL_Schema {public:	virtual const __FlashStringHelper* className() const { return S(sensor); }	xPL_Sensor* add(xPL_Sensor* node) { return (xPL_Sensor*)addChild(node); }	virtual void parseMessage(xPL_MessageIn& msg);};class xPL_SensorDimmer : public xPL_Sensor {

	xPL_Float _value;
	xPL_Float _tmpValue;
	bool _power;

	virtual const __FlashStringHelper* className() const { return F("dimmer"); }	virtual size_t printCurrentTo(Print& p) const;public:	xPL_SensorDimmer(const VString& name):xPL_Sensor(name),_value(0.0),_tmpValue(0.0),_power(false) { }	float setValue(float v);

	float setTmpValue(float v);

	float move(float v);

	void validate() { setValue(_tmpValue); }
	void cancel() { setTmpValue(_value); }

	bool power() { return _power; }
	bool power(bool p);
	bool toggle();
};extern xPL_SchemaSensor xplSensor;#endif