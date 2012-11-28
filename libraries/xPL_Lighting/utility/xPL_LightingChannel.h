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

	  Modified 20 Mars 2012 by Mathieu GRENET
*/

#ifndef LBCHANNEL_H
#define LBCHANNEL_H

#include "utility/xPL_String.h"
#include "xPL_LightingTypes.h"
#include "xPL_LightingNode.h"

class xPL_LightingNode;
class xPL_LightingChannel;

#ifdef XPL_LIGHTING_STDCONFIG
class xPL_LightingChannelConfig: public xPL_Printable {
private:
	xPL_LightingChannel* _channel;
public:
	xPL_LightingChannelConfig(xPL_LightingChannel* c) { _channel=c; }
	size_t printTo(Print& p) const;
};
#endif

class xPL_LightingLevelDyn : public xPL_LightingLevel {
protected:

	int _levelStartup;
    time_t _timeStartup;

	time_t timeTarget();

	virtual int value();

public:
	bool sendTrigger;
	xPL_LightingLevelDyn();

	void cmdGoto(const xPL_LightingLevel level);
};


class xPL_LightingChannelState: public xPL_Printable {
private:
	xPL_LightingChannel* _channel;
public:
	xPL_LightingChannelState(xPL_LightingChannel* c);
	size_t printTo(Print& p) const;
};

class xPL_LightingChannel:public xPL_LightingNode {
  private: 
	bool _sendTrigger;

    level_t _defaultLevel;
    faderate_t _defaultFaderate;   
protected:
	xPL_LightingLevelDyn _level;
  
  public:
	virtual const prog_char* className() const;
	virtual bool targeted(xPL_Message& msg);

    xPL_LightingChannel(const xPL_String& id);

    xPL_LightingChannel(const xPL_String& id, int outNb,const xPL_LightingLevel& defaultLevel);

  

#ifdef XPL_LIGHTING_CONFIG
	virtual bool storeConfig(xPL_Eeprom& eeprom);
	virtual bool loadConfig(xPL_Eeprom& eeprom);
#endif

// xPL commandes
  	virtual bool parseMessage(xPL_LightingMessage& msg);

#ifdef XPL_LIGHTING_STDCONFIG
	virtual bool msgAddConfigCurrent(xPL_Message& msg) { 
		msg.addKey(CStr::channel(), new xPL_LightingChannelConfig(this),true);
		return false;
	}
#endif

	bool msgAddState(xPL_Message& msg);

	virtual xPL_String channelId() { return *id(); }
	virtual bool cmdGoto(const xPL_LightingLevel& level);

// xPL Messages
	bool sendTrigger();

	virtual size_t printTo(Print& p);


////////////////


    xPL_LightingLevel& defaultLevel();
	xPL_LightingLevel& level();
    xPL_LightingFaderate& defaultFaderate();
};

class xPL_LightingChannelPWM : public xPL_LightingChannel 
{
	byte _pin;

public:
	xPL_LightingChannelPWM(const xPL_String& id):xPL_LightingChannel(id) {};
	void setPin(byte pin) { _pin=pin; }
    virtual bool loop();

};

#endif    
    
