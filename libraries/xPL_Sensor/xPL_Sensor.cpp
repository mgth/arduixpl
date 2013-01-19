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
#include "xPL_Sensor.h"

xPL_SchemaSensor xplSensor;


xPL_Sensor_Message::xPL_Sensor_Message(xPL_Sensor& sensor,const VString& request):xPL_Message(sensor){	if (request==S(current)) _request=current;	else if (request==S(name)) _request=name;	else _request=none;}size_t xPL_Sensor_Message::printContentTo(Print& p) const {	size_t l = 0;	if (_node)	{		l += printKeyTo(p,S(device),((xPL_Sensor*)_node)->id);		l += printKeyTo(p,S(type),_node->className());		switch(_request) {		case current:			l += ((xPL_Sensor*)_node)->printCurrentTo(p);			break;		case name:			l += printKeyTo(p,S(name),((xPL_Sensor*)_node)->id);			break;		default:			break;		}	}	return l;}xPL_SensorGeneric::xPL_SensorGeneric(const VString& name):xPL_Sensor(name) {	_count = 0;	_maxCount = 1;	_threshold=0.1;	_status.reset=true;}size_t xPL_SensorGeneric::printCurrentTo(Print& p) const{	size_t l=0;	l += xPL_Message::printKeyTo(p,S(current),_currentValue);#ifdef XPL_SENSOR_EXTRA	l += xPL_Message::printKeyTo(p,S(lowest),_lowestValue);	l += xPL_Message::printKeyTo(p,S(highest),_highestValue);#endif	l += xPL_Message::printKeyTo(p,S(units),units()); 	_publicValue=_currentValue;	return l;}/*void xPL_SensorGeneric::setValue(float v) {	_currentValue=v;	if (_status.reset || _highestValue<v) _highestValue=v;	if ( _status.reset || _lowestValue>v) _lowestValue=v;	_status.reset=false;	if (abs((float)_currentValue-(float)_publicValue)>=_threshold )	{		xPL_Sensor_MessageTrig(*this).send();
	}}*/void xPL_SensorGeneric::setValue(float v) {#ifdef XPL_DEBUG_LCD	lcd.saveCursor();	lcd.setCursor(0,2); lcd.print(v,2);	lcd.restoreCursor();#endif	if (_count==0) _currentValue=v;	else _currentValue+=v;	if (_count<_maxCount)	{		_count++;		return;	}		_currentValue/=_count;	_count = 0;#ifdef XPL_DEBUG_LCD	lcd.saveCursor();	lcd.setCursor(10,2); lcd.print(_currentValue,2);	lcd.restoreCursor();#endif#ifdef XPL_SENSOR_EXTRA	if (_status.reset || _highestValue <_currentValue) _highestValue=_currentValue;	if ( _status.reset || _lowestValue >_currentValue) _lowestValue=_currentValue;#endif	if (_status.reset || abs(_currentValue-_publicValue)>=_threshold )	{		xPL_Sensor_MessageTrig(*this).send();
	}	_status.reset=false;}

void xPL_Sensor::parseMessage(xPL_MessageIn& msg) {	if ( msg.schema.instance == S(request) )	{		VString req = msg.key_request();		// Todo : flush memory		xPL_Sensor_Message(*this,req).send();
	}}void xPL_Sensor::trig(){	xPL_Sensor_MessageTrig(*this).send();}void xPL_SchemaSensor::parseMessage(xPL_MessageIn& msg){	return sendParseMessage(msg);}