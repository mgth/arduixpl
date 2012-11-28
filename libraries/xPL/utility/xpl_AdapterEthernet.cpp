/*
  Arbouyno - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of Arbouyno.

    Arbouyno is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Arbouyno is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Arbouyno.  If not, see <http://www.gnu.org/licenses/>.

	  Modified 21 Mars 2012 by Mathieu GRENET
*/

#include "xPL_AdapterEthernet.h"
#include "xPL_schema.h"




//char xPL_AdapterEthernet::_buffer[XPL_BUFFER_SIZE]; 
void xPL_MacAddress::setRandom()
{
	for(byte i=0; i<6; i++) bin[i]=random(256);
	bin[0]=( bin[0] & B11111110 ) | B00000010;
}
size_t xPL_IpAddress::printTo(Print& p) const
{
	return bin.printTo(p);
}

int xPL_IpAddress::toEeprom(xPL_Eeprom& eeprom)
{
	int i;
	for(i=0;i<4;i++)  eeprom.write(bin[i]);
	return i;
}
int xPL_IpAddress::fromEeprom(xPL_Eeprom& eeprom)
{
	int i;
	for(i=0;i<4;i++)  bin[i]=eeprom.read();
	return i;
}
int xPL_IpAddress::toArray(uint8_t* ip)
{
	int i;
	for(i=0;i<4;i++)  ip[i]=bin[i];
	return i;
}

void xPL_IpAddress::fromString(xPL_String &s)
{
	byte i=0;
	
	int pos=0;
	uint8_t val=0;

	char c = '1'; // dummy value not to be false;

	while (c && i<4)
	{
		c = s.charAt(pos);
		if ( c>='0' && c<='9') { val*=10; val += (char)(c-'0'); }
		else { bin[i++]=val; val=0; }

		pos++;
	}
}

char toHex(uint8_t b) { return (char)(b>9)?(b-10+'A'):(b+'0'); }

size_t xPL_MacAddress::printTo(Print& p) const
{
	int len = 0;
	 for (byte i=0;i<6;i++)
	 {
		len +=p.print(toHex(bin[i] >> 4));
		len += p.print(toHex(bin[i] & 0x0F));
		if (i<5) len += p.print(':');
	 }
	 return len;
}
void xPL_MacAddress::fromString(xPL_String &s)
{
	byte i=0;
	
	size_t pos=0;
	uint8_t val=0;

	char c = '1'; // dummy value not to be false;

	while (c && i<6)
	{
		c = s.lowerCharAt(pos);
		if ( c>='0' && c<='9') { val<<=4; val += (char)(c-'0'); }
		else if (c>='a' && c<='f') { val<<=4; val += (char)(c-'a'+ 10); }
		else { bin[i++]=val; val=0; }

		pos++;
	}
}

void xPL_MacAddress::fromEeprom(xPL_Eeprom& eeprom)
{
	eeprom.readAny(bin);
}

void xPL_MacAddress::toEeprom(xPL_Eeprom& eeprom)
{
	eeprom.writeAny(bin);
}
void xPL_MacAddress::toArray(uint8_t* mac)
{
	memcpy(mac,bin,6);
}


bool xPL_AdapterEthernet::loadConfig(xPL_Eeprom& eeprom)
{
	_mac.fromEeprom(eeprom);
	_ip.fromEeprom(eeprom);
	_mask.fromEeprom(eeprom);
	_dhcp=eeprom.read();
	begin();
	return false;
}

bool xPL_AdapterEthernet::loadDefaultConfig()
{
	_mac.setRandom();
	_dhcp = true;
	begin();
	return false;
}

bool xPL_AdapterEthernet::storeConfig(xPL_Eeprom& eeprom)
{
	_mac.toEeprom(eeprom);
	_ip.toEeprom(eeprom);
	_mask.toEeprom(eeprom);
	eeprom.write(_dhcp);
	return true;
}

bool xPL_AdapterEthernet::msgAddConfigList(xPL_Message& msg) {
	xPL_Adapter::msgAddConfigList(msg);

	msg.addOptionKey( S(mac_address) );
	msg.addOptionKey( S(ip_address) );
	msg.addOptionKey( S(ip_mask) );
	msg.addOptionKey( S(dhcp) );
	return true;
};

bool xPL_AdapterEthernet::msgAddConfigCurrent(xPL_Message& msg) {
	xPL_Adapter::msgAddConfigCurrent(msg);

	msg.addKey(
			S(mac_address),
			&_mac, 
			false
		);
	msg.addKey(
			S(ip_address),
			&_ip, 
			false
		);
	msg.addKey(
			S(ip_mask),
			&_mask, 
			false
		);
	msg.addKey( S(dhcp), _dhcp?S(on):S(off) );
	return true;
};

bool xPL_AdapterEthernet::configure(xPL_Key& key)
{
	if (key == S(mac_address))
	{
		_mac.fromString(key.sValue());
		return true;
	}
	else if  (key == S(ip_address))
	{
		_ip.fromString(key.sValue());
	}
	else if (key == S(ip_address))
	{
		_mask.fromString(key.sValue());
	}
	else if (key == S(dhcp))
	{
		_dhcp = key.sValue().toBool();
	}
	return xPL_Adapter::configure(key);
}







