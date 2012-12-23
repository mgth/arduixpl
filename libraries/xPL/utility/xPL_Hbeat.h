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

#ifndef xPL_HBEAT_H
#define xPL_HBEAT_H

#include <xPL.h>

#include "xPL_Schema.h"


class xPL_Hbeat : public xPL_Schema {
private:

protected:
  unsigned long _lastHbeatTime;
  uint16_t _interval;

	virtual bool loop();

	virtual size_t printConfigList(Print& p);
	virtual bool configure(xPL_Key& key);

public:	
	virtual const __FlashStringHelper* className() const { return S(hbeat); }	


	xPL_Hbeat(uint16_t interval);
    xPL_Hbeat();

	virtual bool loadConfig(xPL_Eeprom& eeprom);
	virtual bool loadDefaultConfig();
	virtual bool storeConfig(xPL_Eeprom& eeprom);
	virtual size_t printConfigCurrent(Print& p);

	void setIntervalMinutes(uint16_t interval){_interval=interval;}

	uint32_t interval() { return ((uint32_t)_interval)*60000; }

	uint16_t intervalMinutes() const { return _interval;} // TODO : should be able to return decimal value ?

	void sendHbeat(const __FlashStringHelper* type,bool configured,const VString* id=NULL);

};

class xPL_Hbeat_Message : public xPL_Message {	xPL_Hbeat* _hbeat;public:	xPL_Hbeat_Message(xPL_Hbeat& hbeat, const __FlashStringHelper* schClass, const __FlashStringHelper* schType):xPL_Message(S(stat),schClass,schType)	{		_hbeat =  &hbeat;	}	size_t printContentTo(Print& p) const;};
#endif