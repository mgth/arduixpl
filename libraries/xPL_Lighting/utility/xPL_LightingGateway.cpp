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
#include "xPL_LightingGateway.h"

#include "xPL_LightingNetwork.h"
#include "xPL_LightingMessage.h"


bool xPL_LightingGateway::parseMessage(xPL_LightingMessage& msg)
{
	xPL_Message* msgStatus = msg.statusMessage(this,S(gateinfo));
	if (msgStatus)
	{
			msgStatus->addKey( S(protocol), S(_protocol) );//[X10|UPB|CBUS|ZWAVE|INSTEON]
			/*
			description=
			version=
			author=
			info-url=
			*/
			msgAddCount(*msgStatus,S(net_count));

			msgStatus->addKey( S(preferred_net), defaultChild()->id() );

#ifdef XPL_LIGTHING_SCENES
			msgStatus->addKey( S(scenes_ok), S(_true) );
#else
			msgStatus->addKey( S(scenes_ok), S(_false) );
#endif
			msgStatus->addKey( S(channels_ok), S(_true) );
			msgStatus->addKey( S(fade_rate_ok), S(_true) );
			msgStatus->send(true);
			return false;
	}

	msgStatus = msg.statusMessage(this,S(netlist));
	if (msgStatus) {
			msgStatus->addKey( S(network), new xPL_ListId(this), true );
			msgStatus->send(true);
			return false;
	}

#ifdef XPL_LIGHTING_EXTCONFIG
	return msg.configCreate(this,S(network));
#else
	return true;
#endif

	//TODO somehow implement not found 

}

xPL_LightingNode* xPL_LightingGateway::add(const xPL_String& id)
{
	return (xPL_LightingNetwork*)addChild(new xPL_LightingNetwork(id));
}
