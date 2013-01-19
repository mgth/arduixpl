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

#include "xPL_Config.h"

xPL_Config xplConfig;

size_t xPL_Config::event(const xPL_Event& evt)
{
	switch(evt.id())
	{
/***********************************************
  PARSE INCOMING MESSAGES
************************************************/
		case PARSE_MESSAGE:
/*---------------------------------------------
config.request
-----------------------------------------------*/
			if ( evt.messageIn().schema.instance == S(list) || evt.messageIn().schema.instance == S(current) )
			{
				if (evt.messageIn().key_command() == S(request))
				{
					if ( evt.messageIn().schema.instance == S(list) )
						xPL_MessageConfigList().send();
					else
						xPL_MessageConfigCurrent().send();
				}
				return 0;
			}
	
/*---------------------------------------------
config.response
-----------------------------------------------*/
			if ( evt.messageIn().schema.instance == S(response) )
			{
				if (evt.messageIn().parseContent())
				{
					xPL_Key* k = evt.messageIn().keys();
					while(k) {
						xPL.event(xPL_Event(CONFIGURE, k ));
						k = (xPL_Key*)k->next();
					}
					xPL.storeConfig();
				}
				return 0;
			}
			break;
/***********************************************
  CONFIG
************************************************/
		case CONFIGURE:
			if (evt.key().id == S(newconf))
			{
				VString& newconf = evt.key().value;

				if ( xPL.source().instance == newconf || xPL.setId(newconf) )
				{
					_configured=true;
				}
				_trigHbeat = true;
				return 0;
			}
			break;
/*---------------------------------------------*/
		case LOAD_CFG:
			if (evt.eeprom().isxPL())
			{
				evt.eeprom().readAny(_interval);
				_configured = true;
			}
			else
			{
				_interval = XPL_CONFIG_INTERVAL;
				_configured = false;
			}
			_trigHbeat = true;
			return 0;
/*---------------------------------------------*/
		default:
			break;
	}
	return xPL_Hbeat::event(evt);
}
