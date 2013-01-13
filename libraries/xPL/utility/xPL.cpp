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
#include "utility/xPL_Adapter.h"

xPL_Main::xPL_Main()
{
	_adapter = NULL;
	_state.oldConfigured = _state.configured = true;
	_state.trigHbeat = false;
	_state.deleteGroups = _state.deleteFilters = false;
	_source.vendor = S(_vendorid);
	_source.device = S(_deviceid);
}

const VString xPL_Main::id() const { return _source.instance; }

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
 
	bool xPL_Main::trigHbeat(bool b) { bool old=_state.trigHbeat; _state.trigHbeat = b; return old; }
	bool xPL_Main::deleteGroups(bool b) { bool old=_state.deleteGroups; _state.deleteGroups = b; return old; }
	bool xPL_Main::deleteFilters(bool b) { bool old=_state.deleteFilters; _state.deleteFilters = b; return old; }

	bool xPL_Main::configured() { return _state.configured; }
	bool xPL_Main::oldConfigured() { return _state.oldConfigured; }

	void xPL_Main::setAdapter(xPL_Adapter* adapter) { _adapter=adapter; }

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
	void xPL_Main::xplInterrupt(uint8_t pin) { xPL.sendEvent(xPL_EventInterrupt(pin,micros())); }
#if defined (__AVR_ATmega328P__)|| defined(__AVR_ATmega168P__) || defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt0() { xplInterrupt(2); }	void xPL_Main::xplInterrupt1() { xplInterrupt(3); }#endif#if defined (__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)	void xPL_Main::xplInterrupt2() { xplInterrupt(21); }	void xPL_Main::xplInterrupt3() { xplInterrupt(20); }	void xPL_Main::xplInterrupt4() { xplInterrupt(19); }	void xPL_Main::xplInterrupt5() { xplInterrupt(18); }#endif#if defined (__AVR_ATmega32U4__)	void xPL_Main::xplInterrupt0() { xplInterrupt(3); }	void xPL_Main::xplInterrupt1() { xplInterrupt(2); }	void xPL_Main::xplInterrupt2() { xplInterrupt(0); }	void xPL_Main::xplInterrupt3() { xplInterrupt(1); }#endif

/************************************************************
 * CONFIG                                             *
 ************************************************************/
// EEPROM
bool xPL_Main::loadConfig(xPL_Eeprom& eeprom)
{
	_source.instance = eeprom.readString();
	_source.instance.load();
	return true;
}

bool xPL_Main::loadDefaultConfig()
{
	//_source.instance = S(default);
	return true;
}

bool xPL_Main::storeConfig(xPL_Eeprom& eeprom)
{
	eeprom.setAddress(3);
	_source.instance.printlnTo(eeprom,'\0');

	eeprom.setxPL();
	return true;
}

bool xPL_Main::storeConfig()
{
	xPL_Eeprom eeprom(3);
	sendEvent(&xPL_Node::storeConfig,eeprom);
	return true;
}

// Messages
size_t xPL_Main::printConfigList(Print& p)
{
	return xPL_Message::printKeyTo(p,S(reconf),S(newconf));
}

size_t xPL_Main::printConfigCurrent(Print& p)
{
	return xPL_Message::printKeyTo(p,S(newconf),source().instance );
}

bool xPL_Main::configure(xPL_Key& key)
{
	if (key.key == S(newconf))
	{
		VString& value = key.value;

		if ( _source.instance == value || setId(value) )
		{
			setConfigured();
		}
		trigHbeat();
		return false; // just mean not to configure childs with that key
	}
	return true;
}

void xPL_Main::setConfigured(bool c)
{
	 _state.oldConfigured = _state.configured && c;
	_state.configured     = c;
}
/************************************************************
 * begin                                                     *
 ************************************************************/
void xPL_Main::begin(const __FlashStringHelper* vendor,const __FlashStringHelper* device,const __FlashStringHelper* instance) {
	xPL_Eeprom eeprom(3);

	_source.vendor=vendor;
	_source.device=device;
	if (instance) _source.instance=instance; else _source.instance=S(default);


	// if reset_pin HIGH, config is not loaded
	pinMode(XPL_RESET_PIN, INPUT);
	// TODO : enable eeprom config when ok
	if ( false && eeprom.isxPL() && !digitalRead(XPL_RESET_PIN) )
	{
		eeprom.setAddress(3);

		DBG(F("Cfg:EEProm"),);

		sendEvent(&xPL_Node::loadConfig,eeprom);
	}
	else
	{
		DBG(F("Cfg:Default"),);
		sendEvent(&xPL_Node::loadDefaultConfig);
	}
}
/************************************************************
 * loop                                                     *
 ************************************************************/
bool xPL_Main::loop() { return true; }
void xPL_Main::loopAll() {

#ifdef XPL_SLOWDEBUG
	class :public xPL_Event {
	public: 
		mutable int count;
		virtual bool send(xPL_Node* n) const {
			Serial.print(F("<"));
			Serial.print( n->className() );
			printMemCost(F(" loop>"));
			//Serial.print(F(" - "));
			//if (n->id()) Serial.println(*n->id());
			delay(XPL_SLOWDEBUG);
			bool r = n->loop();

			Serial.print(F("<end "));
			VString(n->className()).printlnTo(Serial,'>');
			Serial.println();

			printMemCost(F("loop:"));
			delay(XPL_SLOWDEBUG);
			return r;
		}
	} evt;

	
	sendEvent(evt);
#else
	sendEvent(&xPL_Node::loop);
#endif

}


/************************************************************
 * sendMessage                                          *
 ************************************************************/

bool xPL_Main::sendMessage(xPL_Message& msg) {
	if (_adapter)
		return _adapter->sendMessage(msg);
	else
		return false;
} 

/************************************************************
 * receivedMessage                                          *
 * Called when an xPL message is received                   *
 ************************************************************/
bool xPL_Main::receivedMessage(VString& buffer) {

	xPL_MessageIn msg(buffer);

	if (msg.parseHeader())
	{
		DBG(msg,);

		if (msg.msgType.instance==S(cmnd))
		{
			sendEvent(&xPL_Node::checkTargeted,msg);

			if(msg.targeted())
			{
				sendEvent(&xPL_Node::parseMessage,msg,true);
			
			}
		}
	}
	return true;
}

/************************************************************
 * checkTargeted                                            *
 ************************************************************/
bool xPL_Main::checkTargeted(xPL_MessageIn& msg)
{
	xPL_Address& addr = msg.target;
	msg.setTargeted(addr.isAny() || addr == source() );
	return true;
}


xPL_Main xPL;






