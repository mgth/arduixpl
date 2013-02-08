#include <xPL.h>
#include <xPL_Control.h>
#include <xPL_ENC28J60.h>



void parserTest(xPL_MessageIn& msg)
{
	Serial.print(F("message !"));
}


unsigned long timer;
xPL_MessageGeneric* msg;

xPL_Float value(0,3);


void setup()
{
	Serial.begin(57600);
	DBG(F("=== boot ===="),);

	xplAdapter.reg();
	
	xPL.addChild(new xPL_SchemaGeneric(F("test"),parserTest));

	msg = new xPL_MessageGeneric(S(stat),F("test"),S(basic));
	msg->addKey(F("dummy"),F("nothing"));
	msg->addKey(F("value"),&value);


	xPL.begin(F("arduixpl"),F("test"),F("default"));
	timer = millis();
}

void loop()
{
	if (millis()-timer>1000)
	{
		// exemple d'envoi de message toutes les secondes.
		value=((float)millis())/1000.0;

		msg->send();

		timer=millis();
	}

	xPL.loop();
}
