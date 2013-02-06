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

#ifndef xPL_Schema_h
#define xPL_Schema_h

#include "xPL_Node.h"

class xPL_Schema : public xPL_NodeParent
{
protected:
//	virtual size_t event(xPL_Event& evt);
	virtual bool targeted(xPL_MessageIn& msg);
	
public:

	virtual void reg();
	virtual xPL_Schema* schema();

	virtual bool sendMessage(xPL_Message& msg) { return false; }
};

class xPL_SchemaGeneric : public xPL_Schema
{
	const __FlashStringHelper* _schClass;
	void (*_parser)(xPL_MessageIn& msg);

	const __FlashStringHelper* className() const { return _schClass; }

	void parseMessage(xPL_MessageIn& msg){ (*_parser)(msg); }
public:
	xPL_SchemaGeneric(const __FlashStringHelper* schClass, void (&parser)(xPL_MessageIn& msg)):
		_schClass(schClass), _parser(&parser) {}

};
#endif
