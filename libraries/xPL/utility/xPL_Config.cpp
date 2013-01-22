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

	  Modified 2013-1-22 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#include "xPL_Config.h"
#include "../xPL.h"

void xPL_Config::parseMessage(xPL_MessageIn& msg)
{
/*---------------------------------------------
config.list
config.current
-----------------------------------------------*/
	if ( msg.schema.instance == S(list) || msg.schema.instance == S(current) )
	{
		if (msg.key_command() == S(request))
		{
				xPL_MessageConfig(*this,msg.schema.instance == S(list)).send();
		}
		return;
	}

/*---------------------------------------------
config.response
-----------------------------------------------*/
	if ( msg.schema.instance == S(response) )
	{
		if (msg.parseContent())
		{
			xPL_Key* k = msg.keys();
			while(k) {
				xPL.configureChilds(*k);
				k = (xPL_Key*)k->next();
			}
			xPL.storeConfig();
		}
		return;
	}
}

/*---------------------------------------------
configure
-----------------------------------------------*/
void xPL_Config::configure(xPL_Key& key)
{
	if (key.id == S(newconf))
	{
		_newconf = key.value;
	}
	else xPL_Hbeat::configure(key);
}

void xPL_Config::loop()
{
	if (_newconf)
	{
		sendHbeat(S(end));

		if ( xPL.source().instance == _newconf || xPL.setId(_newconf) )
		{
			_configured=true;
			_newconf.clear();
		}
		_trigHbeat = true;
	}
	xPL_Hbeat::loop();
}
/*---------------------------------------------
Eeprom
-----------------------------------------------*/
void xPL_Config::config(xPL_Eeprom& eeprom, bool store)
{
	if (store)
	{
		xPL_Hbeat::config(eeprom,true);
	}
	else
	{
		eeprom.readAny(_interval);
		_configured = true;
		_trigHbeat = true;
	}
}

/*---------------------------------------------
Default
-----------------------------------------------*/
void xPL_Config::defaultConfig()
{
	_interval = XPL_CONFIG_INTERVAL;
	_configured = false;
}

size_t xPL_MessageConfig::printContentTo(Print& p) const{	return xPL.printConfig(p,_list);}