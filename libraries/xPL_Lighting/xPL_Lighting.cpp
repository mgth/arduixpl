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

#include "xPL_Lighting.h"

xPL_Lighting xplLighting;

#ifdef XPL_LIGHTING_STDCONFIG

bool xPL_Lighting::msgAddConfigList(xPL_Message& msg)
{
	  msg.addOptionKey( CStr::channel(), XPL_LIGHTING_MAX_CHANNELS );
#ifdef XPL_LIGHTING_SCENES
	  msg.addOptionKey( CStr::scene(), XPL_LIGHTING_MAX_SCENES );
#endif
	  return false;
};

bool xPL_Lighting::msgAddConfigCurrent(xPL_Message& msg) { return true; }

// TODO :
//channel=network,device,channel,default_level,default_faderate,pin
//scene  =network,scene,device,channel,level,faderate        
bool xPL_Lighting::configure(xPL_Key& key)
{
#ifdef XPL_LIGHTING_SCENES
	bool isScene=(key== CStr::scene());
#endif

	bool isChannel=(key == CStr::channel());

	if (
#ifdef XPL_LIGHTING_SCENES
		isScene || 
#endif
		isChannel)
	{
		xPL_LightingLevel level;
		xPL_LightingFaderate faderate;

		xPL_String val[6];
		byte i =0;

		val[i]=key.sValue();
		while(i<5) {
			val[i].parseTo(',',val[i+1]);
			i++;
		}

		i=0;

		xPL_LightingNetwork* n = (xPL_LightingNetwork*)gateway.findOrAdd(val[i]);
		i++;

#ifdef XPL_LIGHTING_SCENES
		xPL_LightingScene* s=NULL;
		if (isScene)
		{
			s = (xPL_LightingScene*)n->scenes.findOrAdd(val[i]);
			i++;
		}
#endif
		xPL_LightingDevice* d = (xPL_LightingDevice*)n->devices.findOrAdd(val[i]);
		i++;

		xPL_LightingChannel* c = (xPL_LightingChannel*)d->findChild(val[i]); i++;

		level.fromString(val[i]); i++;
		level.faderate().fromString(val[i]); i++;

#ifdef XPL_LIGHTING_SCENES
		if (s)
		{
			if (c)
				s->add(c,level);
			else
				s->add(d,level);
		}
		else
#endif
		{
			xPL_Int pin(val[i]);

			if (c)
			{
				c->defaultLevel() = level;
				c->defaultFaderate() = faderate;
			}
			else
			{
				c = d->addChannel(val[2],1,level,faderate);
			}
		}

		return false;
	}
	return xPL_Schema::configure(key);
}

#endif