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

	  Modified Aug 31, 2012 by Mathieu GRENET
*/

#ifndef XPL_MAIN_H
#define XPL_MAIN_H

class xPL_Schema;
class xPL_Adapter;

#include "utility/xPL_Message.h"

#include "utility/xPL_Node.h"

class xPL_Main:public xPL_Node
{
 private:
	 
	 struct {
		bool configured    :1; 
		bool oldConfigured :1;
		bool trigHbeat     :1;
		bool deleteGroups  :1;
		bool deleteFilters :1;
	} _state;

	 xPL_Adapter* _adapter;
	 xPL_Address _source;
	 xPL_String _oldId;

protected:

	virtual bool loop();
	virtual bool checkTargeted(xPL_Message& msg);

//configuration
//EEPROM
	virtual bool loadConfig(xPL_Eeprom& epprom);
	virtual bool loadDefaultConfig();
	virtual bool storeConfig(xPL_Eeprom& epprom);

//Messages
	virtual bool msgAddConfigList(xPL_Message& msg);
	virtual bool msgAddConfigCurrent(xPL_Message& msg);
	virtual bool configure(xPL_Key& key);


public:
//id
	virtual const xPL_String* id() const;

	xPL_Main();

	void begin();

	virtual bool setId(const xPL_String& id);
	xPL_String& oldId();
	xPL_Address& source();
 
	bool trig(bool& trigger, const bool& b);

	bool trigHbeat(bool b=true);
	bool deleteGroups(bool b=true);
	bool deleteFilters(bool b=true);

	void setConfigured(bool c=true);
	bool configured();
	bool oldConfigured();

	void setAdapter(xPL_Adapter* adapter);


	bool receivedMessage(char* buffer);
	bool sendMessage(xPL_Message& msg); 
 
	void loopAll();
	bool storeConfig();

};

extern xPL_Main xPL;

#endif
