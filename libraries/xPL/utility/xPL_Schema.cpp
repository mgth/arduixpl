/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.

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

	  Modified 2013-1-22 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#include "xPL_Schema.h"
#include "xPL_Message.h"
#include "../xPL.h"

bool xPL_Schema::targeted(xPL_MessageIn& msg) { return (msg.schema.device==className()); }


void xPL_Schema::reg() {
#ifdef XPL_DEBUG
	if (parent())
	{
		Serial.println(F("Schema already registered"));
	} else
#endif		
	xPL.addChild(this);
}

xPL_Schema* xPL_Schema::schema() { return this; }
