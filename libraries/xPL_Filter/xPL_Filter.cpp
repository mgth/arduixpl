#include "xPL_Filter.h"
xPL_SchemaFilter xplFilter;

void xPL_Filter::setField(byte i,const xPL_String& s)
{
	if (s.isAny()){_field[i].clear();}
	else _field[i].load(s);
}

bool xPL_SchemaFilter::configure(xPL_Key& key) {

	if ( key != S(filter) ) return false;

	if (xPL.deleteFilters(false)) { deleteChilds(); }

	// TODO : check max childs count

	xPL_String s = key.sValue(); 
	xPL_String next;

	if (!s.isAny())
	{
		s.parseTo('-',next);
		if ( s != S(xpl) ) return false;
		s = next;
	}

	xPL_Filter* f = new xPL_Filter();
	if (addChild(f))
	{
		for (int i=0; i<6;i++){
			s.parseTo('.',next);
			f->setField(i,s);
			s = next;
		}
	}
	xPL.trigHbeat();
	return false;
}

bool xPL_SchemaFilter::msgAddConfigList(xPL_Message& msg)
{
	msg.addOptionKey( S(filter),XPL_MAX_FILTERS );
	return false;
}

bool xPL_SchemaFilter::msgAddConfigCurrent(xPL_Message& msg)
{
	if(child()) return true;
	msg.addKey( S(filter),(xPL_Printable*)NULL );
	return false;
}

bool xPL_Filter::msgAddConfigCurrent(xPL_Message& msg)
{
	msg.addKey( S(filter),this );
	return false;
}

size_t xPL_Filter::printTo(Print& p) const
{
	size_t l=0;


	const xPL_String* s = &_field[0];

	if (*s) l += xPL_Printable::println_prog(p,S(xpl),'-'); //S(xpl).printlnTo(p,'-');

	while(s<&_field[5])
	{
		l+=s->printlnTo(p,'.');
		s++;
	}
	l+=s->printTo(p);
	return l;
}



bool xPL_Filter::checkTargeted(xPL_Message& msg)
{
	const xPL_String* s = &_field[0];

	if (
		msg.msgType.msgType().like(*s++) &&
		msg.source.vendor.like(*s++) &&
		msg.source.device.like(*s++) &&
		msg.source.instance.like(*s++) &&
		msg.schema.schClass().like(*s++) &&
		msg.schema.schType().like(*s)
		) msg.setMatchFilter();
	return false;
}


/*****************************************
EEPROM
*****************************************/
xPL_Node* xPL_SchemaFilter::readConfig(xPL_Eeprom& eeprom)
{
	if (eeprom.look()==0xFF) { eeprom.read(); return NULL; }

	return addChild(new xPL_Filter());
}

bool xPL_Filter::loadConfig(xPL_Eeprom& eeprom)
{
	xPL_String s;

	for (int i=0; i<6;i++)
	{
			setField(i,eeprom.readString());
	}	
	return false;
}

bool xPL_SchemaFilter::storeConfig(xPL_Eeprom& eeprom)
{
	sendEvent(&xPL_Node::storeConfig,eeprom,true);
	eeprom.write(0xFF);
	return false;
}

bool xPL_Filter::storeConfig(xPL_Eeprom& eeprom)
{
		for (int i=0; i<6;i++)
		{
			_field[i].printlnTo(eeprom,'0');
		}	
	return false;
}
