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

	  Modified 30 Oct, 2012 by Mathieu GRENET
*/
/********************************************************************Here goes all string constants, it may reduce rom size a bit, might be a betterway to handle that*/#ifndef XPL_STRINGS_H#define XPL_STRINGS_H#include "xPL_Define.h"#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))#include <avr/pgmspace.h>
//typedef char PROGMEM prog_char;
#define S(s) CStr::s_##s()typedef byte xPL_StringType;#define XPL_STRING_NONE   0x00#define XPL_STRING_RAM    0x01#define XPL_STRING_PROG   0x02#define XPL_STRING_EEPROM 0x03#define XPL_STRING_INT    0x04#define XPL_STRING_ALLOC  0x80class CStr {public:#define C(s) static PGM_P s_##s()  { return PSTR(#s); }; #define CF(f,s) static PGM_P s_##f()  { return PSTR(s); }; #include "xPL_StringList.h"};class xPL_Printable : public Printable {public:	virtual ~xPL_Printable() {}	size_t printlnTo(Print& p,char c='\n') const;	size_t printKey(Print& p,const prog_char* ptr) const;	static size_t print_prog(Print& p,const prog_char* ptr);	static size_t println_prog(Print& p,const prog_char* ptr, char ln='\n');	static size_t emit_p(Print& p, PGM_P fmt, ...);	size_t printDebug() const;};class xPL_String : public xPL_Printable {private:	typedef void (xPL_String::*bool_type)() const;    void this_type_does_not_support_comparisons() const {}protected:	xPL_StringType _type ;	char* _ptr;	bool allocated() const;	xPL_StringType type() const;public:		bool isValid() const;	operator bool_type() const;	bool readOnly() const;	void set(xPL_StringType t,char* ptr);	void setInit(xPL_StringType t,char* ptr);	void operator=(const xPL_String& s);	void operator=(int i);	void operator=(const prog_char* s);	//xPL_String(xPL_StringType t,char* ptr);	xPL_String(char* ptr);	xPL_String(const prog_char* ptr);	xPL_String(const xPL_String& s);	xPL_String(xPL_StringType t) ;	xPL_String();	xPL_String(int i);	virtual ~xPL_String();	char charAt(size_t pos) const;	const bool isAny() const;	size_t len() const;				virtual size_t printTo(Print& p) const;	char lowerCharAt(size_t& pos) const;	bool operator==(const xPL_String& s) const;	bool operator==(const prog_char* c) const;	//trick to link string to const vs volatile value	bool cmp(const xPL_String& s);	//bool operator==(const xPL_String& s) const { return cmp(s); }	bool operator!=(const xPL_String& s) const;	bool operator!=(const prog_char* s) const;	bool like(const xPL_String& s) const;	// parseTo:	// locate char c, change it by zero and affect the rest of the string to s	bool parseTo(char c,xPL_String& s);	// moveTo:	// copy the string to s having it reponsible for memory freeing.	void moveTo(xPL_String& s);	// clear: 	// invalidate string whith taking care of memory	void clear();	// load: allocate memory to copy string	bool load();	bool load(const xPL_String& s);	//float toFloat() const { return atof(_ptr); }	//int toInt() const;	bool toBool() const;};#endif