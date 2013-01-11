#include <ps2.h>
#include <Trackpad.h>

#include <xPL.h>
#include "utility/xPL_Hbeat.h"
#include <xPL_ENC28J60.h>

#include <Arduino.h>
xPL_Hbeat xplHbeat;
#include <xPL_Sensor.h>


unsigned int CLICLEVEL = 10; // click level detect
int TCLIC = 300;    // clic max time
float NOISE = 0.1 ; // click error
int ORIENTATION = 1;// pad orientation

float SlowDimmer = 50.0; // quantity to dim for full pad high, one finger
float FastDimmer = 200.0; // ... two fingers.

// initialize the library with the numbers of the interface pins
Trackpad t(6,5);

// initial pad bounds (values auto tuned while using)
// (absolute values, no orientation)
unsigned int xMin = 1126; 
unsigned int xMax = 5797;
unsigned int yMin = 818;
unsigned int yMax = 4959;

float x,y;      // position

float xStart=0; // position at click start
float yStart=0;
int tStart;     // start clic time

float xMove=0;  // move since click
float yMove=0;

boolean hFix=true; // axis follow detection
boolean vFix=true;

int fingers = 0;   // number of fingers on the pad
int oldFingers = 0;


class xPL_SensorDimmer : public xPL_Sensor {

	xPL_Float _dimmer;
	xPL_Float _tmpDimmer;
	bool _power;

	virtual const __FlashStringHelper* className() const { return F("dimmer"); }	virtual size_t printCurrentTo(Print& p) const	{		size_t l=0;		l += xPL_Message::printKeyTo(p,S(current),_tmpDimmer);		l += xPL_Message::printKeyTo(p,S(power),(_power)?S(on):S(off));		l += xPL_Message::printKeyTo(p,S(units),F("%"));		return l;	}public:	xPL_SensorDimmer(const VString& name):xPL_Sensor(name) { _dimmer=0.0; _tmpDimmer=0.0; _power=false; }	void setDimmer(float d) {		setTmpDimmer(d);
		_dimmer=_tmpDimmer;
	}

	void setTmpDimmer(float d) {		if (d<0.0) d=0.0;
		if (d>100.0) d=100.0;
		if (d!=_tmpDimmer)
		{
			_tmpDimmer = d;
			trig();
		}
	}

	void move(float v) {
		setTmpDimmer(_tmpDimmer + v);
	}

	void validate() { setDimmer(_tmpDimmer); }
	void cancel() { setTmpDimmer(_dimmer); }
	bool getPower() { return _power; }
	void setPower(bool p)
	{
		if (p!=_power) { _power=p; trig(); }
	}
};

xPL_SensorDimmer* curDimmer;

XPL_SETUP()
{
  DBG(F("=== boot ===="),);
  DBG_MEM(F("start"));

  xplAdapter.reg();
//	xplConfig.reg();
	xplHbeat.reg();

	xplSensor.reg();

	xPL.begin(F("arduixpl"),F("sensors"),F("switch"));

	curDimmer = (xPL_SensorDimmer*)xplSensor.addChild(new xPL_SensorDimmer(F("5")));
	xplSensor.addChild(new xPL_SensorDimmer(F("6")));
	xplSensor.addChild(new xPL_SensorDimmer(F("7")));
}

XPL_LOOP()
{
	xPL.loopAll();
	Serial.print(".");
		packet_t * packet;
		packet = t.getNewPacket();

			if (packet->z > CLICLEVEL) 
			{
			  if (packet->w >=4 )
			  {
				if (fingers<2) // si on a déjà détecté deux doigts, on garde : au relachement on passe régulièrement par un doigt
				  fingers = 1; // un seul doigt;
			  }
			  else if (packet->w == 0) fingers = 2; // deux doigts;
			  else fingers=0;
			}
			else fingers=0;

			if (packet->x > xMax) xMax = packet->x;
			if (packet->x < xMin && packet->x > 100 ) xMin = packet->x;
			if (packet->y > yMax) yMax = packet->y;
			if (packet->y < yMin && packet->y > 100 ) yMin = packet->y;
        
			switch(ORIENTATION)
			{
			  case(0):
			  {
				x = (float)(packet->x - xMin) / (xMax-xMin) ;
				y = (float)(packet->y - yMin) / (yMax-yMin) ;
				break;
			  }
			  case(1):
			  {
				y = (float)(xMax - packet->x) / (xMax-xMin) ;
				x = (float)(packet->y - yMin) / (yMax-yMin) ;
				break; 
			  }
			  case(2):
			  {
				x = (float)(xMax - packet->x) / (xMax-xMin) ;
				y = (float)(yMax - packet->y) / (yMax-yMin) ;
				break;
			  }
			  case(3):
			  {
				y = (float)(packet->x - xMin) / (xMax-xMin) ;
				x = (float)(yMax - packet->y) / (yMax-yMin) ;
				break; 
			  }
		   }
       
        
			if ( oldFingers == 0 )
			{
			  if ( fingers > 0 )      ///////////////////////////////////// start of clic
			  {
				tStart = millis();
            
				xStart = x;    yStart = y;
				xMove  = 0;    yMove  = 0;
				hFix   = true; vFix = true;
            
				oldFingers = fingers;
				curDimmer->cancel();
			  }
			}
			else
			{
			  if ( fingers == 0) /////////////////////////////////////////// end of click
			  {
				int tClic = millis() - tStart;  // durée du clic
            
				if ( hFix && vFix && tClic<=TCLIC )      curDimmer->setPower(!curDimmer->getPower());    // click
            
				if ( (hFix && tClic > TCLIC) || !vFix  ) curDimmer->validate(); // dim
				else curDimmer->cancel();
            
				if ( !hFix && vFix )                                         // dimmer selection
				{
				  if (xMove>0)
				  {
					curDimmer = (xPL_SensorDimmer*)curDimmer->nextLoop();
				  }
				  else 
				  {
					curDimmer = (xPL_SensorDimmer*)curDimmer->prevLoop();
				  }
					curDimmer->trig();
				}
             
				oldFingers = 0;
			  }
        
			  else  //////////////////////////////////////////////////////// during click
			  {
				int tClic = millis() - tStart;  // durée du clic
            
				xMove= x-xStart;
				hFix = ( abs(xMove) < NOISE );
				yMove= y-yStart;
				vFix = ( abs(yMove) < NOISE );
        
				if ( (hFix && tClic > TCLIC) || !vFix  )
				  curDimmer->move(yMove * (fingers==1 ? SlowDimmer : FastDimmer) );
				else
				  curDimmer->cancel();
              
			  }
			}



	}
	XPL_END