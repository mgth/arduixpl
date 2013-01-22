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
	id = buffer;
	buffer = id.parseTo('\n');
	value = id.parseTo('=');
}

size_t xPL_Key::printTo(Print& p) const {

	size_t len = 0;

	len += id.printlnNzTo(p,'=');
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
xPL_Message::xPL_Message(xPL_Node& node) { _node = &node; }

const __FlashStringHelper*  xPL_Message::schClass() const {
	if (_node && _node->schema() )
		return _node->schema()->className();
	return S(message);
}

size_t xPL_Message::printKeyTo(Print&p, const VString& key) {
	return key.printlnNzTo(p,'=');
}
size_t xPL_Message::printKeyTo(Print&p, const VString& key,float value,int dec) {
	return
	printKeyTo(p,key) +
	p.print(value,dec) +
	p.print('\n');
}

size_t xPL_Message::printKeyTo(Print&p, const VString& key,int value) {
	return
	printKeyTo(p,key) +
	p.print(value) +
	p.print('\n');
}
size_t xPL_Message::printKeyTo(Print&p, const VString& key,const Printable& value) {
	return
	printKeyTo(p,key) +
	p.print(value) +
	p.print('\n');
}

size_t xPL_Message::printKeyTo(Print&p, const VString& key,const VString& value) {
	return
	printKeyTo(p,key) +
	value.printlnTo(p);
}

size_t xPL_Message::printOptionKeyTo(Print&p, const __FlashStringHelper* value) {
	return printKeyTo(p,S(option),value);
}

size_t xPL_Message::printOptionKeyTo(Print&p, const __FlashStringHelper* value,int index)
{
	return
	printKeyTo(p,S(option)) +
	VString(value).printlnTo(p,'[') +
	p.print(index) +
	p.print(F("]\n"));
}

size_t xPL_Message::printTo(Print& p) const //710
{
  int len=0;

 len += p.print(F("xpl-"));
 len += p.print(msgType());
 len += p.print(F("\n{\nhop=1\n"));
// len += printKey(p,S(hop),hop);
 len += printKeyTo(p,S(source)); len += xPL.source().printTo(p); len += p.print('\n');
 len += printKeyTo(p,S(target)); len += printTargetTo(p);
// len += printKey(p,S(target),target);
 len += p.print(F("\n}\n"));
 len += p.print(schClass());
 len += p.print('.');
 len += p.print(schType());
 len += p.print(F("\n{\n"));
 len += printContentTo(p);
 //len += p.print(F("}"));
 len += p.print(F("}\n")); // domogik and xplhal manager won't receive messages that do not end with \n

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


size_t xPL_Message::send(){
	return xPL.sendMessage(*this);
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
		return (xPL_Key*)_keys.findChild(name);
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
		if(k)
		{
			k->id = name;
			k->value = getValue(name);
				k->value.load();
		}
		return k;
}



bool xPL_MessageIn::parseHeader() {
     enum {
	  END, ERROR, MSG_TYPE, SCHEMA_NAME, START_SECTION, IN_SECTION
     } state = MSG_TYPE;

	xPL_Key k;

	do
	{
		k.parse(_input);
		if (!k.id) { state=ERROR; }
		char c = k.id.charAt(0);

		switch(state)
		{
			case MSG_TYPE:
				msgType.parse(k.id); 
				if (msgType.vendor == S(xpl))
					state=START_SECTION;
				else
					state=ERROR;
				break;
			case SCHEMA_NAME:
				schema.parse(k.id); 
				state=END;
				break;
			case START_SECTION:
				if (c=='{') state=IN_SECTION;
				break;
			case IN_SECTION:
				if (c=='}') { state=SCHEMA_NAME; }
				else
				{
						if ( k.id == S(hop) ) {
							hop=xPL_Int(k.value);
						}
						else if ( k.id == S(source) ) {
							source.parse(k.value);
						}
						else if ( k.id == S(target) ) {
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
	xPL_Key k;

	if (_state.contentParsed) return true; //14

     enum {
	  END, ERROR, START_SECTION, IN_SECTION
     } state = START_SECTION;
	
	do 
	{
		k.parse(_input); //278

		if (!k.id) { state=ERROR; } //12

		char c = k.id.charAt(0);

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
					if (!_keys.addChild(new xPL_Key(k))) state=ERROR;
				}
				break;
			case END: break;
			case ERROR: break;
		}

	} while(state>ERROR);

	_state.contentParsed=true;
	return state==END;
}

bool xPL_MessageIn::parse() { 
	return (parseHeader() && parseContent());
}

