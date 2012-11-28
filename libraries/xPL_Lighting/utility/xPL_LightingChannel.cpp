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

	  Modified Oct 30, 2012 by Mathieu GRENET
*/

#include "../xPL_Lighting.h"

#include "xPL_LightingChannel.h"
#include "xPL_LightingNode.h"
#include "xPL_LightingMessage.h"

/*
*  LightingChannel
*/
const prog_char* xPL_LightingChannel::className() const { return S(channel);}

xPL_LightingLevel& xPL_LightingChannel::defaultLevel() { return _defaultLevel; }
xPL_LightingLevel& xPL_LightingChannel::level() { return _level; }
xPL_LightingFaderate& xPL_LightingChannel::defaultFaderate() { return _defaultFaderate; }
xPL_LightingChannel::xPL_LightingChannel(const xPL_String& id)
:xPL_LightingNode(id)
{
	_sendTrigger=false;

	_defaultLevel = 0;
	_defaultFaderate = 0;
}

xPL_LightingChannel::xPL_LightingChannel(const xPL_String& id,int outNb,const xPL_LightingLevel& defaultLevel)
:xPL_LightingNode(id)
{
	_sendTrigger=false;
	_defaultLevel = defaultLevel;

	_level.cmdGoto( _defaultLevel );
}

bool xPL_LightingChannel::targeted(xPL_Message&  msg) {

	xPL_String s = msg.getValue(className());

	return (!s || s == S(_zero) || s == _id );
}

bool xPL_LightingChannel::cmdGoto(const xPL_LightingLevel& level) //168
{
		_level.cmdGoto(level);
		return false;
}

bool xPL_LightingChannel::parseMessage(xPL_LightingMessage& msg) //806
{
	if (!targeted(msg)) return false; //32
/*
	if ( msg.isTypeCommand(S(basic),S(goto) ))
	{
			cmdGoto(msg.level(&_level,&_defaultLevel)); // 60
			return false; // 2
	}
	*/
#ifdef XPL_LIGHTING_EXTCONFIG
	if ( msg.isConfigCreate() ) //110
	{
		//xPL_String sPin = msg.key_pin(); // TODO move to pwm
		//if (sPin) _outNb = xPL_Int(sPin);

		_defaultLevel=msg.level();
	}
#endif
	return false;	
}








size_t xPL_LightingChannel::printTo(Print& p) // 142
{
/*	size_t len = 0;
	_id.printlnTo(p,','); //20
	xPL_Printable::println_prog(p,CStr::s_true(),',');// 22  TODO : deal with non dimmable pins
	defaultFaderate().printlnTo(p,','); //24
	level().printTo(p); //68
	return len;//48
	*/

	return xPL_Printable::emit_p(p,
			PSTR("$,&,$,$"),
			&_id,
			S(_true),
			&defaultFaderate(),
			&level()
		);
}

bool xPL_LightingChannel::sendTrigger() // 338
{
	xPL_Message msg(S(stat),S(lighting),S(device));

	msgAddIdPath(msg); // 24
	msg.addKey( S(state), level().toBit()?S(on):S(off)  ); // 56
	msg.addKey( S(level), &level() ); // 44

	return msg.send(true); // 12
}

bool xPL_LightingChannel::msgAddState(xPL_Message& msg)
{
	msg.addKey(S(channel), new xPL_LightingChannelState(this),true);
	return false;
}	
#ifdef XPL_LIGHTING_CONFIG
bool xPL_LightingChannel::loadConfig(xPL_Eeprom& eeprom) {
//	eeprom.readAny(_outNb); //TODO
	_defaultLevel.fromEeprom(eeprom); //18
	_defaultFaderate.fromEeprom(eeprom); //18
	return false; // 2
}

bool xPL_LightingChannel::storeConfig(xPL_Eeprom& eeprom) {
	xPL_LightingNode::storeConfig(eeprom); // 4
//	eeprom.writeAny(_outNb); // TODO
	_defaultLevel.toEeprom(eeprom); // 18
	_defaultFaderate.toEeprom(eeprom); // 18
	return false; // 2
}


//channel=network_id,device_id,channel_id,default_level,default_faderate,pin
#ifdef XPL_LIGHTING_STDCONFIG
size_t xPL_LightingChannelConfig::printTo(Print& p) const
{
	size_t len = 0; //TODO emit
	len += _channel->network()->printlnTo(p,','); //48
	len += _channel->parent()->printlnTo(p,','); //34
	len += _channel->printlnTo(p,','); //30
	len += _channel->defaultLevel().printlnTo(p,','); //32
	len += _channel->defaultFaderate().printlnTo(p,','); //32
	len += xPL_String( _channel->Out() ).printTo(p); //20
	return len; 
}
#endif
#endif
/*
*  LightingChannelState
*/
xPL_LightingChannelState::xPL_LightingChannelState(xPL_LightingChannel* c):_channel(c) {}


/*
*  LightingChannelDyn
*/
xPL_LightingLevelDyn::xPL_LightingLevelDyn():sendTrigger(false),_levelStartup(0),_timeStartup(0) {}

time_t xPL_LightingLevelDyn::timeTarget() { return _timeStartup + faderate().toTime();}

int xPL_LightingLevelDyn::value()
{
	if (to16bits()!=_levelStartup)
	{
		time_t t=millis();
		if (timeTarget()-t < t-timeTarget()) // _timeTarget in the future (trick to prevent time counter cycles)
		{

			int val = _levelStartup;
			time_t total = timeTarget()-_timeStartup;
			time_t part = t-_timeStartup;

			if (to16bits()>_levelStartup)
				val += ( (part*(to16bits()-_levelStartup)) /total);
			else
				val -= ( (part*(_levelStartup-to16bits())) /total);
		
			return val;
		}
		sendTrigger=true;
		_levelStartup=to16bits();
	}
	return to16bits();
}

void xPL_LightingLevelDyn::cmdGoto(xPL_LightingLevel level)
{
	time_t t = millis(); //14
	_levelStartup = value(); //50
	_timeStartup  = t;
	xPL_LightingLevel::operator=(level);
	faderate() = level.faderate();
}





//channel=channel_id,dimmable,default_faderate,level
size_t xPL_LightingChannelState::printTo(Print& p) const
{
	return emit_p(p,
		PSTR("$,true,$,$"),
		_channel,
		//CStr::s_true(), //TODO : deal with non dimmable pins
		&_channel->defaultFaderate(),
		&_channel->level()
		);// 98
}

// TODO create an xPL_Printable that print a defined function of a node

/*
size_t xPL_LightingTrigger::printTo(Print& p)
{
	return xPL_Printable::emit_p(p,
			PSTR(
				"state=&\n"
				"level=$\n"				
				),
			_channel->level().toBit()?PSTR("on"):PSTR("off"),
			&_channel->level()
			);
}*/

bool xPL_LightingChannelPWM::loop()
{
	analogWrite(_pin,level().to8bits());
	if (_level.sendTrigger)
	{
		_level.sendTrigger=false;
		sendTrigger();
	}

	return false;
}
