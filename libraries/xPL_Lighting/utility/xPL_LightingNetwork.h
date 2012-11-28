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

#ifndef NETWORK_H
#define NETWORK_H

#include <xPL.h>

#ifdef XPL_LIGHTING_SCENES
#include "xPL_LightingScene.h"
#endif
#include "xPL_LightingDevice.h"

#include "xPL_LightingNode.h"

/********************************************************************
xPL_LightingGroup: container for devices
********************************************************************/
class xPL_LightingGroup:public xPL_LightingNode {

protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);

	virtual void msgAddIdPath(xPL_Message& msg) const { parent()->msgAddIdPath(msg); } // level is virtual, should not be printed as an ID address
public:

	virtual xPL_LightingNode* add(const xPL_String& id);

};

#ifdef XPL_LIGHTING_SCENES
/********************************************************************
xPL_LightingSceneGroup: container for scenes
********************************************************************/
class xPL_LightingSceneGroup:public xPL_LightingGroup {
protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);

public:

	int msgAddDevId(xPL_Message& msg, xPL_LightingDevice* dev) const;
	int count(xPL_LightingDevice* dev) const;

	virtual xPL_LightingNode* add(const xPL_String& id);

};
#endif


/********************************************************************
xPL_LightingNetwork
********************************************************************/
class xPL_LightingNetwork:public xPL_LightingNode
{
protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);

#ifdef XPL_LIGHTING_CONFIG
	virtual void loadConfigClose() {}
	virtual bool loadConfig() { return true; } // lets devices and scenes groups do the job
#endif

public:
	virtual const prog_char* className() const { return S(network);}
	virtual xPL_LightingNetwork* network() { return this; }


	xPL_LightingGroup devices;
#ifdef XPL_LIGHTING_SCENES
	xPL_LightingSceneGroup scenes;
#endif

	xPL_LightingNetwork(const xPL_String& id);


};
#endif //NETWORK_H
