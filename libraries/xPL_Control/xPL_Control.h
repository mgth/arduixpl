/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.

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

	  Modified Jan 20, 2013 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#ifndef xPL_SchemaControl_h
#define xPL_SchemaControl_h
#include "utility/xPL_Define.h"
#include "utility/xPL_Schema.h"
#include "utility/xPL_Device.h"

class xPL_Control : public xPL_Device {	virtual const __FlashStringHelper* className() const { return S(sensor); }	public:	xPL_Control(const VString& name):xPL_Device(name) {  }
};

class xPL_ControlOutput : public xPL_Control {
	void parseMessage(xPL_MessageIn& msg);
	virtual void setValue(bool v) = 0;
	virtual bool getValue() = 0;
public:
	xPL_ControlOutput(const VString& name):xPL_Control(name) {  }};

class xPL_ControlVariable : public xPL_Control {
	void parseMessage(xPL_MessageIn& msg);
	virtual void setValue(float v) = 0;
	virtual float getValue() = 0;
public:	xPL_ControlVariable(const VString& name):xPL_Control(name) {  }};

class xPL_ControlPin : public xPL_ControlOutput {
	byte _pin;
	bool _v;
public:	xPL_ControlPin(const VString& name, byte pin):xPL_ControlOutput(name),_pin(pin) {  }	
	void setValue(bool v);
	bool getValue() {return _v; };
};

class xPL_ControlPWM : public xPL_ControlVariable {
	byte _pin;
	float _v;
public:	xPL_ControlPWM(const VString& name, byte pin):xPL_ControlVariable(name),_pin(pin) {  }	virtual void setValue(float v)
	{
		_v = v;
		pinMode(_pin,OUTPUT);
		digitalWrite(_pin,v);
	}
	virtual float getValue() {return _v; };
};

class xPL_SchemaControl : public xPL_Schema {

protected:
	virtual const __FlashStringHelper* className() const { return S(control); }

//	virtual void configure(xPL_Key& key);

};

extern xPL_SchemaControl xplControl;

#endif