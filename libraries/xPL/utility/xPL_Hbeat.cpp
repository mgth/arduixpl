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

	  Modified Jan 18, 2013 by Mathieu GRENET 
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

	xPL_Hbeat_Message(*this).send();
}


size_t xPL_Hbeat_Message::printContentTo(Print& p) const {	return ((xPL_Hbeat*)_node)->event(xPL_Event(CFG_CURRENT,&p));}size_t xPL_HbeatEnd_Message::printContentTo(Print& p) const { return 0; }

size_t xPL_Hbeat::event(const xPL_Event& evt)
{
	size_t len=0;
	switch(evt.id())
	{
/***********************************************
  LOOP
************************************************/
		case LOOP:
			if (xPL.oldId())
			{
				sendHbeat(S(end));
				xPL.oldId().clear();
			}

			// if it's time to or trigged, send heartbeat
			if ( _trigHbeat || (millis()-_lastHbeatTime)>=interval() ) {
				sendHbeat(S(basic));
			}
			break;
/***********************************************
  PARSE INCOMING MESSAGES
************************************************/
		case PARSE_MESSAGE:
			if ( evt.messageIn().schema.instance == S(request) )
			{
				if (evt.messageIn().key_command() == S(request))
				{
					_trigHbeat=true;
				}
			}
			break;

/***********************************************
  CONFIG
************************************************/
		case CFG_LIST:
			len += xPL_Message::printOptionKeyTo(evt.print(),S(interval) );
			break;
		case CFG_CURRENT:
			len += xPL_Message::printKeyTo(evt.print(),S(interval),(int)intervalMinutes() );
			break;

/*---------------------------------------------*/
		case CONFIGURE:
			if (evt.key().id == S(interval))
			{
				setIntervalMinutes( xPL_Int(evt.key().value) );
			}
			break;

		case LOAD_CFG:
			if (evt.eeprom().isxPL())
				evt.eeprom().readAny(_interval);
			else
				_interval = XPL_HBEAT_INTERVAL;

			_trigHbeat = true;
			break;

		case STORE_CFG:
			evt.eeprom().writeAny(_interval);
			break;
/*---------------------------------------------*/
		default:
			break;
	}
	return len;
}
