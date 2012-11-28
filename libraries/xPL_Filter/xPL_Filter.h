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

	  Modified Apr 21, 2012 by Mathieu GRENET
*/
#ifndef XPL_FILTER_H
#define XPL_FILTER_H

#include <xPL.h>
#include "utility/xPL_Schema.h"

#ifndef XPL_MAX_FILTERS
#define XPL_MAX_FILTERS 16
#endif

typedef enum {MSGTYPE,VENDOR,DEVICE,INSTANCE,CLASS,TYPE} filterIndex;

class xPL_Filter:public xPL_Node {
private:
	xPL_String _field[6];

protected:
	virtual bool checkTargeted(xPL_Message& msg);
	virtual bool msgAddConfigCurrent(xPL_Message& msg);

public:
	void setField(byte i,const xPL_String& s);

	size_t printTo(Print& p) const;

	virtual bool loadConfig(xPL_Eeprom& eeprom);
	virtual bool storeConfig(xPL_Eeprom& eeprom);

};

class xPL_SchemaFilter:public xPL_Schema {
protected:
	virtual const prog_char* className() const { return S(filter); }
	virtual bool checkTargeted(xPL_Message& msg) { return true; }
	virtual bool msgAddConfigCurrent(xPL_Message& msg);
	virtual bool msgAddConfigList(xPL_Message& msg);
	virtual bool configure(xPL_Key& key);

public:
	virtual xPL_Node*  readConfig(xPL_Eeprom& eeprom);
	virtual bool storeConfig(xPL_Eeprom& eeprom);
};

extern	xPL_SchemaFilter xplFilter;

#ifndef XPL_BEGIN
#define XPL_BEGIN ;
#endif

#define XPL_BEGIN XPL_BEGIN##xplFilter.reg();

#endif