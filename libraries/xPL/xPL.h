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


#ifndef XPL_MAIN_H
#define XPL_MAIN_H

class xPL_Schema;
class xPL_Adapter;

#include "utility/xPL_Message.h"
#include "utility/xPL_Node.h"

class xPL_Main:public xPL_NodeParent
{
 private:
	 
	 struct {
		bool deleteGroups  :1;
		bool deleteFilters :1;
	} _state;

	 xPL_Adapter* _adapter;
	 xPL_Address _source;
	 VString _oldId;

protected:

	virtual void checkTargeted(xPL_MessageIn& msg);

//configuration
//EEPROM
	virtual void loadConfig(xPL_Eeprom& epprom);
	virtual void storeConfig(xPL_Eeprom& epprom);

//Messages
	virtual size_t printConfigList(Print& p);
	virtual size_t printConfigCurrent(Print& p);


public:
//id
	virtual void loop();
	virtual const __FlashStringHelper* className() const { return S(xpl); }	

	xPL_Main();

	void begin(const __FlashStringHelper* vendor, const __FlashStringHelper* device, const __FlashStringHelper* instance=NULL);

	virtual bool setId(const VString& id);
	VString& oldId();
	xPL_Address& source();
 
	bool trig(bool& trigger, const bool& b);

	bool deleteGroups(bool b=true);
	bool deleteFilters(bool b=true);

	void setConfigured(bool c=true);
	bool configured();
	bool oldConfigured();

	void setAdapter(xPL_Adapter* adapter);

	bool attachInterrupt(int8_t pin, int mode);
	bool detachInterrupt(int8_t pin);

	static void xplInterrupt(uint8_t pin);
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega32U4__)	static void xplInterrupt0();	static void xplInterrupt1();#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)	static void xplInterrupt2();	static void xplInterrupt3();#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	static void xplInterrupt4();	static void xplInterrupt5();#endif
	bool receivedMessage(VString& buffer);
	bool sendMessage(xPL_Message& msg); 
 
	void storeConfig();

};

extern xPL_Main xPL;

#endif
