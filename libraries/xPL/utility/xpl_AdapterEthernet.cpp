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

#include "xPL_AdapterEthernet.h"
#include "xPL_schema.h"


void xPL_MacAddress::setRandom()
{
	randomSeed(analogRead(0));
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

void xPL_IpAddress::fromString(VString &s)
{
	byte i=0;
	
	int pos=0;
	char val=0;

	char c = '1'; // dummy value not to be false;

	while (c && i<4)
	{
		c = s.charAt(pos);
		if ( c>='0' && c<='9') { val*=10; val += (char)(c-'0'); }
		else { bin[i++]=val; val=0; }

		pos++;
	}
}

char toHex(char b) { return (b>9)?(b-10+'A'):(b+'0'); }

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

size_t xPL_MacAddress::printBinTo(Print& p) const
{
	for (byte i=0;i<6;i++) p.print(bin[i]);
	return 6;
}

void xPL_MacAddress::fromString(VString &s)
{
	byte i=0;
	
	size_t pos=0;
	uint8_t val=0;

	char c = '1'; // dummy value not to be false;

	while (c && i<6)
	{
		c = s.charAt(pos);
		if ( c>='0' && c<='9') { val<<=4; val += (char)(c-'0'); }
		else if (c>='a' && c<='f') { val<<=4; val += (char)(c-'a'+ 0xa); }
		else if (c>='A' && c<='F') { val<<=4; val += (char)(c-'A'+ 0xa); }
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
void xPL_MacAddress::toArray(char* mac)
{
	memcpy(mac,bin,6);
}

/*---------------------------------------------
Default
-----------------------------------------------*/
void xPL_AdapterEthernet::defaultConfig()
{
	_mac.setRandom();
#if defined(XPL_IP) && defined(XPL_DHCP)
	_dhcp = true;
#endif
	begin();
}

/*---------------------------------------------
Eeprom
-----------------------------------------------*/
#ifdef XPL_CONFIG
void xPL_AdapterEthernet::config(xPL_Eeprom& eeprom, bool store)
{
	if(store)
	{
		_mac.toEeprom(eeprom);
#ifdef XPL_IP
		_ip.toEeprom(eeprom);
		_mask.toEeprom(eeprom);
		eeprom.write(_dhcp);
#endif
	}
	else
	{
		_mac.fromEeprom(eeprom);
#ifdef XPL_IP
		_ip.fromEeprom(eeprom);
		_mask.fromEeprom(eeprom);
		_dhcp=eeprom.read();
#endif
		begin();
	}
}

size_t xPL_AdapterEthernet::printConfig(Print& p, bool list)
{
	size_t len = 0;

	if(list)
	{
		len += xPL_Message::printOptionKeyTo(p, S(mac_address) );
#ifdef XPL_IP
		len += xPL_Message::printOptionKeyTo(p, S(ip_address) );
		len += xPL_Message::printOptionKeyTo(p, S(ip_mask) );
		len += xPL_Message::printOptionKeyTo(p, S(dhcp) );
#endif
	}
	else
	{
		len += xPL_Message::printKeyTo( p, S(mac_address), &_mac );
#ifdef XPL_IP
		len += xPL_Message::printKeyTo( p, S(ip_address) , &_ip  );
		len += xPL_Message::printKeyTo( p, S(ip_mask),     &_mask);
		len += xPL_Message::printKeyTo( p, S(dhcp), _dhcp?S(on):S(off) );
#endif
	}
	return len;
}

/*---------------------------------------------
configure
-----------------------------------------------*/

void xPL_AdapterEthernet::configure(xPL_Key& key)
{
	if      (key.id == S(mac_address)) { _mac.fromString(key.value); }
#ifdef XPL_IP
	else if (key.id == S(ip_address))  { _ip.fromString(key.value); }
	else if (key.id == S(ip_address))  { _mask.fromString(key.value); }
	else if (key.id == S(dhcp))        { _dhcp = key.value.toBool(); }
#endif
}
#endif
