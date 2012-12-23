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

#include "xPL_Message.h"
#include "vstring.h"
#include "xPL_Adapter.h"

#include "xPL_Numeric.h"

xPL_Key::xPL_Key(VString&  buffer)
{
	parse(buffer);
}

void xPL_Key::parse(VString& buffer)
{
	key = buffer;
	buffer = key.parseTo('\n');
	value = key.parseTo('=');
}

bool xPL_Key::sendEventConfigure() {
		class :public xPL_Event {
		public:
			xPL_Key* key;
			virtual bool send(xPL_Node* n) { return n->configure(*key); } 
		} evt;

		evt.key = this;

		DBG(F("parse:"),*this);

		xPL.sendEvent(&evt);
		return false;
}

size_t xPL_Key::printTo(Print& p) const {

	size_t len = 0;

	len += key.printlnNzTo(p,'=');
	len += value.printNzTo(p);

	return len;
}

size_t xPL_Address::printTo(Print& p) const {

	size_t len = 0;

	len += vendor.printlnNzTo(p,'-');
	len += device.printlnNzTo(p,'.');
	len += instance.printTo(p);
	
	return len;
}

size_t xPL_String_Index::printTo(Print& p) const {
	return xPL_Printable::emit_p(p, PSTR("&[$]"),_key,&_index);
}

void xPL_Address::parse(const VString& s) {
	vendor = s;
	instance = vendor.parseTo('.');
	if (!instance) {
		instance = vendor.parseTo('-'); device.clear();
		if (!instance) { instance = vendor; vendor.clear(); }
	}
	else {
		device = vendor.parseTo('-');
		if (!device) { device=vendor; vendor.clear(); }	
	}
}

bool xPL_Address::operator==(const xPL_Address& addr) {
	if ( instance != addr.instance ) return false;
	if ( device != addr.device ) return false;
	if ( vendor != addr.vendor ) return false;
	return true;
}

/********************************************************************
xPL_Message
********************************************************************/
xPL_Message::xPL_Message(const __FlashStringHelper* mType,const __FlashStringHelper* sClass,const __FlashStringHelper* sType)
{
	msgType.vendor = S(xpl);
	msgType.instance = mType;

	hop=1;
	
	source = xPL.source();
	
	target.setAny();
	
	schema.device = sClass;
	schema.instance = sType;
}

size_t xPL_Message::printKey(Print&p, const VString& key,int value) {
	return
	key.printlnNzTo(p,'=') +
	p.print(value) +
	p.print('\n');
}
size_t xPL_Message::printKey(Print&p, const VString& key,const Printable& value) {
	return
	key.printlnNzTo(p,'=') +
	p.print(value) +
	p.print('\n');
}

size_t xPL_Message::printKey(Print&p, const VString& key,const VString& value) {
	return
	key.printlnNzTo(p,'=') +
	value.printlnNzTo(p);
}

size_t xPL_Message::printOptionKey(Print&p, const __FlashStringHelper* value) {
	return printKey(p,S(option),value);
}

size_t xPL_Message::printOptionKey(Print&p, const __FlashStringHelper* value,int index)
{
	return
	VString(S(option)).printlnTo(p,'=') +
	VString(value).printlnTo(p,'[') +
	p.print(index) +
	p.print(F("]\n"));
}

size_t xPL_Message::printTo(Print& p) const //710
{
  int len=0;

 len += p.print(msgType);
 len += p.print(F("\n{\n"));
 len += printKey(p,S(hop),hop);
 len += printKey(p,S(source),source);
 len += printKey(p,S(target),target);
 len += p.print(F("}\n"));
 len += p.print(schema);
 len += p.print(F("\n{\n"));
 len += printContentTo(p);
 len += p.print(F("}"));

 return len;
/*
xPL_ChildsPrinter content(*this);

 xPL_Int h(hop);

 return xPL_Printable::emit_p(p,
		PSTR(
		"$\n"
		"{\n"
		"&=$\n"
		"&=$\n"
		"&=$\n"
		"}\n"
		"$\n"
		"{\n"
		"$"
		"}\n"
		),
		&msgType,
		S(hop),&h,
		S(source),&source,
		S(target),&target,
		&schema,
		&content
		);
*/		
}

