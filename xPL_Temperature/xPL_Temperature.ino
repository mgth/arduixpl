
#include <EtherCard.h>
//#include <SPI.h>
//#include <Ethernet.h>
//#include <Dhcp.h>
//#include <EthernetUdp.h>

//#include <OneWire.h>
//#include <DallasTemperature.h>

#include <xPL.h>
#include <xPL_Config.h>
//#include <xPL_AdapterENC28J60.h>
//#include <xPL_AdapterW5100.h>
//#include <xPL_Sensor.h>

#include <Arduino.h>

#define ONE_WIRE_BUS 7
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
//DeviceAddress deviceAddress;

void setup()
{
	Serial.begin(57600);
#ifdef XPL_DEBUG_MEM
		printMemCost((char*)"start");
#endif
//	xplAdapter.reg();
		printMemCost((char*)"adapter");
	xplConfig.reg();
		printMemCost((char*)"config");
/*	Serial.println("<2>");
	//xplSensor.reg();
	Serial.println("<3>");
	Serial.println("<4>");

	sensors.begin();
	Serial.println("<5>");
	sensors.getAddress(deviceAddress, 0); 
	Serial.println("<6>");
	sensors.setResolution(deviceAddress, 9);
	Serial.println("<7>");

	xPL_SensorTempOneWire t(deviceAddress);
	Serial.println("<8>");
	xplSensor.addChild(&t);
	Serial.println("<9>");
*/
	xPL.begin();


}

void loop()
{
	xPL.loopAll();
	#ifdef XPL_DEBUG_MEM
		printMemCost((char*)"mainloop");
	#endif
}