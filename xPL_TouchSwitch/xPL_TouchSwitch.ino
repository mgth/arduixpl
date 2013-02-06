/*		ArduixPL - xPL library for Arduino(tm)		Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.		This file is part of ArduixPL.		ArduixPL is free software: you can redistribute it and/or modify		it under the terms of the GNU General Public License as published by		the Free Software Foundation, either version 3 of the License, or		(at your option) any later version.		ArduixPL is distributed in the hope that it will be useful,		but WITHOUT ANY WARRANTY; without even the implied warranty of		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		GNU General Public License for more details.		You should have received a copy of the GNU General Public License		along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.		Modified 2013-2-4 by Mathieu GRENET 		mailto:mathieu@mgth.fr		http://www.mgth.fr
*/

#include <xPL.h>
#include <xPL_ENC28J60.h>

#include <Arduino.h>

#include <xPL_Sensor.h>
#include <xPL_SensorTouchpad.h>

xPL_SensorTouchpad xplTouchpad;


XPL_SETUP()
{
	DBG(F("=== boot ===="),);
	DBG_MEM(F("start"));

	xplAdapter.reg();
	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("switch"));

	xplSensor.addChild(&xplTouchpad);

	xplTouchpad.add(F("1"));
	xplTouchpad.add(F("2"));
}

XPL_LOOP()
{
	xPL.loop();
}

XPL_END