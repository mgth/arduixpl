/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of ArduixPL.

    ArduixPL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduixPL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.

	  Modified Dec 23, 2012 by Mathieu GRENET
*/
#ifndef XPL_SENSOR_H#define XPL_SENSOR_H#include "utility/xPL_Schema.h"#include "utility/VString.h"#include "utility/xPL_Numeric.h"//#include "utility/xPL_Message.h"class xPL_Sensor:public xPL_Node {protected:	VString _id;public:	xPL_Sensor(const VString& name):_id(name) { /*_id.load();*/ }	virtual const __FlashStringHelper* className() const { return S(sensor); }	
	virtual VString* id() { return &_id; }	virtual bool targeted(xPL_MessageIn& msg);	virtual size_t printCurrent(Print& p) const=0;	virtual bool parseMessage(xPL_MessageIn& msg);};class xPL_Sensor_Message : public xPL_Message {	xPL_Sensor* _sensor;	enum {none, current,name} _request;public:	xPL_Sensor_Message(xPL_Sensor& sensor,const __FlashStringHelper* msgType,const VString& request);	size_t printContentTo(Print& p) const;};class xPL_SensorTemp:public xPL_Sensor {private:	mutable xPL_Float _publicTemp;	xPL_Float _temp;	xPL_Float  _highestTemp;	xPL_Float  _lowestTemp;	float _threshold;	bool _reset;protected:	xPL_SensorTemp(const VString& name);	virtual const __FlashStringHelper* className() const { return S(temp); }	virtual size_t printCurrent(Print& p) const;public:	void setTemp(float t);};#include <OneWire.h>#include <DallasTemperature.h>class xPL_SensorTempOneWire:public xPL_SensorTemp {DallasTemperature* _sensors;DeviceAddress _addr;#define _wait 750#define _resolution 12unsigned long lastrequest;public:	xPL_SensorTempOneWire(DallasTemperature& sensors, DeviceAddress addr, const VString & name);	virtual bool loop();};class xPL_SchemaSensor:public xPL_Schema {public:	virtual const __FlashStringHelper* className() const { return S(sensor); }	xPL_Sensor* add(xPL_Sensor* node) { return (xPL_Sensor*)addChild(node); }	virtual bool parseMessage(xPL_MessageIn& msg);	void discoverOneWireTemp(DallasTemperature& sensors);};extern xPL_SchemaSensor xplSensor;#endif