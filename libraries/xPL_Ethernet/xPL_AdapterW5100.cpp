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


#include "xPL_AdapterW5100.h"

xPL_AdapterW5100 xplAdapter;

bool xPL_AdapterW5100::begin()
{
	
#ifdef XPL_DHCP
	if (_dhcp && Ethernet.begin(_mac.bin))
	{
		_ip.bin = Ethernet.localIP();
		_mask.bin = Ethernet.subnetMask();
	}
	else
	{
		Ethernet.begin(_mac.bin,_ip.bin,INADDR_NONE,INADDR_NONE,_mask.bin);
	}
#else
	for (byte i=0;i<4;i++) { _ip.bin[i] = 0;_mask.bin[i] = 0; }
	Ethernet.begin(_mac.bin,_ip.bin,INADDR_NONE,INADDR_NONE,_mask.bin);

#endif

	DBG(F("IP:"),_ip);

	return _udp.begin(XPL_PORT);
}

void xPL_AdapterW5100::loop() {
			
	size_t len = _udp.parsePacket();
		
	if (len)
	{
		char* buffer = (char*)malloc(len);

		if (buffer)
		{
			len = _udp.read(buffer,len);
		
			DBG(F("received:"),len);

			if (len)
			{
				VString msg(buffer);
				xPL.receivedMessage(msg);
			}
		}

		free(buffer);
	}
}

bool xPL_AdapterW5100::sendMessage(xPL_Message& msg)
{

	
#ifdef XPL_DEBUG
	DBG(F("<send_W5100>"),);
	msg.printTo(Serial);
#endif

	if (connection()) 
	{
		//uint8_t ip[4]={ 0xFF, 0xFF, 0xFF, 0xFF};

		IPAddress ip;
		for (byte i=0;i<4;i++) { ip[i]= (_ip.bin[i]&_mask.bin[i]) | (~_mask.bin[i]); } // TODO : maybe broadcast could be stored

		DBG(F("dest:"),ip);

		_udp.beginPacket(ip, XPL_PORT);

		//char* buffer = (char*)malloc( VString(msg).len() );

		xPL_BufferFiller buf( msg.len() );
		msg.printTo(buf);

		_udp.write(buf.buffer(),buf.position());
		
#ifdef XPL_DEBUG
		if (_udp.endPacket())
		{
			DBG(F("sent:"),buf.position());
			return true;
		}
		else
		{
			DBG(F("err:"),_udp.getWriteError());
			return false;
		}

#else
		return _udp.endPacket();
#endif

		return true;
	}

	DBG(F("no connection"),);

	return false;
}

bool xPL_AdapterW5100::connection()
{
/*  if (ether.dhcpExpired())
  {
	  if (!ether.dhcpSetup()) return false;
  }*/
  return true;
}

#ifdef XPL_HTML_SUPPORT

bool xPL_AdapterEthernet::sendHomePage() {
/*
	if (!_state==XA_STATE_READY) return false;

  _bfill = ether.tcpOffset();
  _bfill.emit_p(PSTR(
  "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<title>xPL</title>"
    "<h1>xPL $F-$F.$S</h1>"
//    "<p>..TODO: some state here..</p>"
    ),
	VendorID(),
	DeviceID(),
	_source
	);
  xPL_Schema* sch = _schema;
  while (sch)
  {
	  _bfill.emit_p(PSTR(
	  "<li>$F</li>"),
	  sch->Family()
	  );
	  sch=sch->next();
  }
  ether.httpServerReply(_bfill.position()); // send web page data
  */
  return true;
}
#endif



