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

#include "xPL.h"
#include "xPL_Adapter.h"

xPL_Main::xPL_Main()
{
	_state.deleteGroups = _state.deleteFilters = false;
	_source.vendor = S(_vendorid);
	_source.device = S(_deviceid);
}

bool xPL_Main::setId(const VString& s)
{
	
	_oldId = _source.instance;

	_source.instance = s;

	if (_source.instance.load())
		return true;

	_source.instance=_oldId;
	return false;
}

	VString& xPL_Main::oldId() { return _oldId; }
	xPL_Address& xPL_Main::source() { return _source; }
 
/************************************************************
 * Interrupts                                               *
 ************************************************************/

	bool xPL_Main::attachInterrupt(int8_t pin, int mode)
	{
		switch(pin) {
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 2:				::attachInterrupt(0,xplInterrupt0,mode);				return true;			case 3:				::attachInterrupt(1,xplInterrupt1,mode);				return true;#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 21:				::attachInterrupt(2,xplInterrupt2,mode);				return true;			case 20:				::attachInterrupt(3,xplInterrupt3,mode);				return true;			case 19:				::attachInterrupt(4,xplInterrupt4,mode);				return true;			case 18:				::attachInterrupt(5,xplInterrupt5,mode);				return true;#endif#if defined (__AVR_ATmega32U4__)			case 3:				::attachInterrupt(0,xplInterrupt0,mode);				return true;			case 2:				::attachInterrupt(1,xplInterrupt1,mode);				return true;			case 0:				::attachInterrupt(2,xplInterrupt2,mode);				return true;			case 1:				::attachInterrupt(3,xplInterrupt3,mode);				return true;#endif		}		return false;	}

	bool xPL_Main::detachInterrupt(int8_t pin)
	{
		switch(pin) {
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 2:				::detachInterrupt(0);				return true;			case 3:				::detachInterrupt(1);				return true;#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)			case 21:				::detachInterrupt(2);				return true;			case 20:				::detachInterrupt(3);				return true;			case 19:				::detachInterrupt(4);				return true;			case 18:				::detachInterrupt(5);				return true;#endif#if defined (__AVR_ATmega32U4__)			case 3:				::detachInterrupt(0);				return true;			case 2:				::detachInterrupt(1);				return true;			case 0:				::detachInterrupt(2);				return true;			case 1:				::detachInterrupt(3);				return true;#endif		}		return false;	}

	//TODO: calculate funtion overhead
	void xPL_Main::xplInterrupt(uint8_t pin) {		xPL_Interrupt i(pin,micros());		xPL.event(xPL_Event(INTERRUPT,&i));	}
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt0() { xplInterrupt(2); }	void xPL_Main::xplInterrupt1() { xplInterrupt(3); }#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt2() { xplInterrupt(21); }	void xPL_Main::xplInterrupt3() { xplInterrupt(20); }	void xPL_Main::xplInterrupt4() { xplInterrupt(19); }	void xPL_Main::xplInterrupt5() { xplInterrupt(18); }#endif#if defined (__AVR_ATmega32U4__)	void xPL_Main::xplInterrupt0() { xplInterrupt(3); }	void xPL_Main::xplInterrupt1() { xplInterrupt(2); }	void xPL_Main::xplInterrupt2() { xplInterrupt(0); }	void xPL_Main::xplInterrupt3() { xplInterrupt(1); }#endif

/************************************************************
 * EVENTS                                                   *
 ************************************************************/
size_t xPL_Main::event(const xPL_Event& evt)
{
	size_t len=0;

	switch(evt.id())
	{
/***********************************************
  LOOP
************************************************/
#ifdef XPL_SLOWDEBUG
		case LOOP:
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
		
				Serial.print(F("<"));Serial.print( n->className() );
			
				printMemCost(F(" loop>"));

				delay(XPL_SLOWDEBUG);
			
				n->event(xPL_Event(LOOP));

				Serial.print(F("<end "));
				VString(n->className()).printlnTo(Serial,'>');
				Serial.println();

				printMemCost(F("loop:"));

		#ifdef XPL_DEBUG_LCD
				lcd.setCursor(0,2);
				lcd.setCursor(19,0);lcd.print('E');
				lcd.setCursor(0,3);printMemLCD();
		#endif
				delay(XPL_SLOWDEBUG);

				n=n->next();
			}
#endif
/***********************************************
  CHK_TARGETED
************************************************/
		case CHK_TARGETED:
			{
				xPL_Address& addr = evt.messageIn().target;
				evt.messageIn().setTargeted(addr.isAny() || addr == source() );
			}
/***********************************************
  CONFIG
************************************************/
		case CFG_LIST:
			len +=xPL_Message::printKeyTo(evt.print(),S(reconf),S(newconf));
			break;
		case CFG_CURRENT:
			len +=xPL_Message::printKeyTo(evt.print(),S(newconf),source().instance );
			break;
/*---------------------------------------------*/
		case LOAD_CFG:
			if (evt.eeprom().isxPL())
			{
				_source.instance = evt.eeprom().readString();
				_source.instance.load();
			}
			else
			{
			//_source.instance = S(default);
			}
			break;
		case STORE_CFG:
			evt.eeprom().setAddress(3);
			_source.instance.printlnTo(evt.eeprom(),'\0');

			evt.eeprom().setxPL();
			break;
/*---------------------------------------------*/
		default:
			break;

	}
	return len + sendEvent(evt);
}


void xPL_Main::storeConfig()
{
	xPL_Eeprom eeprom(3);
	event(xPL_Event(STORE_CFG,&eeprom));
}

/************************************************************
 * begin                                                     *
 ************************************************************/
void xPL_Main::begin(const __FlashStringHelper* vendor,const __FlashStringHelper* device,const __FlashStringHelper* instance) {
	xPL_Eeprom eeprom(3);

	_source.vendor=vendor;
	_source.device=device;
	if (instance) _source.instance=instance; else _source.instance=S(default);

	event(xPL_Event(LOAD_CFG,&eeprom));
}


/************************************************************
 * receivedMessage                                          *
 * Called when an xPL message is received                   *
 ************************************************************/
bool xPL_Main::receivedMessage(VString& buffer) {

	xPL_MessageIn msg(buffer);

	if (msg.parseHeader())
	{
#ifdef XPL_DEBUG
		DBG(F("<received>"),);
		buffer.printTo(Serial);
#endif

		if (msg.msgType.instance==S(cmnd))
		{
			event(xPL_Event(CHK_TARGETED,&msg));

			if(msg.targeted())
			{
				event(xPL_Event(PARSE_MESSAGE,&msg));
			
			}
		}
	}
	return true;
}


xPL_Main xPL;






