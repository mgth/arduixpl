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

	  Modified 2013-1-22 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#ifndef xPL_HBEAT_H
#define xPL_HBEAT_H

#include "xPL_Schema.h"
#include "xPL_Message.h"

class xPL_Hbeat : public xPL_Schema {

protected:
	const __FlashStringHelper* className() const { return S(hbeat); }	

	void loop();
	void parseMessage(xPL_MessageIn& msg);
	void configure(xPL_Key& key);
	void config(xPL_Eeprom& eeprom,bool store=false);
	void defaultConfig();

	unsigned long _lastHbeatTime;
	uint16_t _interval;
	bool _trigHbeat;
	VString _newconf;

public:	
	size_t printConfig(Print& p,bool list=false);

    xPL_Hbeat();
	xPL_Hbeat(uint16_t interval);

	void setIntervalMinutes(uint16_t interval){_interval=interval;}

	uint32_t interval() { return ((uint32_t)_interval)*60000; }

	uint16_t intervalMinutes() const { return _interval;} // TODO : should be able to return decimal value ?

	void sendHbeat(const __FlashStringHelper* type=S(basic));

	virtual const __FlashStringHelper* hbeatClass() const { return className(); }
};

class xPL_MessageHbeat : public xPL_Message {	const __FlashStringHelper* _schType;	size_t printContentTo(Print& p) const;	const __FlashStringHelper* schType() const { return _schType; }public:	xPL_MessageHbeat(xPL_Hbeat& hbeat,const __FlashStringHelper* schType=S(basic)):xPL_Message(hbeat),_schType(schType) {};};
#endif