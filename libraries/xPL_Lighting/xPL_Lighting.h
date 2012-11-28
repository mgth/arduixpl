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

#ifndef XPL_LIGHTING_H
#define XPL_LIGHTING_H
#include <xPL.h>

#include "utility/xPL_LightingGateway.h"
#include "utility/xPL_LightingNetwork.h"
#include "utility/xPL_LightingDevice.h"
#include "utility/xPL_LightingChannel.h"


#include "utility/xPL_LightingTypes.h"


#include "utility/xPL_LightingOutputMapper.h"

#include "utility/xPL_Schema.h"

class xPL_Lighting : public xPL_Schema {
  
protected:

public:
	virtual const prog_char* className() const { return S(lighting); }
	xPL_LightingOutputMapper* out;

	void setMapper(xPL_LightingOutputMapper* mapper) { out = mapper; }

    xPL_LightingGateway gateway;

	xPL_Lighting() {
		out = NULL;
		addChild(&gateway);
	}
#ifdef XPL_LIGHTING_STDCONFIG
	virtual bool msgAddConfigList(xPL_Message& msg);
	virtual bool msgAddConfigCurrent(xPL_Message& msg);

	virtual bool configure(xPL_Key& key);
#endif
};

extern xPL_Lighting xplLighting;

#endif

