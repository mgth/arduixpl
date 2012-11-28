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

#ifndef xPL_LightingSCENE_H
#define xPL_LightingSCENE_H

#include <xPL.h>
#include "xPL_LightingDevice.h"
#include "xPL_LightingSceneMember.h"

#include "xPL_LightingNode.h"

class xPL_LightingMessage;

class xPL_LightingScene:public xPL_LightingNode {

protected:
    virtual bool parseMessage(xPL_LightingMessage& msg);

public:
	virtual const prog_char* className() const { return S(scene);}

    xPL_LightingScene(const xPL_String& id);

	xPL_LightingSceneMember* add( xPL_LightingNode* node, xPL_LightingLevel level);

	virtual xPL_Node* add(const xPL_String& id);

};

#endif