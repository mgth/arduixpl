#include "xPL_Node.h"
#include "xPL_Message.h"

//xPL_Message xPL_Node::msgIn;
//xPL_Message xPL_Node::msgOut;
//xPL_EepromParser xPL_Node::eeprom;


xPL_Node* xPL_Node::next() const { return _next; }
xPL_Node* xPL_Node::child() const { return _child; }
xPL_Node* xPL_Node::parent() const { return _parent; }

xPL_Node* xPL_Node::findOrAdd(const xPL_String& id)
{
	xPL_Node* n = findChild(id);
	if (!n) n = add(id);
	return n;
}

xPL_Node* xPL_Node::defaultNode() const
{
	if (next()) return next()->defaultNode();
	return const_cast<xPL_Node*>(this);
}
xPL_Node* xPL_Node::defaultChild() const
{			
	if (child()) return child()->defaultNode();
	return NULL;
}

xPL_Node* xPL_Node::addChild( xPL_Node* node)
{
	if (node)
	{ 
		node->_parent = this;
		node->_next = _child;
		_child = node;
	}
	return node;

}
xPL_Node* xPL_Node::find(const xPL_String& cmpid) {
		
	if ( id() && (*id() == cmpid) ) return this;
	if (next()) return next()->find(cmpid);
	return NULL;
}

xPL_Node* xPL_Node::findChild(const xPL_String& id) { if (child()) return child()->find(id); return NULL; }

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

void xPL_Node::deleteNode(xPL_Node* previous, bool deleteAll)
{
	deleteChilds(); 

	if (deleteAll && next()) { next()->deleteNode(this,deleteAll); }
	previous->_next = _next;

	DELETE(this);
}

void xPL_Node::deleteChilds() { if (child()) { child()->deleteNode(this,true); } }


void xPL_Node::sendEvent ( xPL_Event* evt, bool childsOnly, bool all)
{
	if (all && next()) { next()->sendEvent(evt,childsOnly,all);}
	if (childsOnly || evt->send(this))
	{ 
		if (child()) child()->sendEvent(evt,false,true);
		evt->close(this);
	}
}

/********************************************************************
CONFIG
********************************************************************/

xPL_Node* xPL_Node::readConfig(xPL_Eeprom& eeprom)
{
	xPL_String id=eeprom.readString();
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
