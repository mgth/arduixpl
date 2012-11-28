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

#ifndef xPL_LightingDEVICE_H
#define xPL_LightingDEVICE_H

#include <xPL.h>
#include "xPL_LightingNode.h"
#include "xPL_LightingChannel.h"

class xPL_LightingMessage;

class xPL_LightingDevice: public xPL_LightingNode {
protected:
	virtual bool parseMessage(xPL_LightingMessage& msg);
  
  public:
 	virtual const prog_char* className() const { return S(device);}

	xPL_LightingDevice(const xPL_String& id):xPL_LightingNode(id) {}

	xPL_LightingDevice* device() { return this; }

	xPL_Node* add(const xPL_String& id) { return addChild(new xPL_LightingChannelPWM(id)); }

	xPL_LightingChannel* addChannel(const xPL_String& id, uint16_t pin, xPL_LightingLevel level, xPL_LightingFaderate faderate) {
		xPL_LightingChannelPWM* c = (xPL_LightingChannelPWM*)add(id);
		c->setPin(pin);
		c->defaultLevel() = level;
		c->defaultFaderate() = faderate;
		return c;
	}
	
};
#endif