size_t xPL_Message::printContentTo(Print& p) const {	return 0;	//	return xPL_ChildsPrinter(*this).printTo(p);}


bool xPL_Message::send(bool del){
	bool r = xPL.sendMessage(*this);
	if ( del ) { DELETE(this); }
	return r;
}

/********************************************************************
xPL_MessageIn
********************************************************************/
xPL_MessageIn::xPL_MessageIn(VString& buffer)
{
	_message = buffer;
	_input = buffer;

	_state.targeted = false;
	_state.matchFilter = true;
	_state.contentParsed = false;
}


bool xPL_MessageIn::isTypeCommand (const __FlashStringHelper* type,const __FlashStringHelper* cmd )
{
	return (schema.instance==type && key_command()==cmd);
}

xPL_Key* xPL_MessageIn::getKey(const __FlashStringHelper* name)
{
	if (parseContent())
	{
		return (xPL_Key*)findChild(name);
	}
	return NULL;
}

VString xPL_MessageIn::getValue(const __FlashStringHelper* s)
{
	VString r;
	xPL_Key*k = getKey(s);
	if(k) r = k->value;
	return r;
}

xPL_Key* xPL_MessageIn::getKeyCopy(const __FlashStringHelper* name)
{
		xPL_Key* k= new xPL_Key();
		k->key = name;
		if (_parent)
		{
			k->value = ((xPL_MessageIn*)_parent)->getValue(name);
			k->value.load();
		}
		return k;
}



bool xPL_MessageIn::parseHeader() {
     enum {
	  END, ERROR, MSG_TYPE, SCHEMA_NAME, START_SECTION, IN_SECTION
     } state = MSG_TYPE;

//	_state.contentParsed = true;

	xPL_Key k;

	do
	{
		k.parse(_input);
		if (!k.key) { state=ERROR; }
		char c = k.key.charAt(0);

		switch(state)
		{
			case MSG_TYPE:
				msgType.parse(k.key); 
				if (msgType.vendor == S(xpl))
					state=START_SECTION;
				else
					state=ERROR;
				break;
			case SCHEMA_NAME:
				schema.parse(k.key); 
				state=END;
				break;
			case START_SECTION:
				if (c=='{') state=IN_SECTION;
				break;
			case IN_SECTION:
				if (c=='}') { state=SCHEMA_NAME; }
				else
				{
						if ( k.key == S(hop) ) {
							hop=xPL_Int(k.value);
						}
						else if ( k.key == S(source) ) {
							source.parse(k.value);
						}
						else if ( k.key == S(target) ) {
							target.parse(k.value);
						}
				}
				break;
			case END:
			case ERROR:
				break;
		}
	} while(state>ERROR);

	return state==END;
}

bool xPL_MessageIn::parseContent() //390
{
	if (_state.contentParsed) return true; //14

     enum {
	  END, ERROR, START_SECTION, IN_SECTION
     } state = START_SECTION;

	xPL_Key* k;
	
	do 
	{
		k = new xPL_Key(_input); //278
		if (!k) { state=ERROR; } //12

		char c = k->key.charAt(0);

		switch(state)
		{
			case START_SECTION:
				if (c=='{') state=IN_SECTION; //110
				else state=ERROR;
				break;
			case IN_SECTION:
				if (c=='}') { //56
					state=END;
				}
				else {
					addChild(k);
					k=NULL;
				}
				break;
			case END: break;
			case ERROR: break;
		}

		if (k) { DELETE(k); } //30

	} while(state>ERROR);

	_state.contentParsed=true;
	return state==END;
}

bool xPL_MessageIn::parse() { 
	return (parseHeader() && parseContent());
}
/*
size_t xPL_MessageIn::printTo(Print& p) const
{
	if (_message) return p.print(_message);
	return 0;
}
*/