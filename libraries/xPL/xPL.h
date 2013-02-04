/*
  ArduixPL - xPL library for Arduino(tm)
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

	  Modified 2013-2-4 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#ifndef xPL_h
#define xPL_h

class xPL_Schema;
class xPL_Adapter;

#include "utility/xPL_Config.h"
#include "utility/xPL_Group.h"
#include "utility/xPL_Filter.h"
#include "utility/xPL_Message.h"
#include "utility/xPL_Node.h"

class xPL_Main:public xPL_NodeParent
{
 private:
	 
	 xPL_Address _source;
public:
#ifdef XPL_SLOWDEBUG
	void loop();
#endif
	void config(xPL_Eeprom& eeprom, bool store=false);
	size_t printConfig(Print& p,bool list=false);
	void storeConfig();


#ifdef XPL_CONFIG
	xPL_Config hbeat;
#else
	xPL_Hbeat hbeat;
#endif


#ifdef XPL_GROUPS
	xPL_SchemaGroup groups;
#endif

#ifdef XPL_FILTERS
	xPL_SchemaFilter filters;
#endif

	bool targeted(xPL_MessageIn& msg);


	virtual const __FlashStringHelper* className() const { return S(xpl); }	

	xPL_Main();

	void begin(const __FlashStringHelper* vendor, const __FlashStringHelper* device, const __FlashStringHelper* instance=NULL);

	bool sendMessage(xPL_Message &msg);
	void parseMessage(xPL_MessageIn& msg)
	{
		if (targeted(msg)) parseMessageChilds(msg);
	}

	bool setId(const VString& id);
	xPL_Address& source();
 
	bool attachInterrupt(int8_t pin, int mode);
	bool detachInterrupt(int8_t pin);

#ifdef XPL_INTERRUPTS
	static void xplInterrupt(uint8_t pin);#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega32U4__)	static void xplInterrupt0();	static void xplInterrupt1();#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)	static void xplInterrupt2();	static void xplInterrupt3();#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	static void xplInterrupt4();	static void xplInterrupt5();#endif#endif

};

extern xPL_Main xPL;

#endif
