/*  ArduixPL - xPL for arduino  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.  This file is part of ArduixPL.    ArduixPL is free software: you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation, either version 3 of the License, or    (at your option) any later version.    ArduixPL is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.	  Modified Jan 20, 2013 by Mathieu GRENET 	  mailto:mathieu@mgth.fr	  http://www.mgth.fr*/#include "xPL_Control.h"

xPL_SchemaControl xplControl;
void xPL_ControlOutput::parseMessage(xPL_MessageIn& msg) {	if ( msg.schema.instance == S(basic) )	{		VString val = msg.key_current();		if (val==S(enable) || val==S(high)) setValue(true);		else if (val==S(disable) || val==S(low)) setValue(false);		else if (val==S(toggle))		{			setValue(!getValue());		}		DBG(F("current="),getValue()?S(on):S(off));	}}void xPL_ControlVariable::parseMessage(xPL_MessageIn& msg) {	if ( msg.schema.instance == S(basic) )	{		VString val = msg.key_current();		VString data1 = msg.key_data1();		if      (val==S(inc)) setValue(getValue()+xPL_Float(data1));		else if (val==S(dec)) setValue(getValue()-xPL_Float(data1));		else if (val==S(set)) setValue(xPL_Float(data1));		else		{			setValue(xPL_Float(val));		}	}}void xPL_ControlPin::setValue(bool v)
{
	if (v!=_v)
	{
		_v = v;
		pinMode(_pin,OUTPUT);
		digitalWrite(_pin,v);
		DBG(F("pin:"),_pin);
		DBG(F("val:"),v);
	}
}
