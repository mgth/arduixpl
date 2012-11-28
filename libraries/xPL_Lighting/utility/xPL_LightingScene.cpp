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

	  Modified Aug 31, 2012 by Mathieu GRENET
*/

#include "xPL_LightingScene.h"
#include "xPL_LightingMessage.h"

xPL_LightingScene::xPL_LightingScene(const xPL_String& id):xPL_LightingNode(id) { }

xPL_LightingSceneMember*  xPL_LightingScene::add(xPL_LightingNode* n,xPL_LightingLevel level)
{
	xPL_LightingSceneMember* member = (xPL_LightingSceneMember*)addChild(new xPL_LightingSceneMember(n));
	member->level() = level;
	return member;
}

xPL_Node* xPL_LightingScene::add(const xPL_String& id) 
{
	xPL_LightingNode* node = (xPL_LightingNode*)network()->devices.findChild(id);
	if (node) return (xPL_LightingSceneMember*)addChild(new xPL_LightingSceneMember(node));
	else return NULL;
}



bool xPL_LightingScene::parseMessage(xPL_LightingMessage& msg)
{
	if(!targeted(msg)) return false;

	xPL_Message* msgStatus = msg.statusMessage(this,S(scninfo));

	if (msgStatus)
	{
		msgAddCount(*msgStatus, S(device_count));
		msgAddChildsKey(*msgStatus);
		msgStatus->send(true);
		return false;
	}

#ifdef XPL_LIGHTING_EXTCONFIG
	return msg.configCreate(this,S(device));
#else
	return true;
#endif
}
