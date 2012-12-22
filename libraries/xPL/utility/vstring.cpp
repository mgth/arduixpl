// 
// 
// 

#include "vstring.h"

const VStringHelper** VString::_helper = NULL;
vsHelperIndex VString::_helperCount = 0;

VSHelperNone VSHelperNone::helper;
VSHelperRam VSHelperRam::helper;
VSHelperRamAlloc VSHelperRamAlloc::helper;
VSHelperFlash VSHelperFlash::helper;
VSHelperEeprom VSHelperEeprom::helper;
VSHelperPrintable VSHelperPrintable::helper;


VString VStringHelper::from(VString& s) {return s;}

size_t VStringHelper::len(const VString& s) const {return s._len;}

size_t VStringHelper::rawlen(const VString& s) const
{
	size_t i=0;

size_t VStringHelper::printTo(const VString& s,Print& p) const {
		size_t i=0;
}
void VStringHelper::copyTo(const VString& sFrom,VString& sTo) const { sTo._addr = sFrom._addr; sTo._helper=sFrom._helper; sTo._len=sFrom._len; }
void VStringHelper::moveTo(VString& sFrom,VString& sTo) const { copyTo(sFrom,sTo); }

/********************************************************************
Helper:ram
********************************************************************/
char VSHelperRam::charAt(const VString& s, size_t pos) const { return ((char*)s.addr())[pos] ; }
size_t VSHelperRam::rawlen(const VString& s) const {return strlen((char*)s.addr()); }
size_t VSHelperRam::write(uint8_t c) {*printPtr++ = c ; return 1;}

/********************************************************************
Helper:ramAlloc
********************************************************************/
void VSHelperRamAlloc::copyTo(const VString& sFrom,VString& sTo) const
{
	VStringHelper::copyTo(sFrom,sTo);
	sTo.load();
}

void VSHelperRamAlloc::moveTo(VString& sFrom,VString& sTo) const { VStringHelper::copyTo(sFrom,sTo); }

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
	free((void*)s.addr());
	s.setAddr((char*)NULL);
}

/********************************************************************
Helper:flash
********************************************************************/
char VSHelperFlash::charAt(const VString& s, size_t pos) const {
	return pgm_read_byte((char*)s.addr()+pos);
}
size_t VSHelperFlash::rawlen(const VString& s) const {return strlen_P((char*)s.addr()); }

/********************************************************************
Helper:eeprom
********************************************************************/
char VSHelperEeprom::charAt(const VString& s, size_t pos) const { return eeprom_read_byte(((unsigned char*)s.addr())+pos); }

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
size_t VSHelperPrintable::printTo(const VString& s,Print& p) const {
	return ((Printable*)s.addr())->printTo(p);
}
char VSHelperPrintable::charAt(const VString& s, size_t pos) const {
	class :public Print { size_t write(uint8_t c) { return 1; } } p;

	return s.printTo(p);
}
size_t VSHelperPrintable::rawlen(const VString& s) const {

	class :public Print {
		size_t write(uint8_t w) { return 1; /*(w='\x0d')?0:1;*/ }
	  } p;

	return s.printTo(p);
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
			return VString(addr(pos),l - pos,*_helper[_helperIdx]);
		
		return VString(0,0,*_helper[_helperIdx]);
}
/*
VString & VString::operator = (const VString &rhs)
{
	if (this == &rhs) return *this;
	
	rhs.helper().copyTo(rhs,*this);
	
	return *this;
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
VString & VString::operator = (String &&rval)
{
	if (this != &rval) rval.helper().moveTo(*this);
	return *this;
}
#endif
*/
bool VString::operator==(const VString& s) const {

	if (
		operator==(F("true")) ||  // 18
	    operator==(F("1"))  ||  // 14
		operator==(F("on"))     ||  // 16
		operator==(F("yes"))  // 16
	) return true;
	
	return false;
}