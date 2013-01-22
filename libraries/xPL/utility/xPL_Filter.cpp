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

#include "xPL_Filter.h"
#include "../xPL.h"

void xPL_Filter::setField(byte i,const VString& s)
{
	if (s==S(_asterisk) || s.len()==0) _field[i]=S(_asterisk);
	else if (s==S(cmnd)) _field[i]=S(cmnd);
	else if (s==S(stat)) _field[i]=S(stat);
	else if (s==S(trig)) _field[i]=S(trig);
	else
	{
		_field[i]=s;
		_field[i].load();
	}
}

void xPL_SchemaFilter::loop() { _deleteFirst = true; }

size_t xPL_SchemaFilter::printConfig(Print& p, bool list)
{
	if(list)
		return xPL_Message::printOptionKeyTo(p,S(filter), XPL_MAX_FILTERS);
	else
	{
		if(!child()) return xPL_Message::printKeyTo(p,S(filter),F("") );
		else return printConfigChilds(p);
	}
}


void xPL_SchemaFilter::configure(xPL_Key& key)
{
				if ( key.id != S(filter) ) return;

				if (_deleteFirst) { deleteChilds(); _deleteFirst=false; }

				if (count()>=XPL_MAX_FILTERS) return;

				VString& s = key.value; 

				// remove the xpl- in filters to reduce memory usage
				if (s.charAt(0)!='*')
				{
					VString next = s.parseTo('-');
					if ( s != S(xpl) ) return;
					s = next;
				}

				xPL_Filter* f = (xPL_Filter*)addChild(new xPL_Filter());
				if (f) f->configure(key);
}

void xPL_SchemaFilter::config(xPL_Eeprom& eeprom, bool store)
{
		if (store)
		{
			configChilds(eeprom,true);
			eeprom.write(0);
		}
		else
		{
			while(eeprom.look())
			{
				xPL_Filter* f = (xPL_Filter*)addChild(new xPL_Filter());
				f->config(eeprom);
			}
		}
}

bool xPL_SchemaFilter::checkTarget(xPL_MessageIn& msg)
{
		xPL_Filter* f = (xPL_Filter*)child();

		if (!f) return true;

		while(f) { if (f->checkTarget(msg)) return true; f= (xPL_Filter*)f->next();}

		return false;
}

bool xPL_Filter::checkTarget(xPL_MessageIn& msg)
{
			byte i=0;

			if ( 
				((_mask & bit(1)) && msg.msgType.instance != _field[i++])
			||  ((_mask & bit(2)) && msg.source.vendor != _field[i++])
			||	((_mask & bit(3)) && msg.source.device != _field[i++])
			||	((_mask & bit(4)) && msg.source.instance != _field[i++])
			||	((_mask & bit(5)) && msg.schema.device != _field[i++])
			||	((_mask & bit(6)) && msg.schema.instance != _field[i++]) 
				) return false;

			return true;
}

void xPL_Filter::config(xPL_Eeprom& eeprom,bool store)
{
	if (store)
	{
		eeprom.write(_mask);
		VString * f = _field;
		for (byte bit=1; bit < bit(7); bit<<=1)
		{
			if (_mask & bit) (f++)->printlnTo(eeprom,'0');
		}	
	}
	else
	{
		_mask = eeprom.read();
		byte n;
		byte tmp=_mask; for (n=0; tmp; n++) tmp &= tmp-1; // count bit set

		_field = (VString*)calloc(n,sizeof(VString)); // new VString[n] // TODO arduino 1.0.4;

		for (byte i=0; i<n; i++) setField(i,eeprom.readString());
	}
}

void xPL_Filter::configure(xPL_Key& key)
{
	VString s = key.value;
	VString next;

	byte n = 6 - s.count('*');

	_field = (VString*)calloc(n,sizeof(VString)); // new VString[n] // TODO arduino 1.0.4;
	_mask =0;
	byte bit=1;
	for (byte i=0;i<n;i++)
	{
		while (s.charAt(0)=='*')
		{
			s = s.parseTo('.'); bit<<=1;
		}
		next = s.parseTo('.');
		setField(i,s);
		_mask |= bit;
		bit<<=1;
		s=next;
	}
}

size_t xPL_Filter::printConfig(Print& p, bool list)
{
	return xPL_Message::printKeyTo(p,S(filter)) + printTo(p);
}


size_t xPL_Filter::printTo(Print& p) const
{
	size_t len=0;

	byte bit = 1;
	if (_mask & bit) len += p.print(F("xpl-")); 

	for(byte i=0;i<6;i++)
	{
		if (_mask & bit) len += _field[i].printTo(p);
		else len += p.print('*');

		if (i<5) len += p.print('.');
		bit <<= 1;
	}

	return len;
}


