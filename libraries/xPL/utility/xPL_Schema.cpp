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

	  Modified Dec 23, 2012 by Mathieu GRENET
*/

#include "xPL_Schema.h"

xPL_Schema::xPL_Schema() {	}

bool xPL_Schema::parseMessage(xPL_MessageIn& msg) { return targeted(msg); }
bool xPL_Schema::configure(xPL_Key& key){return false;}
bool xPL_Schema::targeted(xPL_MessageIn& msg) { return (msg.schema.device==className()); }

void xPL_Schema::reg() {
#ifdef xPL_Debug
	if (parent)
	{
		Serial.prinntln(F"Schema already registered");
	} else
#endif		
	xPL.addChild(this);
}

xPL_Schema* xPL_Schema::schema() { return this; }
