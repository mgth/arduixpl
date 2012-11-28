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

#include "xPL_LightingSceneMember.h"
#include "xPL_LightingDevice.h"
#include "xPL_LightingMessage.h"



size_t xPL_LightingSceneMemberState::printTo(Print& p) const {
	/*
	size_t len = 0;
	if (_devid)
		_member->id()->printlnTo(p,',');
	else
		_member->parent()->id()->printlnTo(p,',');

	len += _member->linkedItem()->channelId().printlnTo(p,',');
	len += _member->level().printlnTo(p,',');
	len += _member->faderate().printTo(p);
	return len;
	*/
	return emit_p(p,
		PSTR("$,$,$,$"),
		_devid?_member->id():_member->parent()->id(),
		&_member->level(),
		&_member->level().faderate()
		);
}

xPL_LightingSceneMember::xPL_LightingSceneMember(xPL_LightingNode* item):xPL_LightingNode(xPL_String())
{
    _linked=item;
}

const xPL_String* xPL_LightingSceneMember::id() const 
{
	if (_linked) return _linked->device()->id();
	else
	return NULL;
}

xPL_LightingNode* xPL_LightingSceneMember::setChannelId(const xPL_String& id)
{
	if(id && id != S(_zero)) _linked = (xPL_LightingNode*)_linked->device()->findChild(id);
	else _linked = _linked->device();
	return _linked;
}

bool xPL_LightingSceneMember::parseMessage(xPL_LightingMessage& msg)
{
	if (msg.schema.schType()==S(basic))
	{
		if (msg.key_command()==S(activate))
		{
			
			_linked->sendGoto( _level.percent(msg.level()) );
				/* in official schema, scenes are dot dimmable, it should be implemented like this

				_level,
				*/
				
		}
		else if (msg.key_command()==S(deactivate))
		{
			_linked->sendGoto( xPL_LightingLevel() );
		}
		else if (msg.key_command()==S(goto)) 
		{
			_linked->sendGoto( msg.level() );
		}

	}

#ifdef XPL_LIGHTING_EXTCONFIG
	if ( msg.isConfigCreate() )
	{
			setChannelId( msg.key_channel());
			_level = msg.level();
	}
#endif

	return false;
}

#ifdef XPL_LIGHTING_CONFIG
bool xPL_LightingSceneMember::storeConfig(xPL_Eeprom& eeprom)
{
	xPL_LightingNode::storeConfig(eeprom);
	_linked->channelId().printlnTo(eeprom,'\0'); 
	_level.toEeprom(eeprom);
	return false;
}

bool xPL_LightingSceneMember::loadConfig(xPL_Eeprom& eeprom)
{
	setChannelId( eeprom.readString() );
	_level.fromEeprom(eeprom);


	return false;
}

#endif





