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

#include <xPL.h>
#include "xPL_LightingDevice.h"
#include "xPL_LightingNetwork.h"
#include "xPL_LightingMessage.h"

class xPL_LightingNode;

bool xPL_LightingDevice::parseMessage(xPL_LightingMessage& msg)
{
	if (!targeted(msg)) return false;

	xPL_Message* msgStatus = msg.statusMessage(this,S(devinfo));

	if ( msgStatus )
	{
		msgStatus->addKey(S(report_on_manual),S(_true) );

		msgAddCount(*msgStatus,S(channel_count));
		msgStatus->addKey( S(primary_channel),defaultChild()->id() );

		sendEvent(&xPL_LightingChannel::msgAddState,*msgStatus,true);

#ifdef XPL_LIGHTING_SCENES
		msgStatus->addKey(
			S(scene_count),
			new xPL_String(network()->scenes.count(const_cast<xPL_LightingDevice*>(this))),
			true );

		network()->scenes.msgAddDevId(*msgStatus,this);
#endif
		msgStatus->send(true);
		return false;
	}

#ifdef XPL_LIGHTING_EXTCONFIG
	return msg.configCreate(this,S(channel));
#else
	return true;
#endif
}


