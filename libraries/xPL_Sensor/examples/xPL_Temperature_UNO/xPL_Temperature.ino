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

#include <OneWire.h>
#include <DallasTemperature.h>

#include <xPL.h>
//#include <xPL_Config.h>
#include "utility/xPL_Hbeat.h"
#include <xPL_ENC28J60.h>

#include <xPL_Sensor.h>

#include <Arduino.h>

#define ONE_WIRE_BUS 7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;

xPL_Hbeat xplHbeat;

unsigned long lasttime;

void setup()
{
#ifdef XPL_DEBUG
	Serial.begin(57600);
#endif
  DBG(F("=== boot ===="),);
  DBG_MEM(F("start"));

  xplAdapter.reg();
//	xplConfig.reg();
	xplHbeat.reg();

	xplSensor.reg();
	sensors.begin();

	xPL.begin(F("arduixpl"),F("sensors"),F("temp"));

	xplSensor.discoverOneWireTemp(sensors);

	lasttime=millis();
}

void loop()
{

	xPL.loopAll();
	
//	DBG(F("time"),millis()-lasttime);
//	lasttime=millis();
	DBG_MEM(F("loop"));

}

