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

	  Modified Apr 17, 2012 by Mathieu GRENET
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

const VString* xPL_Main::id() const { return _source.id(); }

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
	return xPL_Message::printKey(p,S(reconf),S(newconf));
}

size_t xPL_Main::printConfigCurrent(Print& p)
{
	return xPL_Message::printKey(p,S(newconf),source().instance );
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

	if ( eeprom.isxPL() && !digitalRead(XPL_RESET_PIN) )
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
		int count;
		virtual bool send(xPL_Node* n) {
			Serial.print(F("<"));
			Serial.print( n->className() );
			printMemCost(F(" loop>"));
			//Serial.print(F(" - "));
			//if (n->id()) Serial.println(*n->id());
			delay(1000);
			bool r = n->loop();

			Serial.print(F("<end "));
			VString(n->className()).printlnTo(Serial,'>');
			Serial.println();

			printMemCost(F("loop:"));
			delay(1000);
			return r;
		}
	} evt;

	
	sendEvent(&evt);
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






