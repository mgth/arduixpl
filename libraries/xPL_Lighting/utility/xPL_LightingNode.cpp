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

	  Modified 20 Mars 2012 by Mathieu GRENET
*/

#include "xPL_LightingNode.h"
#include "xPL_LightingMessage.h"


xPL_LightingNode::xPL_LightingNode() {  }
xPL_LightingNode::xPL_LightingNode(const xPL_String& id) { _id.load(id); }

#ifdef XPL_LIGHTING_CONFIG
bool xPL_LightingNode::msgAddConfigCurrent(xPL_Message &msg) { return true; }
#endif

const prog_char* xPL_LightingNode::className() const { return NULL;}
const xPL_String* xPL_LightingNode::id() const { return &_id; }
xPL_String xPL_LightingNode::channelId() { return S(_zero); }

bool xPL_LightingNode::parseMessage(xPL_LightingMessage& msg) { return false; }
bool xPL_LightingNode::parseMessage(xPL_Message& msg) { return parseMessage(*(xPL_LightingMessage*)&msg); }

xPL_LightingNode* xPL_LightingNode::parent() const { return (xPL_LightingNode*)xPL_Node::parent(); }

xPL_LightingNetwork* xPL_LightingNode::network()  const { return (parent())?parent()->network():NULL; }
xPL_LightingDevice* xPL_LightingNode::device()  const { return (parent())?parent()->device():NULL; }
xPL_LightingGateway* xPL_LightingNode::gateway() const { return (parent())?parent()->gateway():NULL; }

bool xPL_LightingNode::targeted(xPL_Message& msg) {
	return msg.getValue(className()) == _id;
}

void xPL_LightingNode::msgAddIdPath(xPL_Message& msg) const
{
	parent()->msgAddIdPath(msg);
	msg.addKey( className(),id() );
	//msgAddIdKey(msg);
}


void xPL_LightingNode::msgAddIdKey(xPL_Message& msg) const
{
	msg.addKey( className(),id() );
}

// Message helpers

bool xPL_LightingNode::msgAddThisKey(xPL_Message& msg)
{
	msgAddKey( className(), this);
	return false;
}

void xPL_LightingNode::msgAddChildsKey(xPL_Message& msg)
{
	sendEvent(&xPL_LightingNode::msgAddConfigCurrent,msg,true);
}


void xPL_LightingNode::msgAddCount(xPL_Message& msg, const prog_char* keyName) const
{
	msg.addKey( keyName,new xPL_String(count()),true );
}

bool xPL_LightingNode::cmdGoto(xPL_LightingLevel level) { return true; }

void xPL_LightingNode::sendGoto(xPL_LightingLevel level)
{
	class :public xPL_Event {
	public: 
		xPL_LightingLevel level;
		virtual bool send(xPL_Node* n) { return ((xPL_LightingNode*)n)->cmdGoto(level); }
	} evt;

	evt.level=level;
	sendEvent(&evt,false);
}
