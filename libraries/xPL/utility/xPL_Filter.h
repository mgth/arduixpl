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

#ifndef XPL_FILTER_H
#define XPL_FILTER_H

#include "utility/xPL_Schema.h"

#ifndef XPL_MAX_FILTERS
#define XPL_MAX_FILTERS 16
#endif

enum filterIndex {MSGTYPE,VENDOR,DEVICE,INSTANCE,CLASS,TYPE};

class xPL_Filter:public xPL_Node {
private:
	byte _mask;
	VString* _field;
protected:

public:
	bool checkTarget(xPL_MessageIn& msg);
	void config(xPL_Eeprom& eeprom,bool store=false);
	void configure(xPL_Key& key);
	size_t printConfig(Print& p, bool list=false);

	void setField(byte i,const VString& s);

	size_t printTo(Print& p) const;

	virtual ~xPL_Filter() {
		free(_field);
		// delete [] _field; TODO when arduino 1.0.4
	}
};

class xPL_SchemaFilter:public xPL_Schema {
private:
	bool _deleteFirst;

protected:
	virtual const __FlashStringHelper* className() const { return S(filter); }
	void loop();
	size_t printConfig(Print& p, bool list=false);
	void configure(xPL_Key& key);
	void config(xPL_Eeprom& eeprom, bool store=false);
public:
	bool checkTarget(xPL_MessageIn& msg);


};

extern	xPL_SchemaFilter xplFilter;

#endif