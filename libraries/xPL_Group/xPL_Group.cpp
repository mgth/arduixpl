#include "xPL_Group.h"
xPL_SchemaGroup xplGroup;

bool isGroup(const xPL_Address& addr)
{
	if (addr.vendor != S(xpl)) return false; //98
	if (addr.device != S(group)) return false;//56
	return true;
}


bool xPL_SchemaGroup::configure(xPL_Key& key) {
	if ( key != S(group) ) return false;

	if (xPL.deleteGroups(false)) { deleteChilds(); }

	xPL_Address addr;
	addr.parse(key.sValue());

	if (isGroup(addr))
	{
		addChild(new xPL_Group(addr.instance));
		xPL.trigHbeat();
	}
	return false;
}

bool xPL_SchemaGroup::msgAddConfigList(xPL_Message& msg)
{
	msg.addOptionKey(S(group), XPL_MAX_GROUPS);
	return false;
}

bool xPL_SchemaGroup::msgAddConfigCurrent(xPL_Message& msg)
{
	if(child()) return true;
	msg.addKey( S(group),(xPL_Printable*)NULL );
	return false;
}

bool xPL_Group::msgAddConfigCurrent(xPL_Message& msg)
{
	msg.addKey( S(group),this );
	return false;
}

xPL_Group::xPL_Group(const xPL_String& s)
{
	_group.load(s);
}

bool xPL_Group::checkTargeted(xPL_Message& msg)
{
	xPL_Address& target = msg.target;

	if ( isGroup(target) && target.instance == _group ) msg.setTargeted();
	
	return false;
}



size_t xPL_Group::printTo(Print& p) const {
	xPL_Address addr;
	addr.vendor = S(xpl);
	addr.device = S(group);
	addr.instance = _group;
	return addr.printTo(p);
}

/*****************************************
EEPROM
*****************************************/

bool xPL_SchemaGroup::storeConfig(xPL_Eeprom& eeprom)
{
	sendEvent(&xPL_Node::storeConfig,eeprom,true);
	eeprom.write(0);
	return false;
}

bool xPL_Group::storeConfig(xPL_Eeprom& eeprom)
{
	_group.printlnTo(eeprom,'\0');
	return false;
}
