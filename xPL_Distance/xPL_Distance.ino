#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <xPL.h>
//#include <xPL_Config.h>
#include <utility/xPL_Hbeat.h>

#include <xPL_ENC28J60.h>
#include <xPL_Sensor.h>
#include <xPL_SensorUltrasonic.h>

#include <Arduino.h>

xPL_Hbeat xplHbeat;

unsigned long lasttime;

void setup()
{
//	Serial.begin(57600);
//  lcd.setCursor(13,2); lcd.print(F("[crash]"));
	delay(5000);
  lcd.init();
//  lcd.autoscroll();
  lcd.clear();
  lcd.print(F("=== boot ===="));
  lcd.print(sizeof(xPL_Node));
  printMemLCD();

  DBG(F("=== boot ===="),);
delay(5000);
//  DBG_MEM(F("start"));
 
  // Print a message to the LCD.
  lcd.backlight();
  xplAdapter.reg();
//	xplConfig.reg();
	xplHbeat.reg();
	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("dist"));


	xplSensor.addChild(new xPL_SensorUltrasonic(5,2,S(distance)));
//	lasttime=millis();
}

void loop()
{

	xPL.loop();
	
//	DBG(F("time"),millis()-lasttime);
//	lasttime=millis();
//	DBG_MEM(F("loop"));

}


