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

	xPL_Hbeat_Message(*this).send();
}

size_t xPL_Hbeat_Message::printContentTo(Print& p) const {	return ((xPL_Hbeat*)_node)->printConfigCurrent(p);}size_t xPL_HbeatEnd_Message::printContentTo(Print& p) const { return 0; }

size_t xPL_Hbeat::printConfigList(Print& p) {

	return xPL_Message::printOptionKeyTo(p,S(interval) );
};

size_t xPL_Hbeat::printConfigCurrent(Print& p) {

	return xPL_Message::printKeyTo(p,S(interval),(int)intervalMinutes() );
};

void xPL_Hbeat::configure(xPL_Key& key)
{
	if (key.id == S(interval))
	{
		//uint16_t val = (uint16_t) ((key.sValue().toInt()));

		setIntervalMinutes( xPL_Int(key.value) );
	}
	return xPL_Schema::configure(key);
}

void xPL_Hbeat::loop() {

	// send config.end/hbeat.end message when renaming instance
	if (xPL.oldId())
	{
		sendHbeat(S(end));
		xPL.oldId().clear();
	}

	// if it's time to or trigged, send heartbeat
	if ( _trigHbeat || (millis()-_lastHbeatTime)>=interval() ) {
		sendHbeat(S(basic));
	}
}

void xPL_Hbeat::parseMessage(xPL_MessageIn& msg)
{
	if ( msg.schema.instance == S(request) )
	{
		if (msg.key_command() == S(request))
		{
			trigHbeat();
		}
	}
}

/*****************************************
EEPROM
*****************************************/
void xPL_Hbeat::loadConfig(xPL_Eeprom& eeprom)
{
	if (eeprom.isxPL())
		eeprom.readAny(_interval);
	else
		_interval = XPL_HBEAT_INTERVAL;

	trigHbeat();
}

void xPL_Hbeat::storeConfig(xPL_Eeprom& eeprom)
{
	eeprom.writeAny(_interval);
}
