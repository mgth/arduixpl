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

xPL_Node* xPL_Node::findOrAdd(const VString& id)
{
	xPL_Node* n = findChild(id);
	if (!n) n = add(id);
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

xPL_Node* xPL_Node::find(const VString& cmpid) {

	if ( id() && (*id() == cmpid) ) return this;
	if (_next) return _next->find(cmpid);
	return NULL;
}

xPL_Node* xPL_Node::findChild(const VString& id) { if (child()) return child()->find(id); return NULL; }

int xPL_Node::count() const {
	class :public xPL_Event {
	public: 
		int count;
		virtual bool send(xPL_Node* n) { count++; return false; }
	} evt;

	evt.count=0;
	const_cast<xPL_Node*>(this)->sendEvent(&evt,true);
	return evt.count;
}


xPL_Node::~xPL_Node() { deleteChilds(); }

void xPL_Node::deleteNode(bool deleteAll)
{
	if (deleteAll && _next) {
		_next->deleteNode(deleteAll);
	}
	// TODO, adjust next of previous item.
	DELETE(this);
}

void xPL_NodeParent::deleteChilds() 
{
	xPL_Node* n = _child;
	while(n)
	{
		xPL_Node* ndel = n; n = n->_next;
		DELETE(ndel);
	}
	_child=NULL;
}

void xPL_Node::sendEvent ( xPL_Event* evt, bool childsOnly, bool all)
{
	if (all && _next) { _next->sendEvent(evt,childsOnly,all);}
	if (childsOnly || evt->send(this))
	{ 
		if (child()) child()->sendEvent(evt,false,true);
		evt->close(this);
	}
}
void xPL_Node::sendEventConst ( xPL_Event* evt, bool childsOnly, bool all) const
{
	const_cast<xPL_Node*>(this)->sendEvent(evt,childsOnly,all);
}

/********************************************************************
CONFIG
********************************************************************/

xPL_Node* xPL_Node::readConfig(xPL_Eeprom& eeprom)
{
	VString id=eeprom.readString();
	if (id) return findOrAdd(id);
	return NULL;
}

bool xPL_Node::loadConfig(xPL_Eeprom& eeprom)
{
	while ( xPL_Node* node = readConfig(eeprom))
	{
		node->sendEvent(&xPL_Node::loadConfig,eeprom);
	}
	return false;
}

bool xPL_Node::storeConfig(xPL_Eeprom& eeprom) { id()->printlnTo(eeprom,'\0'); return true;}

void xPL_Node::storeConfigClose(xPL_Eeprom& eeprom) { eeprom.write('\0'); }
