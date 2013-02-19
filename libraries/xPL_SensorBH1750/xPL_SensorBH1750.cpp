/*
  ArduixPL - xPL library for Arduino(tm)
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

	  Modified 2013-2-19 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/
#include "xPL_SensorBH1750.h"
#include <Wire.h> 

xPL_SensorBH1750::xPL_SensorBH1750(const VString & name,int addr):xPL_SensorGeneric(name),_addr(addr){	setBuffer(3);	setThreshold(2.0);	Wire.beginTransmission(_addr);
	Wire.write(0x10);//1lx reolution 120ms
	Wire.endTransmission();}void xPL_SensorBH1750::loop(){	int i=0;
	byte buff[2];

	Wire.beginTransmission(_addr);
	Wire.requestFrom(_addr, 2);
	while(Wire.available()) //
	{
		buff[i] = Wire.read();  // receive one byte
		i++;
	}
	Wire.endTransmission();  
	setValue(((buff[0]<<8)|buff[1])/1.2);}