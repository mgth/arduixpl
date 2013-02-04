/*
  ArduixPL - xPL library for Arduino(tm)
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

	  Modified 2013-2-4 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#include "xPL_Sensor.h"
size_t xPL_SensorDimmer::printCurrentTo(Print& p) const{	size_t l=0;	l += xPL_Message::printKeyTo(p,S(current),_tmpValue);	l += xPL_Message::printKeyTo(p,S(power),(_power)?S(on):S(off));	l += xPL_Message::printKeyTo(p,S(units),F("%"));	return l;}float xPL_SensorDimmer::setValue(float v) {	setTmpValue(v);
	_value=_tmpValue;
	return _value;
}

float xPL_SensorDimmer::setTmpValue(float v) {	if (v<0.0) v=0.0;
	if (v>100.0) v=100.0;
	if (v!=_tmpValue)
	{
		_tmpValue = v;
		trig();
	}
	return _tmpValue;
}

float xPL_SensorDimmer::move(float v) {
	return setTmpValue(_tmpValue + v);
}

bool xPL_SensorDimmer::power(bool p)
{
	if (p!=_power) { _power=p; trig(); return !p; }
	return p;
}

bool xPL_SensorDimmer::toggle()
{
	_power = !_power;
	trig();
	return (_power);
}
