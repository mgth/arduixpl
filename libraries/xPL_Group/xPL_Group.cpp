#include "xPL_Group.h"
xPL_SchemaGroup xplGroup;

bool isGroup(const xPL_Address& addr)
{
	if (addr.vendor != S(xpl)) return false;
	if (addr.device != S(group)) return false;
	return true;
}

size_t xPL_SchemaGroup::event(const xPL_Event& evt)
{
	switch(evt.id())
	{
/***********************************************
  LOOP
************************************************/
		case LOOP:
			_deteteFirst=true;
			return 0;
/***********************************************
  CONFIG
************************************************/
		case CFG_LIST:
			return xPL_Message::printOptionKeyTo(evt.print(),S(group), XPL_MAX_GROUPS);

		case CFG_CURRENT:
			if(!child()) return xPL_Message::printKeyTo(evt.print(),S(group),F("") );
		case CHK_TARGETED:
			return sendEvent(evt);
/*---------------------------------------------*/
		case CONFIGURE:
			{
				if ( evt.key().id != S(group) ) return 0;

				if (_deteteFirst) { deleteChilds(); _deteteFirst=false; }

				xPL_Address addr;
				addr.parse(evt.key().value);

				if (isGroup(addr))
				{
					addChild(new xPL_Group(addr.instance));
	//				xPL.trigHbeat(); //TODO
				}
			}
			return 0;
		case STORE_CFG:
			return sendEvent(evt) + evt.eeprom().write(0);

/*---------------------------------------------*/
		default:
			break;
	}
	return 0;
}

size_t xPL_Group::event(const xPL_Event& evt)
{
	size_t len = 0;
	switch(evt.id())
	{
		case CHK_TARGETED:
		{
			xPL_Address& target = evt.messageIn().target;

			if ( isGroup(target) && target.instance == _group )
				evt.messageIn().setTargeted();
		}
		case CFG_CURRENT:
			len += xPL_Message::printKeyTo(evt.print(),S(group));
			len += printTo(evt.print());
			break;

		case STORE_CFG:
			return _group.printlnTo(evt.eeprom(),'\0');

		default:
			break;
	}
	return len;
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

