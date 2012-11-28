

//#define XPL_DEBUG
#define XPL_CONFIG		//  1372 Kb
#define XPL_GROUP		//  1880 Kb
#define XPL_FILTER		//  2170 Kb

#include <EtherCard.h>
//#include <SPI.h>
//#include <Ethernet.h>
//#include <Dhcp.h>
//#include <EthernetUdp.h>


#include <xPL.h>
#include <xPL_AdapterENC28J60.h>
//#include <xPL_AdapterW5100.h>
#include <utility/xPL_SchemaConfig.h>
#include <xPL_Group.h>
#include <xPL_Filter.h>
#include <xPL_Lighting.h>

#ifdef XPL_LIGHTING_H
xPL_LightingOutputMapper_Debug out;
#endif

#include <Arduino.h>


//void setup()

int main(void)
{
	int test = 1;
	#ifdef XPL_DEBUG
		Serial.begin(57600);
	#endif

	xplAdapter.reg();
	xplConfig.reg();
	xplFilter.reg();
	xplGroup.reg();
	xplLighting.reg();

	xplLighting.setMapper(&out);
	/*
		xPL_LightingNetwork* n = (xPL_LightingNetwork*)xplLighting.gateway.add("n1");

		xPL_LightingDevice* d1 = (xPL_LightingDevice*)n->devices.add("d1");
				xPL_LightingChannel* d1c1 = d1->addChannel("1",1,100,0);
				xPL_LightingChannel* d1c2 = d1->addChannel("2",2,100,0);
				xPL_LightingChannel* d1c3 = d1->addChannel("3",3,100,0);

			xPL_LightingDevice* d2 = (xPL_LightingDevice*)n->devices.add("d2");
				xPL_LightingChannel* d2c1 = d2->addChannel("1",4,100,0);
				xPL_LightingChannel* d2c2 = d2->addChannel("2",5,100,0);
			
	#ifdef XPL_LIGHTING_SCENES
		xPL_LightingScene* s1 = (xPL_LightingScene*)n->scenes.add("s1");
			s1->add(d1c2,75,0);
			s1->add(d2,50,0);

		xPL_LightingScene* s2 = (xPL_LightingScene*)n->scenes.add("s2");
			s2->add(d1,100,0);
			s2->add(d2c2,100,0);
	#endif
			*/

	xPL.begin();


	#ifdef XPL_DEBUG
		xPL.print();
	#endif

	#ifdef XPL_DEBUG_MEM
		printMemCost((char*)"start");
	#endif
	//}

	//void loop()
	while (1)
	{
		xPL.loopAll();
		test *= 2;
		if (test>1000) xPL.loopAll();

	#ifdef XPL_DEBUG_MEM
		printMemCost((char*)"mainloop");
	#endif
	}
}