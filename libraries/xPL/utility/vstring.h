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

#ifndef _VSTRING_H
#define _VSTRING_H

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <Print.h>

typedef int8_t vsHelperIndex;

class VString;

class nullPrinter : public Print {
		size_t write(uint8_t w) { return 1; }
};

class VStringHelper : public Print
{
protected:
	size_t write(uint8_t c) {return 0;}

public:
	virtual VString from(VString& s);
	virtual char charAt(size_t pos, const VString& s) const=0;
	virtual size_t len(const VString& s) const;
	virtual size_t rawlen(const VString& s) const;
	virtual size_t printTo(Print& p, const VString& s) const;
	virtual void destruct(VString& s) const {};
	virtual void copyTo(VString& sTo, const VString& sFrom) const;
	virtual void moveTo(VString& sTo, VString& sFrom) const;
};

class VSHelperNone : public VStringHelper
{
protected:
	size_t write(uint8_t c) {return 0;}
public:
	char charAt(size_t pos, const VString& s) const {return '\0';}
	size_t rawlen(const VString& s) const {return 0;}

	static VSHelperNone helper;
};

class VSHelperRam : public VStringHelper
{
protected:
	size_t write(uint8_t c);
public:
	char charAt(size_t pos, const VString& s) const;
	size_t rawlen(const VString& s) const;

	static VSHelperRam helper;

	char* printPtr;
};

class VSHelperFlash : public VStringHelper
{
protected:
public:
	char charAt(size_t pos, const VString& s) const;
	size_t rawlen(const VString& s) const;
	static VSHelperFlash helper;
};

class VSHelperRamAlloc : public VSHelperRam
{
protected:
public:
	virtual void copyTo(VString& sTo, const VString& sFrom) const;
	virtual void moveTo(VString& sTo, VString& sFrom) const;
	VString from(VString& s);
	void destruct(VString& s) const;
		static VSHelperRamAlloc helper;
};

class VSHelperEeprom : public VStringHelper
{
private:
	uint8_t* _ptr;
protected:
	size_t write(uint8_t c);

public:
	VString from(VString& s);
	char charAt(size_t pos, const VString& s) const;

	VSHelperEeprom():_ptr(NULL) {};
	bool setPos(size_t pos) { _ptr = (uint8_t*)pos; return true; }
	
	static VSHelperEeprom helper;
};


class VSHelperPrintable : public VStringHelper
{
protected:
public:
	char charAt(size_t pos, const VString& s) const;
	size_t printTo(Print& p, const VString& s) const;
	size_t rawlen(const VString& s) const;
	size_t len(const VString& s) const;
	static VSHelperPrintable helper;
};

class VSHelperPrintableAlloc : public VSHelperPrintable
{
protected:
	void destruct(VString& s) const;
public:
	static VSHelperPrintableAlloc helper;
};

class VString
{
 protected:
	vsHelperIndex _helperIdx;
	size_t _addr;
	size_t _len;

	static const VStringHelper** _helper;
	static vsHelperIndex _helperCount;

 public:

	bool setHelper(const VStringHelper& se);

	const VStringHelper& helper() const { return *(_helper[_helperIdx]); }

	 VString() { setAddr((size_t)0); setHelper(VSHelperNone::helper); _len=rawLen(); }
	 VString(char* s) { setAddr(s); setHelper(VSHelperRam::helper); _len=rawLen(); }
	 VString(const __FlashStringHelper* f) { setAddr(f); setHelper(VSHelperFlash::helper); _len=rawLen();  }
	 VString(const Printable* p) { setAddr(p); setHelper(VSHelperPrintable::helper); _len=rawLen();  }

	 VString(size_t addr,size_t len,const VStringHelper& h) { _addr=addr; setHelper(h); _len=len; }
	 VString(size_t addr,const VStringHelper& h) { _addr=addr; setHelper(h); _len=rawLen(); }
	 VString(const VString& s) { setAddr(s.addr()); _helperIdx=s._helperIdx; _len=s._len; }

	 size_t printTo(Print& p) const { return (_len)?helper().printTo(p,*this):0; }
	
	 size_t printlnTo(Print&p, char ln='\n') const { return printTo(p) + p.print(ln); }
	 size_t printNzTo(Print& p) const { return (_len)?printTo(p):0; }	 size_t printlnNzTo(Print& p, char ln='\n') const { return (_len)?printlnTo(p,ln):0; }
	 size_t storedLen() const { return _len; /*return helper().len(*this);*/ }
	 size_t len() const { return helper().len(*this); }
	 size_t rawLen() const { return helper().rawlen(*this); }

	 char charAt(size_t pos) const {
		 if(pos>=len()) return '\0';
		 return helper().charAt(pos, *this);
	 }
	 void setLen(size_t len) { _len=len; }

	 size_t addr(size_t pos=0) const { return _addr+pos; }

	 void setAddr(size_t addr) {_addr=addr;}
	 void setAddr(char* addr) {_addr=(size_t)addr;}
	 void setAddr(const __FlashStringHelper* f) {_addr=(size_t)f;}
	 void setAddr(const Printable* p) {_addr=(size_t)p;}

	 VString to(VStringHelper& sh) {
		 return sh.from(*this);
	 }

	 void convert(VStringHelper& sh) {
		VString s = to(sh);
		destruct();
		*this = s;
	 }

	 void clear() {
		destruct();
		setHelper(VSHelperNone::helper);
		_len=0;
		_addr=0;
	 }

	 void destruct() { if (_len) helper().destruct(*this); }

	 bool load() {
		 *this = to(VSHelperRamAlloc::helper); 
		 return true;
	 }

	VString & operator = (const VString &rhs);
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
	VString & operator = (VString &&rval);
	#endif
	 	bool like(const VString& s) { return ( s.charAt(0)=='*' || operator==(s) ); }	size_t count(char c) {		size_t n=0;		for(size_t i=0;i<_len;i++)			if (charAt(i)==c) n++;		return n;	}	 bool operator==(const VString& s) const;	 bool operator!=(const VString& s) const { return !operator==(s); }	 operator bool() { return _len!=0; }

	 ~VString() { destruct(); }

	 VString parseTo(char c);

	bool toBool() const;	
	friend class VStringHelper;
};

class VStringP : public VString, public Printable
{
public:
	 VStringP(char* s):VString(s) { }
	 VStringP(const __FlashStringHelper* f):VString(f) {}
	 VStringP(const VString& s):VString(s) { }

	size_t printTo(Print& p) const { return VString::printTo(p); }
};

#endif

