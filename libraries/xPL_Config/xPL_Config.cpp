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

#include "xPL_Config.h"

xPL_Config xplConfig;

/*****************************************
EEPROM
*****************************************/
bool xPL_Config::loadDefaultConfig()
{
	_interval = XPL_CONFIG_INTERVAL;
	xPL.trigHbeat();
	xPL.setConfigured(false);
	return false;
}
bool xPL_Config::loadConfig(xPL_Eeprom& eeprom)
{
	xPL_Hbeat::loadConfig(eeprom);
	xPL.setConfigured(true);

	xPL.trigHbeat();
	return false;
}




bool xPL_Config::parseMessage(xPL_MessageIn& msg)
{
	if (!targeted(msg)) return false;
	if ( msg.schema.schType() == S(list) || msg.schema.schType() == S(current) )
	{
		if (msg.key_command() == S(request))
		{
			msg.deleteChilds(); //to free memory before sending

			if ( msg.schema.schType() == S(list) )
				sendConfig(S(list));
			else
				sendConfig(S(current));

		}
		return false;
	}
	
	if ( msg.schema.schType() == S(response) )
	{
		if (msg.parseContent())
		{
			xPL.deleteGroups();
			xPL.deleteFilters();

			msg.sendEvent(&xPL_Key::sendEventConfigure,true);

			xPL.deleteGroups(false);
			xPL.deleteFilters(false);

			xPL.storeConfig();

		}
		return false;
	}


	return false;
}


bool xPL_Config::sendConfig(const __FlashStringHelper* type) const
{
	xPL_Message msg(S(stat),S(config),type);

	if ( type == S(list) )
	{
		xPL.sendEvent(&xPL_Node::msgAddConfigList,msg);
	}
	else
	{
		xPL.sendEvent(&xPL_Node::msgAddConfigCurrent,msg);
	}
	return msg.send();

}

