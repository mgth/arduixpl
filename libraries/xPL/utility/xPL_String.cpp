#include <avr/eeprom.h>

#include "xPL_String.h"
#include "xPL_Numeric.h"

class printableParser:public Print {
public:
	int pos;
	char c;
	printableParser(size_t p):pos(p),c('\0') { }
	size_t write (uint8_t v) { if (!pos) c=v; pos++; return 1; }
};

size_t xPL_Printable::printlnTo(Print& p,char c) const{	size_t i = printTo(p);	i += p.print(c);	return i;}
size_t xPL_Printable::printDebug() const{	size_t l = Serial.print('<');	l += printlnTo(Serial,'>');	l += Serial.println();	return l;}


char intCharAt(int i,size_t pos,char last='\0') {
	int tmp = i;
	int8_t n = 0; while (tmp>=10) { tmp/=10; n++; }
	n -= pos;
	if ( n == -1 ) return last;
	if ( n <  0  ) { if (last!='\0') return -1-n; return '\0';}

	for(size_t p=0;p<(size_t)n;++p) i/=10;
	i%=10;
	return  '0' + i;
}

 char xPL_String::charAt(size_t pos) const{

	char* ptr = (char*)_ptr+pos;

	switch( _state.type )
	{
		case ram:
			return *ptr;
		case prog:
			return pgm_read_byte(ptr);
		case eeprom:
			return eeprom_read_byte((unsigned char*)ptr);
		case printable:
			printableParser pp(-pos);
			pp.print(*(Printable*)_ptr);
			return pp.c;
	}
	return '\0';
}

//xPL_String::xPL_String(xPL_StringType  t,char* ptr) { setInit(t,ptr); } xPL_String::xPL_String(char* ptr)				{ setInit(ram, ptr ); }xPL_String::xPL_String(const __FlashStringHelper* ptr)	{ setInit(prog, (void*)(ptr) ); }xPL_String::xPL_String(const xPL_String& s){	setInit(s._state.type, s._ptr);	if (s._state.allocated) load();}xPL_String::xPL_String(const xPL_Printable& p)	{ setInit(printable, (void*)(&p)); }xPL_String::xPL_String(int i){	xPL_Int* _i = new xPL_Int(i);	setInit(printable, (void*)_i );	if (_i) _state.allocated=true;}xPL_String::xPL_String(float f){	xPL_Float* _f = new xPL_Float(f);	setInit(printable, (void*)_f );	if (_f) _state.allocated=true;}xPL_String::xPL_String(xPL_StringType  t)           { setInit(t,NULL); }xPL_String::xPL_String()                            { setInit(none,NULL); }
xPL_String::~xPL_String() { 	if (_state.allocated) { free((void*)_ptr); /*fix28135_malloc_bug();*/}}
xPL_String::operator bool_type() const{      return isValid() ?         &xPL_String::this_type_does_not_support_comparisons : 0;}
bool xPL_String::isValid() const { return (_ptr!=NULL); }bool xPL_String::readOnly() const { return _state.type!=ram; }
void xPL_String::set(xPL_StringType t,void* ptr){	if (_state.allocated) { clear(); }	_state.type=t; _ptr=ptr;}
void xPL_String::setInit(xPL_StringType t,void* ptr){	_state.allocated = false;	_state.type=none;	set(t,ptr);}void xPL_String::operator=(const xPL_String& s){	if (s._state.allocated) load(s);	else	set(s._state.type, s._ptr);}void  xPL_String::operator=(const __FlashStringHelper* s)		{ set(prog,(char*)s); }
const bool xPL_String::isAny() const { return charAt(0)=='*'; }
size_t xPL_String::len() const{	switch(_state.type)	{	case ram:		return strlen((char*)_ptr);	case prog:		return strlen_P((const char*)_ptr);	case printable:		{			printableParser pp(0);
			pp.print(*(Printable*)_ptr);
			return pp.pos;
		}
	default:		{			size_t i=0;			for(;;)			{				if (!charAt(i)) break;				i++;			}			return i;		}	}}

