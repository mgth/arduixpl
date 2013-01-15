#include <xPL.h>
#include "utility/xPL_Hbeat.h"
#include <xPL_ENC28J60.h>

#include <Arduino.h>
xPL_Hbeat xplHbeat;
#include <xPL_Sensor.h>
#include <xPL_SensorTouchpad.h>

xPL_SensorTouchpad xplTouchpad;


void setup()
{
	Serial.begin(57600);
	DBG(F("=== boot ===="),);
  DBG_MEM(F("start"));

  xplAdapter.reg();
//	xplConfig.reg();
	xplHbeat.reg();

	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("switch"));

	xplSensor.addChild(&xplTouchpad);

	xplTouchpad.add(F("1"));
	xplTouchpad.add(F("2"));

}

void loop()
{
	xPL.loop();
}
	//XPL_END