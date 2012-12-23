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

/********************************************************************Here goes all string constants, it may reduce rom size a bit, might be a betterway to handle that*/#ifndef xPL_StringS_H#define xPL_StringS_H#include "xPL_Define.h"#include <avr/pgmspace.h>

#if !defined(pgm_get_far_address)
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))#endif#define S(s) CStr::s_##s()typedef enum {none,ram,prog,eeprom,printable,ethernet} xPL_StringType;class CStr {public:#define C(s) static const __FlashStringHelper* s_##s()  { return F(#s); }; #define CF(f,s) static const __FlashStringHelper* s_##f()  { return F(s); }; #include "xPL_StringList.h"};class xPL_Printable : public Printable {public:	virtual ~xPL_Printable() {}	size_t printlnTo(Print& p,char c='\n') const;	static size_t emit_p(Print& p, PGM_P fmt, ...);	size_t printDebug() const;};class xPL_StringHelper {	virtual char charAt(size_t pos) =0;	virtual void setChar(size_t pos, char c) =0;};class xPL_String : public xPL_Printable {private:	typedef void (xPL_String::*bool_type)() const;    void this_type_does_not_support_comparisons() const {}protected:	 struct {
		 xPL_StringType type :3; 
		bool allocated :1;
		bool linkable :1;
	} _state;	void* _ptr;public://	bool allocated() const;//	xPL_StringType type() const;	static xPL_StringHelper* ethernetHelper;	bool isValid() const;	operator bool_type() const;	bool readOnly() const;	void set(xPL_StringType t,void* ptr);	void setInit(xPL_StringType t,void* ptr);	void operator=(const xPL_String& s);	void operator=(const __FlashStringHelper* s);	xPL_String(char* ptr);	xPL_String(const __FlashStringHelper* ptr);	xPL_String(const xPL_String& s);	xPL_String(const xPL_Printable& p);	xPL_String(int i);	xPL_String(float f);	xPL_String(xPL_StringType t) ;	xPL_String();	virtual ~xPL_String();	char charAt(size_t pos) const;	const bool isAny() const;	size_t len() const;				virtual size_t printTo(Print& p) const;	size_t printNzTo(Print& p) const { return (*this)?printTo(p):0; }	size_t printlnNzTo(Print& p, char ln='\n') const { return (*this)?printlnTo(p,ln):0; }	char lowerCharAt(size_t& pos) const;	bool operator==(const xPL_String& s) const;	bool operator==(const __FlashStringHelper* c) const;	//trick to link string to const vs volatile value	bool cmp(const xPL_String& s);	bool operator!=(const xPL_String& s) const;	bool operator!=(const __FlashStringHelper* s) const;	bool like(const xPL_String& s) const;	// parseTo:	// locate char c, change it by zero and affect the rest of the string to s	char* parseTo(char c);	// moveTo:	// copy the string to s having it reponsible for memory freeing.	void moveTo(xPL_String& s);	// clear: 	// invalidate string whith taking care of memory	void clear();	// load: allocate memory to copy string	size_t load();	size_t load(const xPL_String& s);	bool toBool() const;};#endif