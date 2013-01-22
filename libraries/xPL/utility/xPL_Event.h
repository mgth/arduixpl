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
#ifndef XPL_EVENT_H
#define XPL_EVENT_H

#include "xPL_Define.h"
#include "VString.h"

class xPL_Message;
class xPL_MessageIn;
class xPL_Key;
class xPL_Eeprom;

enum eventId {LOOP,PARSE_MESSAGE,CHK_TARGETED,SEND_MESSAGE,CFG_LIST,CFG_CURRENT,CONFIGURE,STORE_CFG,LOAD_CFG,INTERRUPT};

class xPL_Event
{
eventId _id;
const void* _obj;

public:
	eventId id() const { return _id; }
	xPL_Event(eventId id, void* obj=NULL):_id(id),_obj(obj) {}

	xPL_Message& message() const { return *(xPL_Message*)_obj; }
	xPL_MessageIn& messageIn() const { return *(xPL_MessageIn*)_obj; }
	xPL_Key& key() const { return *(xPL_Key*)_obj; }
	xPL_Eeprom& eeprom() const { return *(xPL_Eeprom*)_obj; }
	Print& print() const { return *(Print*)_obj; }
};



#endif