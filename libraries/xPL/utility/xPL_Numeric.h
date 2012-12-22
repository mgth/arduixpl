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

	  Modified 20 Mars 2012 by Mathieu GRENET
*/

#ifndef XPL_NUMERIC_H
#define XPL_NUMERIC_H

#include <xPL.h>
#include "xPL_Eeprom.h"

class xPL_Numeric: public xPL_Printable
{
protected:
	virtual void store(uint16_t val,uint8_t dec=0)=0;
	virtual uint16_t load(uint8_t dec=0) const =0;

	virtual uint8_t dec() const { return 2; }

public:

virtual void fromString(const VString& s)
	{
			unsigned long val=0;
			//bool indec = false;
			int8_t dec = -127;

			for (size_t i=0;i<s.len();i++)
			{
				char c = s.charAt(i);

				if ( c=='.' ) dec=0;
				else 
	#ifdef XPL_PARSE_STRICT
				if ( (c&0xF0) == 0x30 )// (c>='0' && c<='9' )
	#endif
				{ 
					c &= 0x0F;
					byte mul = 10;
	#ifdef XPL_PARSE_STRICT
					if (c<mul)
	#endif
					{
						val*=mul;
						val+=c;
						dec++;
					}
				}
			}

			store(val,(dec>0)?dec:0);

	}

	static unsigned long divider(int8_t dec) {
		long div=1;
		while (dec>0) {div*=10; dec--; }
		return div;
	}

	static size_t printLong(unsigned long num,int8_t dec,Print& p, bool trailingZeros=false)
	{
		size_t len=0;

		while(!trailingZeros && !(num%10) && dec) { num/=10; dec--; }

		if(num>9)
		{
			len += printLong(num/10,dec-1,p,true);
			if (dec==1) len += p.print('.');
		}
		len += p.print((char)('0'+(num%10)));
		return len;
	}



	size_t printDecTo(int8_t dec,Print& p) const
	{
		long l = load(dec+1);
		if ((l%10)>5) l+=10;
		return printLong(l/10,dec,p);
	}


};


class xPL_Int:public xPL_Numeric
{
	int _val;
protected:
	virtual void store(uint16_t val,uint8_t dec=0) { _val = (int)val; }
	virtual uint16_t load(uint8_t dec=0) const {return _val*divider(dec);}
	virtual byte dec() const { return 0; }

	size_t printTo(Print& p) const
	{
		return p.print(_val);
		//return printDecTo(0,p);
	}
public:
	xPL_Int(const VString& s) { fromString(s); }
	xPL_Int(int i):_val(i) { }

	operator int() { return _val; }

};

class xPL_Float:public xPL_Numeric
{
	float _val;
protected:
	virtual void store(uint16_t val,uint8_t dec=0) { _val = (float)val/divider(dec); }
	virtual uint16_t load(uint8_t dec=0) const {return _val*divider(dec);}
	virtual byte dec() const { return 0; }

	size_t printTo(Print& p) const {
		return printDecTo(2,p);
	}
public:
	xPL_Float() { _val=0.0; }
	xPL_Float(const VString& s) { fromString(s); }
	xPL_Float(float f) { _val=f; }

	operator float() { return _val; }
	xPL_Float& operator=(float val) { _val = val; return *this;}
	bool operator<(float val) { return (_val < val);}
	bool operator>(float val) { return (_val > val);}

};

#endif