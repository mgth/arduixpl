#include <avr/eeprom.h>

#include "xPL_String.h"

size_t xPL_Printable::printlnTo(Print& p,char c) const



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

	switch( type() )
	{
		case XPL_STRING_RAM:
			return *ptr;
		case XPL_STRING_PROG:
			return pgm_read_byte(ptr);
		case XPL_STRING_EEPROM:
			return eeprom_read_byte((unsigned char*)ptr);
		case XPL_STRING_INT:
			return intCharAt((int)_ptr,pos);

		default:
			return '\0';
	}
}

//xPL_String::xPL_String(xPL_StringType  t,char* ptr) { setInit(t,ptr); }

xPL_String::~xPL_String() { 

bool xPL_String::allocated() const { return (_type & XPL_STRING_ALLOC); }
void xPL_String::set(xPL_StringType t,char* ptr)

void xPL_String::operator=(const xPL_String& s) { set(s.type(), s._ptr); }
const bool xPL_String::isAny() const { return charAt(0)=='*'; }
size_t xPL_String::len() const

size_t xPL_String::printTo(Print& p) const {
	size_t pos=0;

	for(;;)
	{
		char c = charAt(pos);
		if (!c) break;
		i = i*10 + ( c - '0' );
		pos++;
	}
	return(i);
/*
 * parseTo(char c,xPL_String s)
 * 
 * 
 */
bool xPL_String::parseTo(char c, xPL_String& s) {
	if (isValid() && !readOnly())
	{
		char* pos = _ptr;
		while(*pos!=c && *pos) pos++;

		if (*pos) { *pos=0;++pos; } else pos=NULL;
		s.set(XPL_STRING_RAM, pos);
		return true;
	}
	return false;
}

void xPL_String::moveTo(xPL_String& s)
{
	s.clear();
	s._type=_type;
	_type = type();
	s._ptr=_ptr;
}

void xPL_String::clear() {
	if (allocated()) { free((void*)_ptr); fix28135_malloc_bug(); }
	_ptr=NULL;
	_type = XPL_STRING_NONE;
}

bool xPL_String::load() { 		
	return load(*this);
 }

bool xPL_String::load(const xPL_String& s) { 	

	size_t len=s.len();

	char* buf =  (char*)malloc(len+1);

	if (buf)
	{
		for (size_t i=0;i<len;i++) { buf[i]=s.charAt(i); }
		buf[len]='\0';
		xPL_String::set(XPL_STRING_RAM|XPL_STRING_ALLOC,buf);
		return true;
	}
	return false;
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

size_t xPL_Printable::println_prog(Print& p, const prog_char* ptr, char ln) {
	xPL_String s = xPL_String(ptr);
	return s.printlnTo(p,ln);
}

size_t xPL_Printable::print_prog(Print& p, const prog_char* ptr) {
	xPL_String s = xPL_String(ptr);
	return s.printTo(p);
}

/*size_t xPL_Printable::printKey(Print& p,const prog_char* ptr) const {
	size_t l = println_prog(p,ptr,'=');
	l+=printlnTo(p);
	return l;
	
}
*/
bool xPL_String::operator==(const prog_char* c) const {
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
                len += p.print(xPL_String(va_arg(ap,const prog_char*)));
                break;
            default:
                len += p.print(c);
                continue;
        }
    }
    va_end(ap);
	return len;
}
