/*
  ArduixPL - xPL library for Arduino(tm)
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

	  Modified 2013-2-4 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
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
	xPL_Node* n = find(cmpid);
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
/*	if (node)
	{ 
		xPL_Node* n = child();
		if (!n) { _child=node; }
		while (n->next()) n=n->next();
		node->_parent = this;
		n->_next = node;
	}
	return node;
*/	
}

xPL_Node* xPL_Node::find(const VString& cmpid)
{
	xPL_Node* n = child();
	while (n)
	{
		if (n->is(cmpid)) return n;
		n=n->next();
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


void xPL_Node::loopChilds()
{
	xPL_Node* n = child();
	while(n) { n->loop(); n=n->next(); }
}

void xPL_Node::parseMessageChilds(xPL_MessageIn& msg)
{
	xPL_Node* n = child();
	while(n) {
		if (n->targeted(msg))
			n->parseMessage(msg);
		
		n=n->next();
	}
}



void xPL_Node::configureChilds (xPL_Key& key)
{
#ifdef XPL_CONFIG
	xPL_Node* n = child();
	while(n) { n->configure(key); n=n->next(); }
#endif
}

void xPL_Node::configChilds(xPL_Eeprom& eeprom, bool store)
{
#ifdef XPL_CONFIG
	xPL_Node* n = child();
	while(n) { n->config(eeprom,store); n=n->next(); }
#endif
}

size_t xPL_Node::printConfigChilds (Print& p,bool list)
{
	size_t len=0;
#ifdef XPL_CONFIG
	xPL_Node* n = child();
	while(n) { len+=n->printConfig(p,list); n=n->next(); }
#endif
	return len;
}

void xPL_Node::configChilds()
{
	xPL_Node* n = child();
	while(n) { n->defaultConfig(); n=n->next(); }
}
#ifdef XPL_INTERRUPTS
void xPL_Node::interruptChilds(uint8_t pin, unsigned long time)
{
	xPL_Node* n = this->child();
	while(n) { n->interrupt(pin,time); n=n->next(); }
}
#endif

