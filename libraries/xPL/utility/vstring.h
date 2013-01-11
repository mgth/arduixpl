// mgstring.h

#ifndef _VSTRING_h
#define _VSTRING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <avr/eeprom.h>
#include <avr/pgmspace.h>


typedef byte vsHelperIndex;

class VString;

class VStringHelper : public Print
{
protected:
	size_t write(uint8_t c) {return 0;}

	virtual VString from(VString& s);
	virtual char charAt(size_t pos, const VString& s) const=0;
	virtual size_t len(const VString& s) const;
	virtual size_t rawlen(const VString& s) const;
	virtual size_t printTo(Print& p, const VString& s) const;
	virtual void destruct(VString& s) const {};
	virtual void copyTo(VString& sTo, const VString& sFrom) const;
	virtual void moveTo(VString& sTo, VString& sFrom) const;

	friend class VString;
};

class VSHelperNone : public VStringHelper
{
protected:
	size_t write(uint8_t c) {return 0;}
	char charAt(size_t pos, const VString& s) const {return '\0';}
	size_t rawlen(const VString& s) const {return 0;}

public:
	static VSHelperNone helper;
};

class VSHelperRam : public VStringHelper
{
protected:
	size_t write(uint8_t c);
	char charAt(size_t pos, const VString& s) const;
	size_t rawlen(const VString& s) const;

public:
	static VSHelperRam helper;

	char* printPtr;
};

class VSHelperFlash : public VStringHelper
{
protected:
	char charAt(size_t pos, const VString& s) const;
	size_t rawlen(const VString& s) const;
public:
	static VSHelperFlash helper;
};

class VSHelperRamAlloc : public VSHelperRam
{
protected:
	virtual void copyTo(VString& sTo, const VString& sFrom) const;
	virtual void moveTo(VString& sTo, VString& sFrom) const;
	VString from(VString& s);
	void destruct(VString& s) const;
public:
		static VSHelperRamAlloc helper;
};

class VSHelperEeprom : public VStringHelper
{
private:
	uint8_t* _ptr;
protected:
	size_t write(uint8_t c);

	VString from(VString& s);
	char charAt(size_t pos, const VString& s) const;

public:
	VSHelperEeprom():_ptr(NULL) {};
	bool setPos(size_t pos) { _ptr = (uint8_t*)pos; return true; }
	
	static VSHelperEeprom helper;
};

class VSHelperPrintable : public VStringHelper
{
protected:
	char charAt(size_t pos, const VString& s) const;
	size_t printTo(Print& p, const VString& s) const;
	size_t rawlen(const VString& s) const;
public:
	static VSHelperPrintable helper;
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
	 	 bool operator==(const VString& s) const;	 bool operator!=(const VString& s) const { return !operator==(s); }	 operator bool() { return _len!=0; }

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

