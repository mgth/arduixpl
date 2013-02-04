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

#include "xPL.h"
#include "utility/xPL_Adapter.h"
#include "utility/xPL_Config.h"
#include "utility/xPL_Group.h"
#include "utility/xPL_Filter.h"

xPL_Main::xPL_Main()
{
	_source.vendor = S(_vendorid);
	_source.device = S(_deviceid);
	addChild(&hbeat);

#ifdef XPL_GROUPS
	addChild(&groups);
#endif
#ifdef XPL_FILTERS
	addChild(&filters);
#endif
}

bool xPL_Main::setId(const VString& s)
{
	_source.instance = s;
	return _source.instance.load();
}

xPL_Address& xPL_Main::source() { return _source; }

bool xPL_Main::sendMessage(xPL_Message& msg)
{
	bool sent = false;
	xPL_Schema* sch = (xPL_Schema*)child();
	while(sch) { sent |= sch->sendMessage(msg); sch=(xPL_Schema*)sch->next(); }
	return sent;
}
 
/************************************************************
 * Interrupts                                               *
 ************************************************************/
#ifdef XPL_INTERRUPTS
	bool xPL_Main::attachInterrupt(int8_t pin, int mode)
	{
		switch(pin) {
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 2:				::attachInterrupt(0,xplInterrupt0,mode);				return true;			case 3:				::attachInterrupt(1,xplInterrupt1,mode);				return true;#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 21:				::attachInterrupt(2,xplInterrupt2,mode);				return true;			case 20:				::attachInterrupt(3,xplInterrupt3,mode);				return true;			case 19:				::attachInterrupt(4,xplInterrupt4,mode);				return true;			case 18:				::attachInterrupt(5,xplInterrupt5,mode);				return true;#endif#if defined (__AVR_ATmega32U4__)			case 3:				::attachInterrupt(0,xplInterrupt0,mode);				return true;			case 2:				::attachInterrupt(1,xplInterrupt1,mode);				return true;			case 0:				::attachInterrupt(2,xplInterrupt2,mode);				return true;			case 1:				::attachInterrupt(3,xplInterrupt3,mode);				return true;#endif		}		return false;	}

	bool xPL_Main::detachInterrupt(int8_t pin)
	{
		switch(pin) {
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 2:				::detachInterrupt(0);				return true;			case 3:				::detachInterrupt(1);				return true;#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 21:				::detachInterrupt(2);				return true;			case 20:				::detachInterrupt(3);				return true;			case 19:				::detachInterrupt(4);				return true;			case 18:				::detachInterrupt(5);				return true;#endif#if defined (__AVR_ATmega32U4__)			case 3:				::detachInterrupt(0);				return true;			case 2:				::detachInterrupt(1);				return true;			case 0:				::detachInterrupt(2);				return true;			case 1:				::detachInterrupt(3);				return true;#endif		}		return false;	}

	//TODO: calculate funtion overhead
	void xPL_Main::xplInterrupt(uint8_t pin) { xPL.interrupt(pin,micros()); }
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt0() { xplInterrupt(2); }	void xPL_Main::xplInterrupt1() { xplInterrupt(3); }#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt2() { xplInterrupt(21); }	void xPL_Main::xplInterrupt3() { xplInterrupt(20); }	void xPL_Main::xplInterrupt4() { xplInterrupt(19); }	void xPL_Main::xplInterrupt5() { xplInterrupt(18); }#endif#if defined (__AVR_ATmega32U4__)	void xPL_Main::xplInterrupt0() { xplInterrupt(3); }	void xPL_Main::xplInterrupt1() { xplInterrupt(2); }	void xPL_Main::xplInterrupt2() { xplInterrupt(0); }	void xPL_Main::xplInterrupt3() { xplInterrupt(1); }#endif#endifbool xPL_Main::targeted(xPL_MessageIn& msg)
{
	if (msg.parseHeader())
	{
		xPL_Address& addr = msg.target;
		bool targeted = (addr.isAny() || addr == source());
#ifdef XPL_GROUPS
		targeted |=  groups.checkTarget(msg);
#endif
#ifdef XPL_FILTERS
		targeted &=  filters.checkTarget(msg); 
#else
		targeted &= msg.msgType.instance==S(cmnd);
#endif
		return targeted;
	}
	return false;
}

/************************************************************
 * EVENTS                                                   *
 ************************************************************/
/***********************************************
  LOOP
************************************************/
#ifdef XPL_SLOWDEBUG
void xPL_Main::loop() {
			xPL_Node* n = child();
			while(n)
			{

		#ifdef XPL_DEBUG_LCD
				lcd.setCursor(0,0); lcd.print( S(_blkline) );
				lcd.setCursor(0,0); lcd.print( n->className() );
				lcd.setCursor(19,0);lcd.print('S');
				lcd.setCursor(0,3);printMemLCD();
				lcd.setCursor(0,1);	lcd.print( S(_blkline) ); lcd.setCursor(0,1);
		#endif
		
				Serial.print('<');Serial.print( n->className() );Serial.print('>');
			
				DBG_MEM(F("before"));

				delay(XPL_SLOWDEBUG);
			
				n->loop();

				Serial.print(F("<end ")); VString(n->className()).printlnTo(Serial,'>');

				printMemCost(F("cost:"));

		#ifdef XPL_DEBUG_LCD
				lcd.setCursor(0,2);
				lcd.setCursor(19,0);lcd.print('E');
				lcd.setCursor(0,3);printMemLCD();
		#endif
				delay(XPL_SLOWDEBUG);

				n=n->next();
			}
}
#endif



size_t xPL_Main::printConfig(Print& p,bool list)
{
	size_t len=0;
		if (list)
			len+=xPL_Message::printKeyTo(p,S(reconf),S(newconf));
		else
			len+=xPL_Message::printKeyTo(p,S(newconf),source().instance );

	return len + printConfigChilds(p,list);
}

void xPL_Main::config(xPL_Eeprom& eeprom, bool store)
{
	if(store)
	{
		eeprom.setAddress(3);
		_source.instance.printlnTo(eeprom,'\0');
		eeprom.setxPL();
	}
	else
	{
		_source.instance = eeprom.readString();
		_source.instance.load();
	}
	configChilds(eeprom,store);
}


void xPL_Main::storeConfig()
{
	xPL_Eeprom eeprom(3);
	config(eeprom,true);
}


/************************************************************
 * begin                                                     *
 ************************************************************/
void xPL_Main::begin(const __FlashStringHelper* vendor,const __FlashStringHelper* device,const __FlashStringHelper* instance) {
//	xPL_Eeprom eeprom(3);

	_source.vendor=vendor;
	_source.device=device;
	if (instance) _source.instance=instance; else _source.instance=S(default);
	defaultConfig();
//	config(eeprom,false);
}


/// TODO : implement targeted and review that to include in event



xPL_Main xPL;






