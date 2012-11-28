#include "xPL_AdapterW5100.h"

xPL_AdapterW5100 xplAdapter;
//char xPL_AdapterW5100::_buffer[XPL_BUFFER_SIZE];

bool xPL_AdapterW5100::begin()
{
	if (_dhcp && Ethernet.begin(_mac.bin))
	{
		_ip.bin = Ethernet.localIP();
		_mask.bin = Ethernet.subnetMask();
	}
	else
	{
		Ethernet.begin((uint8_t*)_mac.bin,_ip.bin,INADDR_NONE,INADDR_NONE,_mask.bin);
	}

#ifdef XPL_DEBUG
	Serial.print("IP:");
	Serial.println(_ip);
#endif
//	_buffer[0]='\0';
	return _udp.begin(XPL_PORT);
}

bool xPL_AdapterW5100::loop() {

	int len = _udp.parsePacket();
		
	if (len)
	{
		len = _udp.read(_buffer,sizeof(_buffer));
		if (len) xPL.receivedMessage(_buffer);     
	}
	return false;
}

bool xPL_AdapterW5100::sendMessage(xPL_Message& msg)
{

	xPL_BufferFiller buf = (byte*)_buffer;
	
	buf.print(msg);

#ifdef XPL_DEBUG
	Serial.println(F("<send>"));
		msg.printTo(Serial);
#endif	
	if (connection()) 
	{
		uint8_t ip[4]={ 0xFF, 0xFF, 0xFF, 0xFF};
		_udp.beginPacket(ip, XPL_PORT);
		_udp.write((uint8_t*)_buffer,buf.position());
		_udp.endPacket(); 
		return true;
	}
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



