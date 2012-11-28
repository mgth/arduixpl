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

	  Modified Aug 31, 2012 by Mathieu GRENET
*/

#ifndef xPL_LightingOUT_H
#define xPL_LightingOUT_H
#include "utility/xPL_Config.h"
#include "xPL_LightingTypes.h"

class xPL_LightingOutputMapper
{
public:
	virtual void begin(){};
	virtual bool setLevel(uint16_t out,xPL_LightingLevel value)=0;
protected:
	// TODO: implement LUT table;
};

/**************************************************************
Debug mapper, outputs to Serial only
**************************************************************/


class xPL_LightingOutputMapper_Debug : public xPL_LightingOutputMapper
{
private:
	uint8_t _values[10];
public:
	xPL_LightingOutputMapper_Debug()
	{
			for (int i=0;i<(int)sizeof(_values);i++) _values[i]=0;
	}
	virtual bool setLevel(uint16_t out,xPL_LightingLevel value);

};

/***************************************************************
 To ouput on an arduino UNO
 with Ethernet Shield on we have only 4 pins available for PWM
 and 6 for digital
 */
class xPL_LightingOutputMapper_Uno : public xPL_LightingOutputMapper
{
	virtual void begin() {};
	virtual bool setLevel(int out,xPL_LightingLevel value)
	{
		int pin=-1;
		
		switch(out)
		{
			//PWM
			case 0: pin=3; break;
			case 1: pin=5; break;
			case 2: pin=6; break;
			case 3: pin=9; break;
			//On/Off
			case 4: pin=0; break;
			case 5: pin=1; break;
			case 6: pin=2; break;
			case 7: pin=4; break;
			case 8: pin=7; break;
			case 9: pin=8; break;
		}
		if (pin>=0)
		{
			if (out<4)
			{
				analogWrite(pin,value.to8bits());
			}
			else
				digitalWrite(pin,value.toBit());
		}
		return false;
	}

};

class xPL_LightingOutputMapper_Mega : public xPL_LightingOutputMapper
{
	virtual bool setLevel(int out,xPL_LightingLevel value)
	{
		analogWrite(out,value.to8bits());
		return true;
	}
};


#endif