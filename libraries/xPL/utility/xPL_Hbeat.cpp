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

	  Modified 27 Mars 2012 by Mathieu GRENET
*/

#include <xPL.h>
#include "xPL_Hbeat.h"
#include "xPL_Numeric.h"

xPL_Hbeat::xPL_Hbeat(uint16_t interval):xPL_Schema() {
	_interval = interval;
	xPL.trigHbeat();
}

xPL_Hbeat::xPL_Hbeat() {
	xPL_Hbeat( XPL_HBEAT_INTERVAL );
}

void xPL_Hbeat::sendHbeat(const prog_char* type,bool configured,const xPL_String* id) {

	xPL_Message msg(S(stat),(configured)?S(hbeat):className(),type);

	_lastHbeatTime = millis();

	if (id) msg.source.instance = *id;

	if(type != S(end)) msgAddConfigCurrent(msg);

	msg.send();
	
}



bool xPL_Hbeat::msgAddConfigList(xPL_Message& msg) {
	  msg.addOptionKey( S(interval) );
	  return false;
};

bool xPL_Hbeat::msgAddConfigCurrent(xPL_Message& msg) {
	msg.addKey(
			S(interval),
			new xPL_String((int)intervalMinutes()),
			true
		);

	return false;
};

bool xPL_Hbeat::configure(xPL_Key& key)
{
	if (key == S(interval))
	{
		//uint16_t val = (uint16_t) ((key.sValue().toInt()));

		setIntervalMinutes( xPL_Int(key.sValue()) );
		return true;
	}
	return xPL_Schema::configure(key);
}

bool xPL_Hbeat::loop() {

	// send config.end/hbeat.end message when renaming instance
	if (xPL.oldId())
	{
		sendHbeat(S(end),xPL.oldConfigured(),&xPL.oldId());
		xPL.oldId().clear();
	}

	// if it's time to or trigged, send heartbeat
	if ( xPL.trigHbeat(false) || (millis()-_lastHbeatTime)>=interval() ) {
		sendHbeat(S(basic),xPL.configured());
	}
	return false;
}

/*****************************************
EEPROM
*****************************************/
bool xPL_Hbeat::loadDefaultConfig()
{
	_interval = XPL_HBEAT_INTERVAL;
	xPL.trigHbeat();
	return false;
}

bool xPL_Hbeat::loadConfig(xPL_Eeprom& eeprom)
{
	eeprom.readAny(_interval);
	xPL.trigHbeat();
	return false;
}

bool xPL_Hbeat::storeConfig(xPL_Eeprom& eeprom)
{
	eeprom.writeAny(_interval);
	return false;
}
