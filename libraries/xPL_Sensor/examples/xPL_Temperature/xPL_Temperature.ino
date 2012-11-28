//#define XPL_DEBUG
#define XPL_CONFIG		//  1372 Kb
//#define XPL_GROUP		//  1880 Kb
//#define XPL_FILTER		//  2170 Kb

#include <EtherCard.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <xPL.h>
#include <xPL_AdapterENC28J60.h>
#include <xPL_Sensor.h>

#include <Arduino.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;

float temp;

class xPL_Float:public xPL_Printable
{
private:
	float _val;
public:
	xPL_Float(float val) { _val=val; }
	size_t printTo(Print& p) const { return p.print(_val); }
};

void sendTrigger(float t)
{
	xPL_Message msg(S(trig),S(sensor),S(basic));
	msg.addKey(S(device),S(test));
	msg.addKey(S(type),S(temp));
	msg.addKey(S(current),new xPL_Float(temp),true);

	xplAdapter.sendMessage(msg);
}

int main(void)
{
	xplAdapter.reg();
	xplSensor.reg();
	xPL.begin();
	sensors.begin();

	sensors.getAddress(deviceAddress, 0); 
	sensors.setResolution(deviceAddress, 9);

	while(1)
	{
		xPL.loopAll();
		float newTemp = sensors.getTempC(deviceAddress);
		if (temp!=-127.0 && temp!=newTemp) sendTrigger(temp=newTemp);
	}
}