
#include <SPI.h>
#include <Ethernet.h>
#include <Dhcp.h>
#include <EthernetUdp.h>


#include <xPL.h>
//#include <xPL_Config.h>
#include "utility/xPL_Hbeat.h"

#include <xPL_AdapterW5100.h>
#include <xPL_Sensor.h>
#include <xPL_SensorDallas.h>
#include <xPL_SensorDHT.h>

#include <Wire.h>
#include <xPL_SensorBMP085.h>

#include <Arduino.h>

xPL_Hbeat xplHbeat;

unsigned long lasttime;

void setup()
{
	Serial.begin(57600);
  DBG(F("=== boot ===="),);
  DBG_MEM(F("start"));

  xplAdapter.reg();
//	xplConfig.reg();
	xplHbeat.reg();

	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("meteo"));

	xPL_SensorDallasTemp::discoverOneWireTemp(xplSensor,S(temp));

	xplSensor.addChild(new xPL_SensorDHT(6, 11, F("dht11t"), F("dht11h")));
	xplSensor.addChild(new xPL_SensorBMP085(F("bmp085t"), F("bmp085p")));
	lasttime=millis();
}

void loop()
{

	xPL.loopAll();
	
//	DBG(F("time"),millis()-lasttime);
//	lasttime=millis();
	DBG_MEM(F("loop"));

}


