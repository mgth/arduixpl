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

	  Modified 07 Sept 2012 by Mathieu GRENET
*/

#include "xPL_AdapterENC28J60.h"
#include <EtherCard.h>

xPL_AdapterENC28J60 xplAdapter;

byte Ethernet::buffer[XPL_BUFFER_SIZE];

bool xPL_AdapterENC28J60::begin()
{	
Serial.println("<begin>");

	_mac.toArray(ether.mymac);
	ether.begin(sizeof Ethernet::buffer, EtherCard::mymac, ENC28J60_PIN);

#ifdef XPL_DHCP
	if (_dhcp && ether.dhcpSetup())
	{
		_ip.bin=ether.myip;
		_mask.bin=ether.mymask;

	}
	else
#endif	
	{
		_ip.toArray(ether.myip);
		_mask.toArray(ether.mymask);
		ether.staticSetup(); }

	ether.enableBroadcast();

	return true;
}

bool xPL_AdapterENC28J60::connection()
{
#ifdef XPL_DHCP
  if (!ether.using_dhcp)
  {
	  if (!ether.dhcpSetup()) return false;
  }
#endif
  return true;
}


bool xPL_AdapterENC28J60::loop()
{
	return false; //TODO Remove
	if (!connection()) { return false; }

	word receiveLen = ether.packetReceive();
	if (!receiveLen) { return false; }

#ifdef XPL_DEBUG
	Serial.print("received:");
	Serial.println(receiveLen);
#endif
#ifdef XPL_HTML_SUPPORT
  word receiveTCP = ether.packetLoop(receiveLen);

  if(receiveTCP &&
	 ether.bufferSize &&
     ether.buffer[IP_PROTO_P] == IP_PROTO_TCP_V && 
     ether.buffer[UDP_DST_PORT_H_P] == 0x00 && 
     ether.buffer[UDP_DST_PORT_L_P] == 0x50) 
	{
		char* data = (char *) Ethernet::buffer + receiveTCP;

		if (strncmp("GET / ", data, 6) == 0) 
		{
#ifdef XPL_DEBUG
			S(html).printDebug();
#endif
			sendHomePage();
		}
		
		return true;
	}
#endif

  // UDP port 3865 -> xPL
  if(ether.bufferSize &&
     ether.buffer[IP_PROTO_P] == IP_PROTO_UDP_V && 
     ether.buffer[UDP_DST_PORT_H_P] == highByte(XPL_PORT) && 
     ether.buffer[UDP_DST_PORT_L_P] == lowByte(XPL_PORT) ) 
    {
		Serial.println("<xPL>");
		xPL.receivedMessage((char*)ether.buffer+UDP_DATA_P);
    }

	return true;
}



bool xPL_AdapterENC28J60::sendMessage(xPL_Message& msg) {

#ifdef XPL_DEBUG
		Serial.println(F("<send>"));
		Serial.print(msg);
#endif	

	ether.udpPrepare(XPL_PORT, ether.myip, XPL_PORT);
    ether.copyMac(ether.buffer + ETH_SRC_MAC, ether.mymac);
    for (int i=0;i<6;i++) ether.buffer[ETH_DST_MAC + i] = 0x0FF;

    ether.buffer[IP_TOTLEN_L_P]=0x82;
    ether.buffer[IP_PROTO_P]=IP_PROTO_UDP_V;
    for (int i=0;i<4;i++) ether.buffer[IP_DST_P+i] = 0x0FF;
	
	xPL_BufferFiller buffer = ether.buffer + UDP_DATA_P;

	buffer.print(msg);

	if (connection()) 
	{
		ether.udpTransmit(buffer.position());
#ifdef XPL_DEBUG
		Serial.print(PSTR("sent:"));
		Serial.println(buffer.position());
#endif
		return true;
	}
	return false;
}


#ifdef XPL_HTML_SUPPORT

bool xPL_AdapterEtherCard::sendHomePage() {

  xPL_BufferFiller buf = ether.tcpOffset();

  buf.print(S(html_header));

   buf.printTagContent_P(S(title),S(xPL));
    buf.printTag(S(h1));
    S(xPL).printlnTo(buf,':');
	source().printlnTo(buf);
	buf.printTagClose(S(h1));
//    "<p>..TODO: some state here..</p>"
  xPL_Schema* sch = (xPL_Schema*)child();
  while (sch)
  {
	  buf.printTagContent(S(li),sch->className());
	  sch=(xPL_Schema*)sch->next();
  }
  ether.httpServerReply(buf.position()); // send web page data
  
  return true;
}
#endif
 
 