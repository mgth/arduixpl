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

#include "xPL_LightingNetwork.h"
#include "xPL_LightingMessage.h"

xPL_LightingNode* xPL_LightingGroup::add(const xPL_String& id)
{ 
	return ( xPL_LightingNode*)addChild(new xPL_LightingDevice(id));
}




#ifdef XPL_LIGHTING_SCENES
int xPL_LightingSceneGroup::count(xPL_LightingDevice* dev) const {
	class :public xPL_Event {
	public: 
		int count;
		xPL_LightingDevice* dev;
		const xPL_LightingSceneGroup* grp;

		virtual bool send(xPL_Node* n) {
			if (n->parent()!=grp)
			{
				xPL_LightingNode* node = ((xPL_LightingSceneMember*)n)->linkedItem();
				if (node==dev || node->parent()==dev) count++;
				return false;
			} else return true;
		}
	} evt;

	evt.grp=this;
	evt.count=0;
	evt.dev=dev;
	const_cast<xPL_LightingSceneGroup*>(this)->sendEvent(&evt,true);
	return evt.count;
}

xPL_LightingNode* xPL_LightingSceneGroup::add(const xPL_String& id)
{
	return ( xPL_LightingNode*)addChild(new xPL_LightingScene(id));
}

int xPL_LightingSceneGroup::msgAddDevId(xPL_Message& msg, xPL_LightingDevice* dev) const {
	class :public xPL_Event {
	public: 
		int count;
		xPL_LightingDevice* dev;
		const xPL_LightingSceneGroup* grp;
		xPL_Message* msg;

		virtual bool send(xPL_Node* n) {
			if (n->parent()!=grp)
			{
				xPL_LightingNode* node = ((xPL_LightingSceneMember*)n)->linkedItem();
				if (node==dev || node->parent()==dev)
					{
						count++;
						msg->addKey( S(scene),new xPL_LightingSceneMemberState((xPL_LightingSceneMember*)n,false),true );
				}
				return false;
			} else return true;
		}
	} evt;

	evt.msg = &msg;
	evt.grp=this;
	evt.count=0;
	evt.dev=dev;
	const_cast<xPL_LightingSceneGroup*>(this)->sendEvent(&evt,true);
	return evt.count;
}
#endif

xPL_LightingNetwork::xPL_LightingNetwork(const xPL_String& id):xPL_LightingNode(id)
{
	addChild(&devices);
#ifdef XPL_LIGHTING_SCENES
	addChild(&scenes);
#endif
}



bool xPL_LightingNetwork::parseMessage(xPL_LightingMessage& msg)
{
	if (!targeted(msg)) return false;

	xPL_Message* msgStatus = msg.statusMessage(this,S(netinfo));

	if (msgStatus)
	{
		devices.msgAddCount(msg, S(device_count));
#ifdef XPL_LIGHTING_SCENES
		scenes.msgAddCount(msg, S(scene_count));
#endif
		msgStatus->send(true);
		return false;
	}

	return true;
}

bool xPL_LightingGroup::parseMessage(xPL_LightingMessage& msg)
{
	xPL_Message* msgStatus = msg.statusMessage(this,S(scnlist));

	if (msgStatus)
	{
		msgAddCount(msg,S(device_count));
		msgStatus->addKey( S(device),new xPL_ListId(this),true );
		msgStatus->send(true);
		return false;
	}

#ifdef XPL_LIGHTING_SCENES
	if ( msg.key_scene() ) return false;
#endif

#ifdef XPL_LIGHTING_CONFIG
	return msg.configCreate(this,S(device));
#else
	return true;
#endif

}

#ifdef XPL_LIGHTING_SCENES
bool xPL_LightingSceneGroup::parseMessage(xPL_LightingMessage& msg)
{
	xPL_Message* msgStatus = msg.statusMessage(this,S(scnlist));
	if (msgStatus)
	{
		msgAddCount(*msgStatus, S(scene_count));
		msgStatus->addKey( S(scene),new xPL_ListId(this),true );
		msgStatus->send(true);
		return false;
	}

#ifdef XPL_LIGHTING_CONFIG
	return msg.configCreate(this,S(scene));
#else
	return true;
#endif
}
#endif




