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

#ifndef XPL_MESSAGE_H
#define XPL_MESSAGE_H

#include "xPL_Node.h"

class xPL_Key: public xPL_Node {
private:
	xPL_String _key;
	const xPL_Printable* _value;
	bool _allocated;

protected:
public:

	void parse(xPL_String& buffer);

	xPL_Key(xPL_String& buffer);
	xPL_Key(xPL_Key* key);
	xPL_Key();
	xPL_Key(const xPL_String& key,const xPL_Printable* value, bool alloc=false) {
		_key   = key;
		_value = value;
		if (_value && alloc) _allocated=true;
		else _allocated = false;
	}
	xPL_Key(const xPL_String& key, const prog_char* value) { //TODO : -> xPL_string
		_key   = key;
		_value = new xPL_String(value);
		if (_value)
			_allocated=true;
		else
			_allocated = false;
	}

	virtual ~xPL_Key();

	//properties
	xPL_String& key() { return _key; }
	virtual const xPL_String* id() const { return &_key; }
	const xPL_Printable* value() { return _value; }
	xPL_String& sValue() { return *(xPL_String*)_value; }

	//operators
	bool operator==(xPL_String s) { return _key == s; }
	bool operator!=(xPL_String s) { return _key != s; }
	bool operator==(xPL_String &s) { return _key == s; }
	bool operator!=(xPL_String &s) { return _key != s; }
	bool operator==(const prog_char* s) { return _key == s; }
	bool operator!=(const prog_char* s) { return _key != s; }


	bool sendEventConfigure();

	//printable
	virtual size_t printTo(Print& p) const;

};

// xPL_ListId : provide a way to print coma separeted Id list of a node's childs
class xPL_ListId: public xPL_Printable {
private:
	const xPL_Node* _node;
public:
	xPL_ListId(const xPL_Node* n) { _node = n; }
	virtual size_t printTo(Print& p) const {

		class :public xPL_Event {
		public:
			Print* print;
			size_t len;

			virtual bool send(xPL_Node* n) {
				if (n->next()) len += print->print(',');
				if (n->id()) len += n->id()->printTo(*print);
				return false;
			} 
		} evt;

		evt.print=&p;
		evt.len=0;

		const_cast<xPL_Node*>(_node)->sendEvent(&evt,true);

		return evt.len;
	}
};

class xPL_Address: public xPL_Printable {
public:
	xPL_String vendor;
	xPL_String device;
	xPL_String instance;

	xPL_Address() {}
	void parse(const xPL_String& s);

	virtual size_t printTo(Print& p) const;


	xPL_String& schClass() { return device; }
	xPL_String& schType() { return instance; }

	xPL_String& msgVendor() { return vendor; }
	xPL_String& msgType() { return instance; }

	const xPL_String* id() const { return &instance; }

	void setAny() { instance=S(_asterisk); vendor.clear(); device.clear(); }
	bool isAny() { return instance.isAny(); } 

	bool operator==(const xPL_Address& addr);
};

class xPL_String_Index : public xPL_Printable {
private:
	const prog_char* _key;
	xPL_String _index;
public:
	virtual size_t printTo(Print& p) const;

	xPL_String_Index(const prog_char* key, const xPL_String& index)
	{
		_key=key;
		_index = index;
	}

	xPL_String_Index(const prog_char* key, int index)
	{
		_key=key;
		_index = index;
	}

};

class xPL_Message : public xPL_Node {
private:
	xPL_String _message;
	struct
	{
		bool targeted : 1;
		bool matchFilter : 1 ;
		bool contentParsed : 1;
	} _state;
/*
	struct
	{
		bool targeted;
		bool matchFilter;
		bool contentParsed;
	} _state;
*/
public:
	xPL_Message();
	xPL_Message(const prog_char*  msgType, const prog_char* schClass, const prog_char* schType);

	virtual ~xPL_Message();

	xPL_Address msgType;
	byte hop;
	xPL_Address source;
	xPL_Address target;
	xPL_Address schema;

	void setTargeted(bool b=true ) { _state.targeted=b; }
	void setMatchFilter(bool b=true){ _state.matchFilter=b; }

	bool targeted(){ return _state.targeted && _state.matchFilter; }

	xPL_Key* getKey(const prog_char* name);
	xPL_String getValue(const prog_char* name);
	xPL_Key* getKeyCopy(const prog_char* name);

	void addKey(const prog_char* key,const xPL_Printable* value, bool alloc=false);
	void addKey(const prog_char* key,const prog_char* value);
	void addOptionKey(const prog_char* value);
	void addOptionKey(const prog_char* value,int index);

	bool parseHeader(char* buffer);
	bool parseContent();

	bool parse(char* buffer);

	virtual size_t printTo(Print& p) const;

	bool send(bool del=false);

	bool isTypeCommand (const prog_char* type,const prog_char* cmd );

#undef C
#undef CF
#define C(s) const xPL_String key_##s()  { return getValue(S(s)); }
#define CF(f,s) const xPL_String key_##f()  { return getValue(S(f)); }
#include "xPL_Stringlist.h"
};

#endif