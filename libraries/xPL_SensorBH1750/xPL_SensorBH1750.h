/*  ArduixPL - xPL library for Arduino(tm)  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.  This file is part of ArduixPL.    ArduixPL is free software: you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation, either version 3 of the License, or    (at your option) any later version.    ArduixPL is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.	  Modified 2013-2-19 by Mathieu GRENET 	  mailto:mathieu@mgth.fr	  http://www.mgth.fr*/#ifndef _XPL_SENSORBH1750_h
#define _XPL_SENSORBH1750_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "../xPL_Sensor/xPL_Sensor.h"#include "../xPL/utility/VString.h"class xPL_SensorBH1750:public xPL_SensorGeneric {
	int _addr;public:	virtual const __FlashStringHelper* className() const { return F("illuminance"); }	virtual const __FlashStringHelper* units() const { return F("lx"); }	xPL_SensorBH1750(const VString & name,int addr);	void loop();};
#endif

