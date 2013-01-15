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


#ifndef XPL_CONFIG_H
#define XPL_CONFIG_H

#include <xPL.h>
#include "utility/xPL_Hbeat.h"

class xPL_Config : public xPL_Hbeat {
private:
		bool _configured; 

protected:
	virtual bool parseMessage(xPL_MessageIn& msg);

	virtual bool configure(xPL_Key& key);
	virtual bool loadConfig(xPL_Eeprom& eeprom);
	virtual bool loadDefaultConfig();

	bool sendConfig(const __FlashStringHelper* type) const;

public:
	virtual const __FlashStringHelper* className() const { return S(config); }



};
extern xPL_Config xplConfig;

#endif