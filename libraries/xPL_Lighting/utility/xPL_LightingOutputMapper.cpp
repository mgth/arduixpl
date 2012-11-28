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

	  Modified 22 Mars 2012 by Mathieu GRENET
*/
#include <Arduino.h>
#include "xPL_LightingOutputMapper.h"

bool xPL_LightingOutputMapper_Debug::setLevel(uint16_t out,xPL_LightingLevel value)
{
	uint8_t outValue=value.to8bits();
	if (_values[out]!=outValue)
	{
		_values[out]=outValue;

#ifdef XPL_DEBUG
		xPL_Printable::print_prog(Serial,CStr::out(),':');
		Serial.print(out);
		Serial.print('=');
		Serial.println(outValue);
#endif
		return true;
	}
	return false;
}
