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

#ifndef xPL_Config_h
#define xPL_Config_h

#include "xPL_Hbeat.h"

class xPL_Config : public xPL_Hbeat {

	const __FlashStringHelper* className() const { return S(config); }

	void loop();
	void parseMessage(xPL_MessageIn& msg);
	void configure(xPL_Key& key);
	void config(xPL_Eeprom& eeprom, bool store=false);
	void defaultConfig();

private:
	bool _configured; 
	bool sendConfig(const __FlashStringHelper* type) const;
	const __FlashStringHelper* hbeatClass() const { return _configured?xPL_Hbeat::className():className(); }
};

class xPL_MessageConfig : public xPL_Message
{
	bool _list;
	const __FlashStringHelper* schType() const { return _list?S(list):S(current); }	size_t printContentTo(Print& p) const;public:	xPL_MessageConfig(xPL_Config& hbeat,bool list=false):xPL_Message(hbeat),_list(list) {};};

#endif