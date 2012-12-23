#include "xPL_Sensor.h"

xPL_SchemaSensor xplSensor;
xPL_Sensor_Message::xPL_Sensor_Message(xPL_Sensor& sensor,const __FlashStringHelper* msgType,const VString& request):xPL_Message(msgType,S(sensor),S(basic)),_sensor(&sensor)
	}
bool xPL_Sensor::targeted(xPL_MessageIn& msg)
{
	VString s = msg.key_device();
	if (!s) return false;
	if (!id() || s!=*id()) return false;
	s=msg.key_type();
	if (!s) return true;
	if (s==className()) return true;
	return false;
}

bool xPL_Sensor::parseMessage(xPL_MessageIn& msg) {

		DBG_MEM(F("a2:"));
	}