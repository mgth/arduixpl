#include "xPL_Sensor.h"

xPL_SchemaSensor xplSensor;

#define ONE_WIRE_BUS 2

OneWire xPL_SensorTempOneWire::oneWire(ONE_WIRE_BUS);
DallasTemperature xPL_SensorTempOneWire::sensors(&oneWire);

bool xPL_Sensor::targeted(xPL_Message& msg)
{
	xPL_String s = msg.key_device();
	if (!s) return false;
	if (s!=*id()) return false;
	s=msg.key_type();
	if (!s) return true;
	if (s==className()) return true;
	return false;
}
