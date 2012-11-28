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

	  Modified Aug 31, 2012 by Mathieu GRENET
*/

#ifndef xPL_LightingSCENEELEMENT_H
#define xPL_LightingSCENEELEMENT_H
#include <xPL.h>

#include "xPL_LightingTypes.h"
#include "xPL_LightingNode.h"


class xPL_LightingSceneMember;

class xPL_LightingSceneMemberState: public xPL_Printable {
private:
	xPL_LightingSceneMember* _member;
	bool _devid;
public:
	xPL_LightingSceneMemberState(xPL_LightingSceneMember* m, bool devid=true) { _member=m; _devid=devid; }
	size_t printTo(Print& p) const;
};

class xPL_LightingSceneMember : public xPL_LightingNode
{
  
  private:
    xPL_LightingNode* _linked;
    xPL_LightingLevel _level;

protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);

#ifdef XPL_LIGHTING_CONFIG
	virtual bool storeConfig(xPL_Eeprom& eeprom);
	virtual bool loadConfig(xPL_Eeprom& eeprom);
#endif

  public:
 	virtual const prog_char* className() const { return S(scene);}
   
    xPL_LightingSceneMember(xPL_LightingNode* item);

    xPL_LightingNode* linkedItem() { return _linked; }
    xPL_LightingLevel& level() { return _level; }
	
	const xPL_String* id() const ;
	xPL_LightingNode* setChannelId(const xPL_String& id);

};

#endif