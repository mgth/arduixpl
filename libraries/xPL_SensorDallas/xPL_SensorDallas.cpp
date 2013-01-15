// 
// 
// 

#include "xPL_SensorDallas.h"

OneWire xPL_SensorDallasTemp::_oneWire(ONE_WIRE_BUS);
DallasTemperature xPL_SensorDallasTemp::_sensors(&_oneWire);

void xPL_SensorDallasTemp::discoverOneWireTemp(xPL_Schema& schema,const VString& prefix){	_sensors.begin();	int n = _sensors.getDeviceCount();	DBG(F("sensors:"),n);	for (int i=0;i<n;i++)	{		DeviceAddress addr;		if (_sensors.getAddress(addr,i))		{			// TODO : check for already registered sensor, need to have a way to recognise 1wire sensors (group ?)			// TODO : find a better way to handle allocated vstrings			// TODO : should be a standard function to create names			char* name = (char*)malloc(prefix.len()+2);			size_t pos;			for(pos=0;pos<prefix.len();pos++) name[pos]=prefix.charAt(pos);			name[pos++]='0'+i;			name[pos++]='\0';			schema.addChild(new xPL_SensorDallasTemp(addr, VString(name) )); 		}	}}xPL_SensorDallasTemp::xPL_SensorDallasTemp(DeviceAddress addr, const VString & name):xPL_SensorGeneric(name){	for (byte i=0;i<8;i++) _addr[i] = addr[i];	_sensors.setResolution(_addr, _resolution); // TODO: parametric resolution	_sensors.setWaitForConversion(false);	_sensors.requestTemperatures();	lastrequest=millis();}void xPL_SensorDallasTemp::loop(){	if((millis()-lastrequest)> _wait)	{		setValue(_sensors.getTempC(_addr));		_sensors.requestTemperatures();		lastrequest=millis();	}}