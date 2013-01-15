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

	  Modified Dec 23, 2012 by Mathieu GRENET
*/


#include "xPL_Eeprom.h"
#include <avr/eeprom.h>

xPL_Eeprom::xPL_Eeprom(unsigned int addr):_addr(addr){ }
xPL_Eeprom::xPL_Eeprom():_addr(0){ }

unsigned int xPL_Eeprom::address() { return _addr; }
void xPL_Eeprom::setAddress(unsigned int addr) { _addr=addr; }

/********************************************************************
 INIT
********************************************************************/
//Todo cache isXPL value;
bool xPL_Eeprom::isxPL()
{
	// TODO : enable eeprom config when ok

	return false;

pinMode(XPL_RESET_PIN, INPUT);

return 
		!digitalRead(XPL_RESET_PIN) &&
		eeprom_read_byte( (unsigned char*)0 )=='x' &&
		eeprom_read_byte( (unsigned char*)1 )=='P' &&
		eeprom_read_byte( (unsigned char*)2 )=='L';
}

void xPL_Eeprom::setxPL()
{
	eeprom_write_byte((unsigned char*)0,'x');
	eeprom_write_byte((unsigned char*)1,'P');
	eeprom_write_byte((unsigned char*)2,'L');
}

uint8_t xPL_Eeprom::read()
{
//    return EEPROM.read(_addr++);
	return eeprom_read_byte((unsigned char *) _addr++);

}

uint8_t xPL_Eeprom::look()
{
//    return EEPROM.read(_addr);
	return eeprom_read_byte((unsigned char *) _addr);
}

size_t xPL_Eeprom::mem( byte* p, size_t size, bool w)
{
	size_t i;
	for (i = 0; i < size; i++)
		if (w) write(*p++);
		else *p++ = read();
	return i;
}

/********************************************************************
 READ


int xPL_EepromParser::readInt()
{
	int i = (int)read();
	i += ((int)read())<<8;
	return i;
}
long xPL_EepromParser::readLong()
{
	long l = (long)readInt();
	l += ((long)readInt())<<16;
	return l;
}********************************************************************/

VString xPL_Eeprom::readString(bool step)
{
	VString s = VString(_addr,VSHelperEeprom::helper);
	s.load();
	if (step) _addr += s.len()+1;
	return s;
}

/********************************************************************
 WRITE
********************************************************************/
size_t xPL_Eeprom::write(uint8_t value)
{
	eeprom_write_byte((unsigned char *) _addr++, value);
	return 1;
}

#ifdef XPL_DEBUG
void xPL_Eeprom::dumpToSerial()
{
	for(int i=0;i<1024;i++)
	{
		byte val = eeprom_read_byte((unsigned char *)i);
		Serial.print(val);Serial.print(F(" - "));
		if (val>32)
			Serial.println((char) eeprom_read_byte((unsigned char *)i));
		else 
			Serial.println();
	}
}
#endif

