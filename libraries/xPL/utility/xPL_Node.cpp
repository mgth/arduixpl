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

#include "xPL_Node.h"
#include "xPL_Message.h"

xPL_Node* xPL_Node::nextLoop() { return (_next)?_next:(_parent?_parent->child():NULL); }
xPL_Node* xPL_Node::prev() {
	if (_parent)
	{
		xPL_Node* n = _parent->child();
		if (n==this) return NULL;
		while(n && n->next()!=this) n=next();
		return n;
	}
	return NULL;
}
xPL_Node* xPL_Node::prevLoop() { 
	if (_parent)
	{
		xPL_Node* n = _parent->child();
		while(n->nextLoop()!=this) n=nextLoop();
		return n;
	}
	return NULL;
}

xPL_Schema* xPL_Node::schema() { return (_parent)?_parent->schema():NULL; }

xPL_Node* xPL_Node::findOrAdd(const VString& cmpid)
{
	xPL_Node* n = findChild(cmpid);
	if (!n) n = add(cmpid);
	return n;
}

xPL_Node* xPL_Node::defaultNode() const
{
	if (_next) return _next->defaultNode();
	return const_cast<xPL_Node*>(this);
}
xPL_Node* xPL_NodeParent::defaultChild() const
{			
	if (_child) return _child->defaultNode();
	return NULL;
}

xPL_Node* xPL_NodeParent::addChild( xPL_Node* node)
{
	if (node)
	{ 
		node->_parent = this;
		node->_next = _child;
		_child = node;
	}
	return node;
}

xPL_Node* xPL_Node::find(const VString& id) {

	if ( is(id) ) return this;

	if (_next) return _next->find(id);

	return NULL;
}

xPL_Node* xPL_Node::findChild(const VString& cmpid)
{
	if (child())
	{
		return child()->find(cmpid);
	}
	return NULL;
}

int xPL_Node::count() const {
	int c = 0;
	xPL_Node* n = _child;
	while(n) { c++; n=n->next(); }
	return c;
}


xPL_Node::~xPL_Node() { deleteChilds(); }

void xPL_Node::deleteNode(bool deleteAll)
{
	if (deleteAll && _next) { _next->deleteNode(deleteAll); }

	if ( prev() ) { prev()->_next = _next; }
	delete this;
}

void xPL_NodeParent::deleteChilds() 
{
	xPL_Node* n = _child;
	while(n)
	{
		xPL_Node* ndel = n; n = n->_next;
		delete ndel;
	}
	_child=NULL;
}
/*
void xPL_Node::sendEvent (const xPL_Event& evt, bool childsOnly, bool all)
{
	if (all && _next) { _next->sendEvent(evt,childsOnly,all);}
	if (childsOnly || evt.send(*this))
	{ 
		if (child()) child()->sendEvent(evt,false,true);
//		evt.close(*this);
	}
}
*/
void xPL_Node::sendLoop()
{
	xPL_Node* n = child();
	while(n) { n->loop(); n=n->next(); }
}

void xPL_Node::sendParseMessage (xPL_MessageIn& msg)
{
	xPL_Node* n = child();
	while(n) {
		if (n->targeted(msg))
			n->parseMessage(msg);
		
		n=n->next();
	}
}

void xPL_Node::sendCheckTargeted (xPL_MessageIn& msg)
{
	xPL_Node* n = child();
	while(n) { n->parseMessage(msg); n=n->next(); }
}

void xPL_Node::sendConfigure (xPL_Key& key)
{
	xPL_Node* n = child();
	while(n) { n->configure(key); n=n->next(); }
}

void xPL_Node::sendStoreConfig (xPL_Eeprom& eeprom)
{
	xPL_Node* n = child();
	while(n) { n->storeConfig(eeprom); n=n->next(); }
}

void xPL_Node::sendLoadConfig (xPL_Eeprom& eeprom)
{
	xPL_Node* n = child();
	while(n) { n->loadConfig(eeprom); n=n->next(); }
}

void xPL_Node::sendInterrupt (uint8_t pin, unsigned long time)
{
	xPL_Node* n = this->child();
	while(n) { n->interrupt(pin,time); n=n->next(); }
}



/*
void xPL_Node::sendEventConst (const xPL_Event& evt, bool childsOnly, bool all) const
{
	const_cast<xPL_Node*>(this)->sendEvent(evt,childsOnly,all);
}
*/
/********************************************************************
CONFIG
********************************************************************/

xPL_Node* xPL_Node::readConfig(xPL_Eeprom& eeprom)
{
	VString id=eeprom.readString();
	if (id) return findOrAdd(id);
	return NULL;
}

void xPL_Node::loadConfig(xPL_Eeprom& eeprom)
{
	while ( xPL_Node* node = readConfig(eeprom))
	{
		node->loadConfig(eeprom);
	}
}

void xPL_Node::storeConfig(xPL_Eeprom& eeprom) { sendStoreConfig(eeprom); eeprom.write('\0'); }

