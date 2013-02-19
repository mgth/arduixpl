#include <xPL.h>

#include <xPL_ENC28J60.h>
#include <xPL_Sensor.h>
#include <Wire.h> 
#include <xPL_SensorBH1750.h>

#include <Arduino.h>

void setup()
{
	Serial.begin(57600);

	DBG(F("=== boot ===="),);
	DBG_MEM(F("start"));

    xplAdapter.reg();
	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("luxmeter"));

	xplSensor.addChild(new xPL_SensorBH1750(F("bh1750"),0x23));
}

void loop()
{
	xPL.loop();
	
	DBG_MEM(F("loop"));

}


