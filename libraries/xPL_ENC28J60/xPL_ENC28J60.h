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

	  Modified sept 07, 2012 by Mathieu GRENET
*/

#ifndef XPL_ENC28J60_H
#define XPL_ENC28J60_H


#include <xPL.h>
#include "utility/xPL_AdapterEthernet.h"

class xPL_ENC28J60 : public xPL_AdapterEthernet
{
private:
protected:
public:
	virtual bool begin();
	
	virtual bool connection();

	virtual bool loop();
	virtual bool sendMessage(xPL_Message& msg);

#ifdef XPL_HTML_SUPPORT
	virtual bool sendHomePage(); 
#endif


	byte initialize ();
	void initSPI ();
	void enableBroadcast ();
	void disableBroadcast ();

	word packetReceive();
	void packetRelease();

};

class VSHelperENC28J60 : public VStringHelper
{
private:
//	uint8_t* _ptr;
protected:
//	size_t write(uint8_t c);

//	VString from(VString& s);
	char charAt(const VString& s, size_t pos) const;

public:
//	VSHelperEeprom():_ptr(NULL) {};
//	bool setPos(size_t pos) { _ptr = (uint8_t*)pos; return true; }
	
	static VSHelperENC28J60 helper;
};

extern xPL_ENC28J60 xplAdapter;

#endif

