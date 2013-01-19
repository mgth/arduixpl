#include "xPL_Filter.h"
xPL_SchemaFilter xplFilter;

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

size_t xPL_SchemaFilter::event(const xPL_Event& evt)
{
	switch(evt.id())
	{
/***********************************************
  LOOP
************************************************/
		case LOOP:
			_deleteFirst = true;
			return 0;
/***********************************************
  CONFIG
************************************************/
		case CFG_LIST:
			return xPL_Message::printOptionKeyTo(evt.print(),S(filter), XPL_MAX_FILTERS);
			break;
		case CFG_CURRENT:
			if(!child()) return xPL_Message::printKeyTo(evt.print(),S(filter),F("") );
		case CHK_TARGETED:
			return sendEvent(evt);
/*---------------------------------------------*/
		case CONFIGURE:
			{
				if ( evt.key().id != S(filter) ) return 0;

				if (_deleteFirst) { deleteChilds(); _deleteFirst=false; }

				if (count()>=XPL_MAX_FILTERS) return 0;

				VString& s = evt.key().value; 

				// remove the xpl- in filters to reduce memory usage
				if (s.charAt(0)!='*')
				{
					VString next = s.parseTo('-');
					if ( s != S(xpl) ) return 0;
					s = next;
				}

				xPL_Filter* f = (xPL_Filter*)addChild(new xPL_Filter());
				if (f) f->event(evt);
				return 0;
			}
		case STORE_CFG:
			return sendEvent(evt) + evt.eeprom().write(0);

		case LOAD_CFG:
			while(evt.eeprom().look())
			{
				xPL_Filter* f = (xPL_Filter*)addChild(new xPL_Filter());
				f->event(evt);
			}
			return 0;
/*---------------------------------------------*/
		default:
			break;
	}
	return 0;
}

size_t xPL_Filter::event(const xPL_Event& evt)
{
	size_t len = 0;
	switch(evt.id())
	{
		case CHK_TARGETED:
		{
			byte i=0;

			if ( (_mask & bit(1)) && evt.messageIn().msgType.instance != _field[i++]) return 0;
			if ( (_mask & bit(2)) && evt.messageIn().source.vendor != _field[i++]) return 0;
			if ( (_mask & bit(3)) && evt.messageIn().source.device != _field[i++]) return 0;
			if ( (_mask & bit(4)) && evt.messageIn().source.instance != _field[i++]) return 0;
			if ( (_mask & bit(5)) && evt.messageIn().schema.device != _field[i++]) return 0;
			if ( (_mask & bit(6)) && evt.messageIn().schema.instance != _field[i++]) return 0;

			evt.messageIn().setMatchFilter();
			return 0;
		}
		case CFG_CURRENT:
			len += xPL_Message::printKeyTo(evt.print(),S(filter));
			len += printTo(evt.print());
			break;
		case CONFIGURE:
			{
				VString s = evt.key().value;
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
		case STORE_CFG:
			{
				len += evt.eeprom().write(_mask);
				VString * f = _field;
				for (byte bit=1; bit < bit(7); bit<<=1)
				{
					if (_mask & bit) len += (f++)->printlnTo(evt.eeprom(),'0');
				}	
				return len;
			}
		case LOAD_CFG:
			{
				_mask = evt.eeprom().read();
				byte n;
				byte tmp=_mask; for (n=0; tmp; n++) tmp &= tmp-1; // count bit set

				_field = (VString*)calloc(n,sizeof(VString)); // new VString[n] // TODO arduino 1.0.4;

				for (byte i=0; i<n; i++) setField(i,evt.eeprom().readString());
				return 0;
			}
		default:
			break;
	}
	return len;
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


