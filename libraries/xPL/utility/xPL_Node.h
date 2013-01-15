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

#ifndef XPL_NODE_H
#define XPL_NODE_H

#include "xPL_Define.h"

#include "VString.h"
#include "xPL_String.h"
#include "xPL_BufferFiller.h"
#include "xPL_Eeprom.h"
//#include "xPL_Event.h"
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

class xPL_Node : public Printable {
protected: 
	xPL_NodeParent* _parent;
	xPL_Node* _next;
#ifdef xPL_NodeParent
	xPL_Node* _child;
#else
	friend class xPL_NodeParent;
#endif


	virtual bool targeted(xPL_MessageIn& msg) const { return false; }


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
	xPL_Node* findChild(const VString& id);

	xPL_Node* defaultNode() const;

	int count() const;

// events
/*
	void sendEvent(const xPL_Event& evt,bool childsOnly=false, bool all=false);
	void sendEventConst(const xPL_Event& evt,bool childsOnly=false, bool all=false) const;

	template<class cls> void sendEvent( bool(cls::*func)(),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunction<cls> evt(func);
		return sendEvent(evt,childsOnly,all);
	}
	template<class cls,class clsparam> void sendEvent( bool(cls::*func)(clsparam&),clsparam& param, bool childsOnly=false, bool all=false)
	{
		xPL_EventFunctionParam<cls,clsparam> evt(func,param);
		return sendEvent(evt,childsOnly,all);
	}

	template<class cls> void sendEvent( bool(cls::*func)(),bool(cls::*funcClose)(),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunction<cls> evt(func,funcClose);
		return sendEvent(evt,childsOnly,all);
	}

	template<class cls,class clsparam> void sendEvent( bool(cls::*func)(clsparam&),clsparam& param,bool(cls::*funcClose)(clsparam&),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunctionParam<cls,clsparam> evt(func,funcClose);
		return sendEvent(evt,childsOnly,all);
	}



	template<class cls> void sendEventConst( bool(cls::*func)(),bool childsOnly=false, bool all=false) const
	{
		xPL_EventFunction<cls> evt(func);
		return sendEventConst(evt,childsOnly,all);
	}
*/


	void sendLoop();
	virtual void loop(){ sendLoop(); }

	void sendParseMessage (xPL_MessageIn& msg);
	virtual void parseMessage(xPL_MessageIn& msg) { }

	void sendCheckTargeted (xPL_MessageIn& msg);
	virtual void checkTargeted(xPL_MessageIn& msg) { }

	void sendInterrupt (uint8_t pin, unsigned long time);
	virtual void interrupt(uint8_t pin, unsigned long time) { sendInterrupt(pin,time); }

	void msgAddKey(const VString& key,const VString& value, bool alloc=false) const ;

//Config
	virtual xPL_Node* readConfig(xPL_Eeprom& eeprom);

	void sendLoadConfig (xPL_Eeprom& eeprom);
	virtual void loadConfig(xPL_Eeprom& eeprom);

	void sendStoreConfig (xPL_Eeprom& eeprom);
	virtual void storeConfig(xPL_Eeprom& eeprom);

	void sendConfigure (xPL_Key& key);
	virtual void configure(xPL_Key& key) { }

	virtual size_t printConfigList(Print& p) { return 0; }
	virtual size_t printConfigCurrent(Print& p) { return 0; }
//Printable
	virtual size_t printTo(Print& p) const {
			return 0;
	}

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
