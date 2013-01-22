#include <xPL.h>
#include <xPL_ENC28J60.h>
//#include <xPL_Config.h>
//#include <xPL_Group.h>
//#include <xPL_Filter.h>
#include <xPL_Control.h>

#include <Arduino.h>


void setup()
{
	Serial.begin(57600);
	DBG(F("=== boot ===="),);
	DBG_MEM(F("start"));

	xplAdapter.reg();
//	xplConfig.reg();
//	xplGroup.reg();
//	xplFilter.reg();
	xplControl.reg();

	xPL.begin(F("arduixpl"),F("control"),F("default"));

	xplControl.addChild(new xPL_ControlPWM(F("pwm1"),2));
	xplControl.addChild(new xPL_ControlPWM(F("pwm2"),3));
	xplControl.addChild(new xPL_ControlPWM(F("pwm3"),4));
	xplControl.addChild(new xPL_ControlPWM(F("pwm4"),5));
	xplControl.addChild(new xPL_ControlPWM(F("pwm5"),6));
	xplControl.addChild(new xPL_ControlPWM(F("pwm6"),7));
	
	}

void loop()
{
	xPL.loop();
}
