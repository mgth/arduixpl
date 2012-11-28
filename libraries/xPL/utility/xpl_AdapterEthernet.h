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

	  Modified Oct 26, 2012 by Mathieu GRENET
*/

#ifndef XPL_ADAPTERETHERNET_H
#define XPL_ADAPTERETHERNET_H


#include <xPL.h>

#include "xPL_BufferFiller.h"
#include "IPAddress.h"
#include "xPL_Adapter.h"

class xPL_IpAddress : public xPL_Printable
{
private:
public:
	IPAddress bin;
	virtual size_t printTo(Print& p) const;
	void fromString(xPL_String &s);
	int toEeprom(xPL_Eeprom& eeprom);
	int toArray(uint8_t* ip);
	int fromEeprom(xPL_Eeprom& eeprom);
};

class xPL_MacAddress : public xPL_Printable
{
private:
public:
	uint8_t bin[6];
	virtual size_t printTo(Print& p) const;
	void fromString(xPL_String &s);
	void fromEeprom(xPL_Eeprom& eeprom);
	void toEeprom(xPL_Eeprom& eeprom);
	void toArray(uint8_t* mac);
	void setRandom();
};

class xPL_AdapterEthernet : public xPL_Adapter
{
protected:

	xPL_MacAddress _mac;
	xPL_IpAddress _ip;
	xPL_IpAddress _mask;
	bool _dhcp;

	xPL_BufferFiller _bfill;


//configuration
//EEPROM
	virtual bool loadConfig(xPL_Eeprom& eeprom);
	virtual bool loadDefaultConfig();
	virtual bool storeConfig(xPL_Eeprom& eeprom);
//Messages
	virtual bool msgAddConfigList(xPL_Message& msg);
	virtual bool msgAddConfigCurrent(xPL_Message& msg);
	virtual bool configure(xPL_Key& key);

public:
	virtual bool connection() =0;
	virtual bool begin() =0;
	virtual bool sendMessage(xPL_Message& msg)=0;
#ifdef XPL_HTML_SUPPORT
	virtual bool sendHomePage()=0; 
#endif

};

#endif