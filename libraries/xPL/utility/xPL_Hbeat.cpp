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

#include <xPL.h>
#include "xPL_Hbeat.h"
#include "xPL_Numeric.h"

xPL_Hbeat::xPL_Hbeat(uint16_t interval):xPL_Schema() {
	_interval = interval;
	_trigHbeat = true;
}

xPL_Hbeat::xPL_Hbeat() {
	xPL_Hbeat( XPL_HBEAT_INTERVAL );
}

void xPL_Hbeat::sendHbeat(const __FlashStringHelper* type) {

	_lastHbeatTime = millis();
	_trigHbeat = false;

	xPL_MessageHbeat(*this,type).send();
}


size_t xPL_MessageHbeat::printContentTo(Print& p) const {	if (_schType==S(end)) return 0;	else return ((xPL_Hbeat*)_node)->printConfig(p);}

void xPL_Hbeat::loop() 
{

	// if it's time to or trigged, send heartbeat
	if ( _trigHbeat || (millis()-_lastHbeatTime)>=interval() ) {
		sendHbeat();
	}
}

void xPL_Hbeat::parseMessage(xPL_MessageIn& msg)
{
	if ( msg.schema.instance == S(request) )
	{
		if (msg.key_command() == S(request))
		{
			_trigHbeat=true;
		}
	}
}

size_t xPL_Hbeat::printConfig(Print& p,bool list)
{		
	if(list)
		return xPL_Message::printOptionKeyTo(p,S(interval) );
	else
		return xPL_Message::printKeyTo(p,S(interval),(int)intervalMinutes() );
}

void xPL_Hbeat::configure(xPL_Key& key)
{
	if (key.id == S(interval))
	{
		setIntervalMinutes( xPL_Int(key.value) );
	}
}

void xPL_Hbeat::config(xPL_Eeprom& eeprom,bool store)
{		
	if (store)
		eeprom.writeAny(_interval);
	else
	{
		eeprom.readAny(_interval);
		_trigHbeat = true;
	}
}

void xPL_Hbeat::defaultConfig()
{
	_interval = XPL_HBEAT_INTERVAL;
	_trigHbeat = true;
}
