
#include <SPI.h>
#include <Ethernet.h>
#include <Dhcp.h>
#include <EthernetUdp.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <xPL.h>
//#include <xPL_Config.h>
#include "utility/xPL_Hbeat.h"

#include <xPL_AdapterW5100.h>
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

