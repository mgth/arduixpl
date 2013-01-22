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
#ifndef XPL_NODE_H
#define XPL_NODE_H

#include "xPL_Define.h"

#include "VString.h"
#include "xPL_String.h"
#include "xPL_BufferFiller.h"
#include "xPL_Eeprom.h"
#include "xPL_Event.h"

class xPL_Main;
class xPL_Key;
class xPL_Message;
class xPL_MessageIn;
class xPL_Node;
class xPL_Schema;

#define xPL_NodeParent xPL_Node

#ifndef xPL_NodeParent
class xPL_NodeParent;
#endif

class xPL_Interrupt
{
	byte _pin;
	unsigned long _time;
public:
	xPL_Interrupt(byte pin, unsigned long time):_pin(pin),_time(time) {}
	byte pin() {return _pin;}
	unsigned long time() {return _time; }
};


class xPL_Node {
protected: 
	xPL_NodeParent* _parent;
	xPL_Node* _next;
#ifdef xPL_NodeParent
	xPL_Node* _child;
#else
	friend class xPL_NodeParent;
#endif


	virtual bool targeted(xPL_MessageIn& msg) { return false; }


public:

// Properties
	virtual const __FlashStringHelper* className() const { return S(xpl); }


// Constructors
	virtual ~xPL_Node();

// Tree
	xPL_NodeParent* parent() const { return _parent; }
	xPL_Node* next() { return _next; }
	xPL_Node* nextLoop();
	xPL_Node* prev();
	xPL_Node* prevLoop();
	virtual xPL_Schema* schema();

	virtual xPL_Node* add(const VString& newid) { return NULL; }

#ifndef xPL_NodeParent
	xPL_Node():_parent(NULL),_next(NULL) {}
	virtual xPL_Node* child() { return NULL; }
	virtual xPL_Node* addChild(xPL_Node* node) { return NULL; }
	virtual void deleteChilds() {};
	virtual xPL_Node* defaultChild() const { return NULL; }
#else
	xPL_Node():_parent(NULL),_next(NULL),_child(NULL) {}
	xPL_Node* child() { return _child; }
	xPL_Node* addChild(xPL_Node* node);
	void deleteChilds();
	xPL_Node* defaultChild() const;
#endif

	xPL_Node* findOrAdd(const VString& cmpid);

	void deleteNode(bool deleteAll=false);

	virtual bool is(const VString& id) { return false; }
	xPL_Node* find(const VString& id);

	xPL_Node* defaultNode() const;

	int count() const;


	void loopChilds();
	virtual void loop(){ loopChilds(); }

	void parseMessageChilds(xPL_MessageIn& msg);
	void configureChilds(xPL_Key& key);
	void configChilds(xPL_Eeprom& eeprom, bool store=false);
	size_t printConfigChilds(Print& p,bool list=false);
	virtual void parseMessage(xPL_MessageIn& msg){ parseMessageChilds(msg); }

#ifdef XPL_CONFIG
	virtual void configure(xPL_Key& key){ return configureChilds(key); }
	virtual void config(xPL_Eeprom& eeprom, bool store=false){ configChilds(eeprom,store); }
	virtual size_t printConfig(Print& p,bool list=false){ return printConfigChilds(p,list); }
#endif
	void configChilds();
	virtual void defaultConfig() { configChilds(); }
#ifdef XPL_INTERRUPTS
	void interruptChilds(uint8_t pin, unsigned long time);
	virtual void interrupt(uint8_t pin, unsigned long time){ interruptChilds(pin,time); }
#endif

};

#ifndef xPL_NodeParent

class xPL_NodeParent: public xPL_Node {

private:
	xPL_Node* _child;
public:
	xPL_NodeParent():_child(NULL) {};
	virtual xPL_Node* child() { return _child; }
	virtual void deleteChilds();
	virtual xPL_Node* addChild(xPL_Node* node);
	virtual xPL_Node* defaultChild() const;


};
#endif


#endif
