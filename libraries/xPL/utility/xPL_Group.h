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

#ifndef xPL_Group_h
#define xPL_Group_h

#include "utility/xPL_Schema.h"

#ifndef XPL_MAX_GROUPS
#define XPL_MAX_GROUPS 16
#endif

class xPL_Group:public xPL_Node {
private:
	VString _group;
protected:
	void config(xPL_Eeprom& eeprom,bool store=false);
	size_t printConfig(Print& p, bool list=false);

	size_t printTo(Print& p) const;
public:
	xPL_Group(const VString& s);
	bool checkTarget(xPL_MessageIn& msg);
};

class xPL_SchemaGroup:public xPL_Schema {
private:
	bool _deteteFirst;

protected:
	virtual const __FlashStringHelper* className() const { return S(group); }

	xPL_Node* add(const VString& id);

	void loop();
	size_t printConfig(Print& p, bool list=false);
	void configure(xPL_Key& key);
	void config(xPL_Eeprom& eeprom, bool store=false);
public:
	bool checkTarget(xPL_MessageIn& msg);
};

#endif