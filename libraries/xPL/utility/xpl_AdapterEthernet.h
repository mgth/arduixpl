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
	void fromString(VString &s);
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
	size_t printBinTo(Print &p) const;
	void fromString(VString &s);
	void fromEeprom(xPL_Eeprom& eeprom);
	void toEeprom(xPL_Eeprom& eeprom);
	void toArray(uint8_t* mac);
	void setRandom();
};

class xPL_AdapterEthernet : public xPL_Schema
{
protected:
#ifdef XPL_CONFIG
	size_t printConfig(Print& p, bool list=false);
	void configure(xPL_Key& key);
	void config(xPL_Eeprom& eeprom,bool store=false);
#endif
	void defaultConfig();

	xPL_MacAddress _mac;

#ifdef XPL_IP
	xPL_IpAddress _ip;
	xPL_IpAddress _mask;
	bool _dhcp;
#endif

	size_t eventConfig(const xPL_Event& evt);

public:
	virtual bool connection() =0;
	virtual bool begin() =0;

};

#endif