size_t xPL_String::printTo(Print& p) const {			if (!*this) return p.print('*');#ifdef XPL_DEBUG	if (len()>512) return p.print(F("?str?"));#endif	switch( _state.type )
	{
	case ram:
		return p.print((char*)_ptr);
	case printable:
		return p.print(*(Printable*)_ptr);
	case prog:
		return p.print((const __FlashStringHelper*)_ptr);
	default:
		size_t i=0;
		for(;;)		{			char c = charAt(i);			if (!c) break;			p.print(c);			i++;		}
		return i;
	}}char xPL_String::lowerCharAt(size_t& pos) const {	char c = charAt(pos);	if (c>='A' && c<='Z') c-=('A'-'a');	return c;}bool xPL_String::operator==(const xPL_String& s) const {	size_t i=0;	for (;;)	{		char c = lowerCharAt(i);		if (s.lowerCharAt(i)!=c) return false;		if (!c) return true;		i++;	}}bool xPL_String::cmp(const xPL_String& s) {	if (operator==(s)) {		*this=s;		return true;	}	return false;}bool xPL_String::operator!=(const xPL_String& s) const { return !operator==(s); }bool xPL_String::operator!=(const __FlashStringHelper* s) const { return !operator==(s); }bool xPL_String::like(const xPL_String& s) const {	if (!s) return true;	//if (s.isAny()) return true;	return operator==(s);}/*int xPL_String::toInt() const { 	//return 0; 	//return atoi(_ptr); //78	int i=0;
	size_t pos=0;

	for(;;)
	{
		char c = charAt(pos);
		if (!c) break;
		i = i*10 + ( c - '0' );
		pos++;
	}
	return(i);}*/
/*
 * parseTo(char c,xPL_String s)
 * 
 * 
 */
char* xPL_String::parseTo(char c) {
	if (isValid() && !readOnly())
	{
		char* ptr = (char*)_ptr;
		while(*ptr!=c && *ptr) ptr++;

		if (*ptr) { *ptr=0;++ptr; } else ptr=NULL;
		return ptr;
	}
	return NULL;
}

void xPL_String::moveTo(xPL_String& s)
{
	s.clear();
	s._state=_state;
	_state.allocated = false;
	s._ptr=_ptr;
}

void xPL_String::clear() {
	if (_state.allocated) { free(_ptr); /*fix28135_malloc_bug();*/ }
	_ptr=NULL;
	_state.type = none;
	_state.allocated = none;
}

size_t xPL_String::load() { 		
	return load(*this);
 }


size_t xPL_String::load(const xPL_String& s) { 	

	size_t len=s.len();

	xPL_BufferFiller buf(len+1);

	s.printlnTo(buf,'\0');
	set(ram,(void*)buf.buffer());
	_state.allocated=true;
	buf.unlink();

	return len;
}

bool xPL_String::toBool() const {

	if (
		operator==(S(_true)) ||  // 18
	    operator==(S(_one))  ||  // 14
		operator==(S(on))     ||  // 16
		operator==(S(yes))  // 16
	) return true;
	
	return false;
}


/*size_t xPL_Printable::printKey(Print& p,const prog_char* ptr) const {
	size_t l = println_prog(p,ptr,'=');
	l+=printlnTo(p);
	return l;
	
}
*/
bool xPL_String::operator==(const __FlashStringHelper* c) const {
	return operator==(xPL_String(c));
}

size_t xPL_Printable::emit_p(Print& p, PGM_P fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

	size_t len = 0;

    for (;;) {
        char c = pgm_read_byte(fmt++);
        if (c == 0)
            break;
        switch (c) {
            case '$':
				len += p.print(*va_arg(ap, Printable*));
                break;
            case '&':
                len += p.print(xPL_String(va_arg(ap,const __FlashStringHelper*)));
                break;
            default:
                len += p.print(c);
                break;
        }
    }
    va_end(ap);
	return len;
}

