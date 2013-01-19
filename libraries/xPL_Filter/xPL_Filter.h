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

enum filterIndex {MSGTYPE,VENDOR,DEVICE,INSTANCE,CLASS,TYPE};

class xPL_Filter:public xPL_Node {
private:
	byte _mask;
	VString* _field;
protected:

public:
	virtual size_t event(const xPL_Event& evt);
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
	virtual size_t event(const xPL_Event& evt);

};

extern	xPL_SchemaFilter xplFilter;

#endif