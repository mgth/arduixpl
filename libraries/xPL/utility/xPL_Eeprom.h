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

#ifndef XPL_EEPROM_H
#define	XPL_EEPROM_H
#include "xPL_Define.h"
#include "VString.h"
//#include <../eeprom/eeprom.h>

class xPL_Eeprom : public Print {
private:
	// position de lecture courante
    unsigned int _addr;
    
public:
    xPL_Eeprom(unsigned int addr);
    xPL_Eeprom();

    unsigned int address();
    void setAddress(unsigned int addr);

	bool isxPL();
	void setxPL();
 
	uint8_t look(); // do not
    uint8_t read(); // increment current pos
/*
	int readInt();
	long readLong();
	*/
	VString readString(bool step=true);

    virtual size_t write(uint8_t value);

	size_t mem( byte* p, size_t size, bool w=false);


	template <class T> size_t writeAny(T& value)
	{
		byte* p = (byte*)(void*)&value;
		return mem(p,sizeof(value),true);
	}
	template <class T> size_t readAny(T& value)
	{
		byte* p = (byte*)(void*)&value;
		return mem(p,sizeof(value));
	}
	/*
	int readMem(byte* p, int size)
	{
		unsigned int i;
		for (i = 0; i < size; i++)
			*p++ = read();
		return i;
	}
	*/
	
	/*
    template <typename t>
    t read()
    {
		t value;
		byte* p = (byte*)(void*)&value;

		int i;
		for (i = 0; i < (int)sizeof(value); i++)
			*p++ = read();

        return value;
    }
    
   template <typename t>
    size_t write(t value)
    {
		const byte* p = (const byte*)(const void*)&value;
		size_t len=0;
        while(len<(int)sizeof(t)) len += write(*p++);
			
		return len;		
    }*/

   

#ifdef XPL_DEBUG
    void dumpToSerial();
#endif
};

#endif	/* XPL_EEPROMPARSER_H */

