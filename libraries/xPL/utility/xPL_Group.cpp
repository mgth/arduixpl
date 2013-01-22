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

#include "xPL_Group.h"
#include "xPL_Message.h"

bool isGroup(const xPL_Address& addr)
{
	if (addr.vendor != S(xpl)) return false;
	if (addr.device != S(group)) return false;
	return true;
}

xPL_Node* xPL_SchemaGroup::add(const VString& id) { return addChild(new xPL_Group(id)); }

void xPL_SchemaGroup::loop() { _deteteFirst=true; }

size_t xPL_SchemaGroup::printConfig(Print& p,bool list)
{
	if (list)
		return xPL_Message::printOptionKeyTo(p,S(group), XPL_MAX_GROUPS);
	else
	{
		if(!child()) return xPL_Message::printKeyTo(p,S(group),F("") );
		return printConfigChilds(p);
	}
}

void xPL_SchemaGroup::configure(xPL_Key& key)
{
	if ( key.id != S(group) ) return;

	if (_deteteFirst) { deleteChilds(); _deteteFirst=false; }

	xPL_Address addr;
	addr.parse(key.value);

	if (isGroup(addr))
	{
		addChild(new xPL_Group(addr.instance));
//				xPL.trigHbeat(); //TODO
	}
}

void xPL_SchemaGroup::config(xPL_Eeprom& eeprom, bool store)
{
		if (store)
		{
			configChilds(eeprom,true);
			eeprom.write(0);
		}
		else
		{
			deleteChilds();
			VString name = eeprom.readString();
			while(name.len())
			{
				addChild(new xPL_Group(name));
				name = eeprom.readString();
			}
		}
}

bool xPL_Group::checkTarget(xPL_MessageIn& msg)
{
		xPL_Address& target = msg.target;

		return ( isGroup(target) && target.instance == _group );
}
bool xPL_SchemaGroup::checkTarget(xPL_MessageIn& msg)
{
		xPL_Group* g = (xPL_Group*)child();
		while(g) { if (g->checkTarget(msg)) return true; g= (xPL_Group*)g->next();}

		return false;
}

size_t xPL_Group::printConfig(Print& p, bool list)
{
			return xPL_Message::printKeyTo(p,S(group)) + printTo(p);
}

void xPL_Group::config(xPL_Eeprom& eeprom,bool store)
{
	if (store)
		_group.printlnTo(eeprom,'\0');
}



xPL_Group::xPL_Group(const VString& s)
{
	_group = s;
	_group.load();
}


size_t xPL_Group::printTo(Print& p) const {
	//return xPL_Address(S(xpl),S(group),_group).printTo(p);
	//return p.print(S(xpl)) + p.print('-') + p.print(S(group)) + p.print('.') + _group.printTo(p);
	return p.print(F("xpl-group.")) + _group.printTo(p);
}

