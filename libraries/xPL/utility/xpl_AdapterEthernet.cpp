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


#include "xPL_AdapterEthernet.h"
#include "xPL_schema.h"




//char xPL_AdapterEthernet::_buffer[XPL_BUFFER_SIZE]; 
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
void xPL_MacAddress::toArray(uint8_t* mac)
{
	memcpy(mac,bin,6);
}

size_t xPL_AdapterEthernet::event(const xPL_Event& evt)
{
	size_t len=0;

	switch(evt.id())
	{
		case LOAD_CFG:
			if (evt.eeprom().isxPL())
			{
				_mac.fromEeprom(evt.eeprom());
				_ip.fromEeprom(evt.eeprom());
				_mask.fromEeprom(evt.eeprom());
				_dhcp=evt.eeprom().read();
				begin();
			}
			else
			{
				_mac.setRandom();
				_dhcp = true;
				begin();
			}
			break;

		case STORE_CFG:
			_mac.toEeprom(evt.eeprom());
			_ip.toEeprom(evt.eeprom());
			_mask.toEeprom(evt.eeprom());
			evt.eeprom().write(_dhcp);
			break;

		case CFG_LIST:
			//len += xPL_Adapter::printConfigListTo(p);

			len += xPL_Message::printOptionKeyTo(evt.print(), S(mac_address) );
			len += xPL_Message::printOptionKeyTo(evt.print(), S(ip_address) );
			len += xPL_Message::printOptionKeyTo(evt.print(), S(ip_mask) );
			len += xPL_Message::printOptionKeyTo(evt.print(), S(dhcp) );
			break;

		case CFG_CURRENT:
			//len += xPL_Adapter::printConfigCurrentTo(p);

			len += xPL_Message::printKeyTo( evt.print(), S(mac_address), &_mac );
			len += xPL_Message::printKeyTo( evt.print(), S(ip_address) , &_ip  );
			len += xPL_Message::printKeyTo( evt.print(), S(ip_mask),     &_mask);
			len += xPL_Message::printKeyTo( evt.print(), S(dhcp), _dhcp?S(on):S(off) );
			break;

		case CONFIGURE:
			{
				xPL_Key& key = evt.key();

				if      (key.id == S(mac_address)) { _mac.fromString(key.value); }
				else if (key.id == S(ip_address))  { _ip.fromString(key.value); }
				else if (key.id == S(ip_address))   { _mask.fromString(key.value); }
				else if (key.id == S(dhcp)) { _dhcp = key.value.toBool(); }
			}
			//xPL_Adapter::configure(key);
			break;
		default:
			break;
	}

	return len;
}







