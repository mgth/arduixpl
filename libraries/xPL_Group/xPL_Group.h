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

	  Modified Apr 21, 2012 by Mathieu GRENET
*/
#ifndef xPL_Group_H
#define xPL_Group_H

#include <xPL.h>
#include "utility/xPL_Schema.h"

#ifndef XPL_MAX_GROUPS
#define XPL_MAX_GROUPS 16
#endif

class xPL_Group:public xPL_Node {
private:
	xPL_String _group;
protected:
	virtual bool checkTargeted(xPL_Message& msg);
	virtual bool msgAddConfigCurrent(xPL_Message& msg);
	virtual size_t printTo(Print& p) const;
public:
	xPL_Group(const xPL_String& s);
	virtual bool storeConfig(xPL_Eeprom& eeprom);

};

class xPL_SchemaGroup:public xPL_Schema {
protected:
	virtual const prog_char* className() const { return S(group); }

	virtual xPL_Node* add(const xPL_String& id) { return addChild(new xPL_Group(id)); }

	virtual bool checkTargeted(xPL_Message& msg) { return true; }
	virtual bool msgAddConfigCurrent(xPL_Message& msg);
	virtual bool msgAddConfigList(xPL_Message& msg);
	virtual bool configure(xPL_Key& key);

	virtual bool storeConfig(xPL_Eeprom& eeprom);

};

extern xPL_SchemaGroup xplGroup;

#ifndef XPL_BEGIN
#define XPL_BEGIN ;
#endif

#define XPL_BEGIN XPL_BEGIN##xplGroup.reg();

#endif