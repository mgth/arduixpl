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
#ifndef XPL_MESSAGE_H
#define XPL_MESSAGE_H

#include "xPL_Node.h"

class xPL_Key: public xPL_Node {
protected:
public:
//	virtual const __FlashStringHelper* className() const { return S(message); }	
	VString id;
	VString value;
	virtual bool is(const VString& cmpid) { return cmpid==id; }

	void parse(VString& buffer);

	xPL_Key(VString& buffer);
	xPL_Key(xPL_Key* key);
	xPL_Key() {};
	xPL_Key(const VString& key,const VString&  val):id(key),value(val) {}

	size_t printTo(Print& p) const;
};


class xPL_Address
{
public:

	VString vendor;
	VString device;
	VString instance;

	xPL_Address() {}
	xPL_Address(const VString& v,const VString& d,const VString& i)
		:vendor(v),device(d),instance(i){}
	void parse(const VString& s);

	void setAny() { instance=S(_asterisk); vendor.clear(); device.clear(); }
	bool isAny() { return instance.charAt(0)=='*'; } 

	bool operator==(const xPL_Address& addr);

	size_t printTo(Print& p) const;
};



class xPL_Message 
{
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

	size_t send();

	size_t printTo(Print& p) const;
	size_t len() { nullPrinter p; return printTo(p); }

	virtual size_t printContentTo(Print& p) const;
	virtual size_t printTargetTo(Print& p) const { return p.print('*'); }
	virtual const __FlashStringHelper* msgType() const { return S(stat); }	virtual const __FlashStringHelper* schClass() const;	virtual const __FlashStringHelper* schType() const { return S(basic); }};


class xPL_MessageIn {
private:
	VString _message;
	VString _input;
	xPL_Node _keys;

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

	bool targeted() const { return _state.targeted && _state.matchFilter; }

	xPL_Key* getKey(const __FlashStringHelper* name);
	VString getValue(const __FlashStringHelper* name);
	xPL_Key* getKeyCopy(const __FlashStringHelper* name);

	xPL_Key* keys() { return (xPL_Key*)_keys.child(); }

	bool parseHeader();
	bool parseContent();

	bool parse();

	bool isTypeCommand (const __FlashStringHelper* type,const __FlashStringHelper* cmd );

#undef C
#undef CF
#define C(s) const VString key_##s() { return getValue(S(s)); }
#define CF(f,s) const VString key_##f() { return getValue(S(f)); }
#include "xPL_Stringlist.h"
};

#endif