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


#ifndef XPL_MESSAGE_H
#define XPL_MESSAGE_H

#include "xPL_Node.h"

class xPL_Key: public xPL_Node {
protected:
public:
	virtual const __FlashStringHelper* className() const { return S(message); }	
	VString key;
	VString value;

	void parse(VString& buffer);

	xPL_Key(VString& buffer);
	xPL_Key(xPL_Key* key);
	xPL_Key() {};
	xPL_Key(const VString& key,const VString&  value):key(key),value(value) { }

	//properties
	virtual const VString id() const { return key; }

	bool sendEventConfigure();

	//printable
	virtual size_t printTo(Print& p) const;

};

// xPL_ListId : provide a way to print coma separated Id list of a node's childs
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
				len += n->id().printTo(*print);
				return false;
			} 
		} evt ;

		evt.print=&p;
		evt.len=0;

		_node->sendEventConst(&evt,true);

		return evt.len;
	}
};

class xPL_Address: public Printable {
public:

	VString vendor;
	VString device;
	VString instance;

	xPL_Address() {}
	void parse(const VString& s);

	virtual size_t printTo(Print& p) const;

	void setAny() { instance=S(_asterisk); vendor.clear(); device.clear(); }
	bool isAny() { return instance.charAt(0)=='*'; } 

	bool operator==(const xPL_Address& addr);
};



class xPL_Message : public Printable  {
protected:	xPL_Node* _node;public:
	xPL_Message() {};
	xPL_Message(xPL_Node& node);

	static size_t printKeyTo(Print& p, const VString& key);
	static size_t printKeyTo(Print& p, const VString& key,const VString& value);
	static size_t printKeyTo(Print& p, const VString& key,const Printable& value);
	static size_t printKeyTo(Print& p, const VString& key,int value);
	static size_t printKeyTo(Print& p, const VString& key,float value,int dec=2);
	static size_t printOptionKeyTo(Print& p, const __FlashStringHelper* value);
	static size_t printOptionKeyTo(Print& p, const __FlashStringHelper* value,int index);

	bool send();

	virtual size_t printTo(Print& p) const;
	virtual size_t printContentTo(Print& p) const;
	virtual size_t printTargetTo(Print& p) const { return p.print('*'); }
	virtual const __FlashStringHelper* msgType() const { return S(stat); }	virtual const __FlashStringHelper* schClass() const;	virtual const __FlashStringHelper* schType() const { return S(basic); }};


class xPL_MessageIn : public xPL_NodeParent {
private:
	VString _message;
	VString _input;
	struct
	{
		bool targeted : 1;
		bool matchFilter : 1 ;
		bool contentParsed : 1;
	} _state;

public:
	xPL_Address msgType;
	byte hop;
	xPL_Address source;
	xPL_Address target;
	xPL_Address schema;

	xPL_MessageIn(VString& buffer);

	void setTargeted(bool b=true )  { _state.targeted=b; }
	void setMatchFilter(bool b=true){ _state.matchFilter=b; }

	bool targeted(){ return _state.targeted && _state.matchFilter; }

	xPL_Key* getKey(const __FlashStringHelper* name);
	VString getValue(const __FlashStringHelper* name);
	xPL_Key* getKeyCopy(const __FlashStringHelper* name);


	bool parseHeader();
	bool parseContent();

	bool parse();

	bool isTypeCommand (const __FlashStringHelper* type,const __FlashStringHelper* cmd );

#undef C
#undef CF
#define C(s) const VString key_##s()  { return getValue(S(s)); }
#define CF(f,s) const VString key_##f()  { return getValue(S(f)); }
#include "xPL_Stringlist.h"
};

#endif