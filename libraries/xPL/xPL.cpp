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
	_state.oldConfigured = _state.configured = true;
	_state.deleteGroups = _state.deleteFilters = false;
	_source.vendor = S(_vendorid);
	_source.device = S(_deviceid);
}

const xPL_String* xPL_Main::id() const { return _source.id(); }

bool xPL_Main::setId(const xPL_String& s)
{
	_source.instance.moveTo(_oldId);
	
	if (_source.instance.load(s)) 
		return true;

	_oldId.moveTo(_source.instance);
	return false;
}

	xPL_String& xPL_Main::oldId() { return _oldId; }
	xPL_Address& xPL_Main::source() { return _source; }
 
	bool xPL_Main::trigHbeat(bool b) { bool v=b; _state.trigHbeat = b; return v; }
	bool xPL_Main::deleteGroups(bool b) { bool v=b; _state.deleteGroups = b; return v; }
	bool xPL_Main::deleteFilters(bool b) { bool v=b; _state.deleteFilters = b; return v; }

	bool xPL_Main::configured() { return _state.configured; }
	bool xPL_Main::oldConfigured() { return _state.oldConfigured; }

	void xPL_Main::setAdapter(xPL_Adapter* adapter) { _adapter=adapter; }

/************************************************************
 * CONFIG                                             *
 ************************************************************/
// EEPROM
bool xPL_Main::loadConfig(xPL_Eeprom& eeprom)
{
	eeprom.readString().moveTo(_source.instance);
	return true;
}

bool xPL_Main::loadDefaultConfig()
{
	_source.instance = S(default);
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
bool xPL_Main::msgAddConfigList(xPL_Message& msg)
{
	msg.addKey( S(reconf),S(newconf) );
	return true;
}

bool xPL_Main::msgAddConfigCurrent(xPL_Message& msg)
{
	msg.addKey( S(newconf),new xPL_String(source().instance),true ); // TODO ??? new ?
	return true;
}

bool xPL_Main::configure(xPL_Key& key)
{
	if (key == S(newconf))
	{
		xPL_String& value = key.sValue();

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
void xPL_Main::begin() {
	xPL_Eeprom eeprom(3);

	// if reset_pin HIGH, config is not loaded
	pinMode(XPL_RESET_PIN, INPUT);

	if ( eeprom.isxPL() && !digitalRead(XPL_RESET_PIN) )
	{
		eeprom.setAddress(3);
#ifdef XPL_DEBUG
		Serial.println(("Cfg:EEProm"));
#endif
		sendEvent(&xPL_Node::loadConfig,eeprom);
	}
	else
	{
#ifdef XPL_DEBUG
		Serial.println("Cfg:Default");
#endif
		sendEvent(&xPL_Node::loadDefaultConfig);
	}
}
/************************************************************
 * loop                                                     *
 ************************************************************/
bool xPL_Main::loop() { return true; }
void xPL_Main::loopAll() { sendEvent(&xPL_Node::loop); }


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
bool xPL_Main::receivedMessage(char* buffer) {

	xPL_Message msg;

	if (msg.parseHeader(buffer))
	{
		#ifdef XPL_DEBUG
		Serial.print(msg);
		#endif

		if (msg.msgType.msgType()==S(cmnd))
		{
			//sendEvent(&xPL_Node::checkTargeted,msg);

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
bool xPL_Main::checkTargeted(xPL_Message& msg)
{
	xPL_Address& addr = msg.target;
	msg.setTargeted(addr.isAny() || addr == source() );
	return true;
}


xPL_Main xPL;






