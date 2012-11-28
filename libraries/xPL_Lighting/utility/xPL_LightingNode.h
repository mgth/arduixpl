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

	  Modified October 26, 2012 by Mathieu GRENET
*/

#ifndef XPL_LIGTHINGNODE_H
#define XPL_LIGTHINGNODE_H
#include "utility/xPL_Config.h"
#include "utility/xPL_Node.h"
#include "xPL_LightingTypes.h"


class xPL_Lighting;
class xPL_LightingGateway;
class xPL_LightingNetwork;
class xPL_LightingDevice;
class xPL_LightingMessage;

class xPL_LightingNode:public xPL_Node {

protected:
	xPL_String _id;
#ifdef XPL_LIGHTING_CONFIG
	virtual bool msgAddConfigCurrent(xPL_Message &msg);
#endif

  public:
	virtual const prog_char* className() const;
	virtual const xPL_String* id() const;
	virtual xPL_String channelId();

	xPL_LightingNode();
	xPL_LightingNode(const xPL_String& id);

	virtual bool parseMessage(xPL_LightingMessage& msg);
	virtual bool parseMessage(xPL_Message& msg);

	// Lighting commands
	virtual bool targeted(xPL_Message& msg);

	// tree
	virtual xPL_LightingNetwork* network() const;
	virtual xPL_LightingDevice* device() const;
	virtual xPL_LightingGateway* gateway() const;

	xPL_LightingNode* parent() const;


	// Messages output
	bool msgAddThisKey(xPL_Message& msg);
	virtual void msgAddIdPath(xPL_Message& msg) const;
	void msgAddIdKey(xPL_Message& msg) const;

	void msgAddChildsKey(xPL_Message& msg);
	void msgAddCount(xPL_Message& msg, const prog_char* keyName) const;

	virtual bool cmdGoto(xPL_LightingLevel level);

	void sendGoto(xPL_LightingLevel level);

};


#endif
