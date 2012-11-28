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

#ifndef XPL_LIGHTINGGATEWAY_H
#define XPL_LIGHTINGGATEWAY_H
#include <xPL.h>

#include "xPL_LightingNode.h"

class xPL_LightingNetwork;
class xPL_LightingMessage;

class xPL_LightingGateway : public xPL_LightingNode
{
protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);

// Configuration

#ifdef XPL_LIGHTING_CONFIG
//	virtual bool loadConfig(); default is ok for network
	virtual bool storeConfig() { return true; }
#ifdef XPL_LIGHTING_STDCONFIG
	virtual bool msgAddConfigCurrent(xPL_Message& msg) { return true; }
#endif
#endif

public:

	virtual const prog_char* className() const { return S(gateway);}
	virtual void msgAddIdPath(xPL_Message& msg) const {};

	xPL_LightingGateway():xPL_LightingNode(xPL_String()) {}


// managment
	virtual xPL_LightingGateway* gateway() const { return const_cast<xPL_LightingGateway*>(this); }

	xPL_LightingNode* add(const xPL_String& id);

};

#endif
