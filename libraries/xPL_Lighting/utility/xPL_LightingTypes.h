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

	  Modified 20 Mars 2012 by Mathieu GRENET
*/

#ifndef XPL_LIGHTINGTYPES_H
#define XPL_LIGHTINGTYPES_H

#include <xPL.h>
#include "utility/xPL_Eeprom.h"

#include "utility/xPL_Numeric.h"

#define XPL_MAX_LEVEL 100

#define XPL_DEFAULT 0x01
#define XPL_LAST    0x02


typedef unsigned long time_t ;
static const unsigned long maxLevel = 0xFFFF;

class xPL_LightingFaderate: public xPL_Numeric
{
	time_t _fadetime;
protected:
	void store(uint16_t val, uint8_t dec);
	uint16_t load(uint8_t dec) const;

public:
	xPL_LightingFaderate(uint16_t fade, uint8_t dec=0) { store(fade,dec); }

	time_t toTime() const { return _fadetime; }

	void fromEeprom(xPL_Eeprom& eeprom) {  eeprom.readAny(_fadetime); }
	void toEeprom(xPL_Eeprom& eeprom) { eeprom.writeAny(_fadetime); }

	size_t printTo(Print& p) const { return printDecTo(3,p); }

	void operator+=(xPL_LightingFaderate faderate) { _fadetime += faderate.toTime(); }

	xPL_LightingFaderate operator+(const xPL_LightingFaderate& faderate) const {
		xPL_LightingFaderate result = *this;
		result+=faderate;
		return result;
	}

	xPL_LightingFaderate(){ _fadetime=0; }
};

class xPL_LightingLevel:public xPL_Numeric
{

private:
	uint16_t _level;
	xPL_LightingFaderate _faderate;

protected:
	virtual uint16_t value() const { return _level; }

	void store(uint16_t val, uint8_t dec);
	uint16_t load(uint8_t dec=0) const;

public:
	xPL_LightingFaderate& faderate() { return _faderate; }

	void fromEeprom(xPL_Eeprom& eeprom);
	void toEeprom(xPL_Eeprom& eeprom);

	uint16_t to16bits() { return _level; }
	uint16_t to10bits() { return ((uint16_t)_level)>>6; }
	uint8_t to8bits() { return (uint8_t)(((uint16_t)_level)>>8); }
	uint8_t toBit() { if (_level) return HIGH; return LOW; }

	void operator=(uint16_t val) { _level=val; }
	void operator=(const xPL_LightingLevel& level) { _level=level.value(); _faderate=level._faderate; }

	xPL_LightingLevel percent(xPL_LightingLevel l)
	{
		xPL_LightingLevel out;
		out._level = ((uint32_t)_level * (uint32_t)l._level) / maxLevel;
		out.faderate() = faderate() + l.faderate();
		return out;
	}


	size_t printTo(Print& p) const {
		return printDecTo(2,p);
	}

	bool operator==(xPL_LightingLevel level) { return _level==level._level; }

	xPL_LightingLevel(const xPL_LightingLevel& level):xPL_Numeric() { *this=level; }
	xPL_LightingLevel(){ _level=0; }
};

typedef byte pin_t;
typedef xPL_LightingLevel level_t;
typedef xPL_LightingFaderate faderate_t;



#endif
