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

	  Modified Jan 20, 2013 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/

#ifndef xPL_ENC28J60_h
#define xPL_ENC28J60_h


#include <xPL.h>
#include "utility/xPL_AdapterEthernet.h"

class xPL_ENC28J60 : public xPL_AdapterEthernet
{
private:
	size_t printETH(Print& p);
	size_t printIP(Print& p, word totallen, word chksum=0);
	size_t printUDP(Print& p, xPL_Message& data, word len_udp, word chksum=0);

protected:
public:
	virtual bool begin();
	
	virtual bool connection();

	void loop();
	bool sendMessage(xPL_Message& msg);


	byte initialize ();
	void initSPI ();
	void filterXpl();
	void filterXplCmnd();

	word packetReceive();
	void packetRelease();

};

class VSHelperENC28J60 : public VStringHelper
{
protected:
	char charAt(size_t pos, const VString& s) const;

public:
	
	static VSHelperENC28J60 helper;
};

extern xPL_ENC28J60 xplAdapter;

#endif

