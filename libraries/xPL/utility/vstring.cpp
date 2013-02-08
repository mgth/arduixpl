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

#include "vstring.h"
#include "xPL_Define.h"


const VStringHelper** VString::_helper = NULL;
vsHelperIndex VString::_helperCount = 0;

VSHelperNone VSHelperNone::helper;
VSHelperRam VSHelperRam::helper;
VSHelperRamAlloc VSHelperRamAlloc::helper;
VSHelperFlash VSHelperFlash::helper;
VSHelperEeprom VSHelperEeprom::helper;
VSHelperPrintable VSHelperPrintable::helper;
VSHelperPrintableAlloc VSHelperPrintableAlloc::helper;


VString VStringHelper::from(VString& s) {return s;}

size_t VStringHelper::len(const VString& s) const {return s.storedLen();}

size_t VStringHelper::rawlen(const VString& s) const
{
	size_t i=0;	for(;;)	{		if (!charAt(i,s)) break;		i++;	}	return i;}

size_t VStringHelper::printTo(Print& p, const VString& s) const {
		size_t i=0;		for(;;)		{			char c = s.charAt(i);			if (!c) break;			p.print(c);			i++;		}		return i;	
}
void VStringHelper::copyTo(VString& sTo, const VString& sFrom) const
{
	sTo.setAddr(sFrom.addr());
	sTo.setHelper(sFrom.helper());
	sTo.setLen(sFrom.len());
}

void VStringHelper::moveTo(VString& sTo, VString& sFrom) const { copyTo(sTo, sFrom); }

/********************************************************************
Helper:ram
********************************************************************/
char VSHelperRam::charAt(size_t pos, const VString& s) const { return ((char*)s.addr())[pos] ; }
size_t VSHelperRam::rawlen(const VString& s) const {return strlen((char*)s.addr()); }
size_t VSHelperRam::write(uint8_t c) {*printPtr++ = c ; return 1;}

/********************************************************************
Helper:ramAlloc
********************************************************************/
void VSHelperRamAlloc::copyTo(VString& sTo, const VString& sFrom) const
{
	VStringHelper::copyTo(sTo, sFrom);
	sTo.load();
}

void VSHelperRamAlloc::moveTo(VString& sTo, VString& sFrom) const {
	VStringHelper::copyTo(sTo, sFrom);
	sFrom.setLen(0);
	sFrom.setAddr((char*)NULL);
}

VString VSHelperRamAlloc::from(VString& sFrom)
{ 
	VString sTo(sFrom);
	char* addr = (char*)malloc(sFrom.len()+1);
	if (addr)
	{
		sTo.setAddr(addr);
		printPtr = addr;
		sFrom.printTo(*this); print('\0');
	}
	else sTo.setHelper(VSHelperRam::helper);
	return sTo;
}

void VSHelperRamAlloc::destruct(VString& s) const {
	//free((void*)s.addr());
	s.setAddr((char*)NULL);
}

/********************************************************************
Helper:flash
********************************************************************/
char VSHelperFlash::charAt(size_t pos, const VString& s) const {
	return pgm_read_byte((char*)s.addr()+pos);
}
size_t VSHelperFlash::rawlen(const VString& s) const {return strlen_P((char*)s.addr()); }

/********************************************************************
Helper:eeprom
********************************************************************/
char VSHelperEeprom::charAt(size_t pos, const VString& s) const { return eeprom_read_byte(((unsigned char*)s.addr())+pos); }

VString VSHelperEeprom::from(VString& sFrom)
{
	VString sTo((size_t)_ptr,sFrom.len(),*this);
	sFrom.printTo(*this); print('\0');
	return sTo;
}

size_t VSHelperEeprom::write(uint8_t c) {
	char old = eeprom_read_byte(_ptr);
	if (old!=c) eeprom_write_byte(_ptr,c); // TODO : check http://arduino.cc/forum/index.php/topic,137411.0.html
	_ptr++;
	return 1;
}
/********************************************************************
Helper:printable
********************************************************************/
size_t VSHelperPrintable::printTo(Print& p, const VString& s) const {
	if (s.addr())
		return ((Printable*)s.addr())->printTo(p);
	return 0;
}
char VSHelperPrintable::charAt(size_t pos, const VString& s) const {
	class :public Print { size_t write(uint8_t c) { return 1; } } p;

	return s.printTo(p);
}
size_t VSHelperPrintable::rawlen(const VString& s) const {

	nullPrinter p;

	return s.printTo(p);
}
size_t VSHelperPrintable::len(const VString& s) const {
	return rawlen(s);
}

void VSHelperPrintableAlloc::destruct(VString& s) const {
	free((void*)s.addr());
	s.setAddr((char*)NULL);
}


/********************************************************************
VString
********************************************************************/
bool VString::setHelper(const VStringHelper& se) {
	vsHelperIndex i=0;
	while(i<_helperCount && _helper[i]!=&se) i++;

	if (i==_helperCount)
	{
		const VStringHelper** tmp = (const VStringHelper**)realloc(_helper,(_helperCount+1)*sizeof(VStringHelper));
		if (tmp)
		{
			_helper = tmp;
			_helper[i] = &se;
			_helperCount++;
		}
		else return false;
	}
	_helperIdx=i;
	return true;
}

VString VString::parseTo(char c) {

	size_t pos = 0;
	size_t l = len();
		while(charAt(pos)!=c && (pos<l)) pos++;

		setLen(pos);
		pos++;

		if (pos<l)
			return VString(addr(pos),l - pos,helper());
		
		return VString();
}

VString & VString::operator = (const VString &rhs)
{
	if (this == &rhs) return *this;
	
	rhs.helper().copyTo(*this,rhs);
	
	return *this;
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
VString & VString::operator = (String &&rval)
{
	if (this != &rval) rval.helper().moveTo(*this,rval);
	return *this;
}
#endif

bool VString::operator==(const VString& s) const {	size_t i=0;	for (;;)	{		char c = charAt(i);		if (s.charAt(i)!=c) return false;		if (!c) return true;		i++;	}}bool VString::toBool() const {

	if (
		operator==(F("true")) ||  
	    operator==(F("1"))  ||  
		operator==(F("on"))     ||
		operator==(F("yes"))
	) return true;
	
	return false;
}
