#include "xPL_AdapterW5100.h"

xPL_AdapterW5100 xplAdapter;

bool xPL_AdapterW5100::begin()
{
	if (_dhcp && Ethernet.begin(_mac.bin))
	{
		_ip.bin = Ethernet.localIP();
		_mask.bin = Ethernet.subnetMask();
	}
	else
	{
		Ethernet.begin(_mac.bin,_ip.bin,INADDR_NONE,INADDR_NONE,_mask.bin);
	}


	DBG(F("IP:"),_ip);

	return _udp.begin(XPL_PORT);
}

bool xPL_AdapterW5100::loop() {
			
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
	return false;
}

bool xPL_AdapterW5100::sendMessage(xPL_Message& msg)
{

	

	DBG(F("<send_W5100>"),);
	DBG(msg,);

	if (connection()) 
	{
		//uint8_t ip[4]={ 0xFF, 0xFF, 0xFF, 0xFF};

		IPAddress ip;
		for (byte i=0;i<4;i++) { ip[i]= (_ip.bin[i]&_mask.bin[i]) | (~_mask.bin[i]); } // TODO : maybe broadcast could be stored

		DBG(F("dest:"),ip);

		_udp.beginPacket(ip, XPL_PORT);

		//char* buffer = (char*)malloc( VString(msg).len() );

		xPL_BufferFiller buf( VString(&msg).len() );
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



