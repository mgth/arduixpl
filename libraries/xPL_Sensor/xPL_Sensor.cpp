#include "xPL_Sensor.h"

xPL_SchemaSensor xplSensor;
xPL_Sensor_Message::xPL_Sensor_Message(xPL_Sensor& sensor,const __FlashStringHelper* msgType,const VString& request):xPL_Message(msgType,S(sensor),S(basic)),_sensor(&sensor){	if (request==S(current)) _request=current;	else if (request==S(name)) _request=name;	else _request=none;}size_t xPL_Sensor_Message::printContentTo(Print& p) const {	size_t l = 0;	l += printKey(p,S(device),*_sensor->id());	l += printKey(p,S(type),_sensor->className());	switch(_request) {	case current:		l += _sensor->printCurrent(p);		break;	case name:		l += printKey(p,S(name),*id());		break;	default:		break;	}	return l;}xPL_SensorTemp::xPL_SensorTemp(const VString& name):xPL_Sensor(name) {	_publicTemp=0.0;	_temp=0.0;	_highestTemp=0.0;	_lowestTemp=0.0;	_threshold=0.1;	_reset=true;}size_t xPL_SensorTemp::printCurrent(Print& p) const{	size_t l=0;	l += xPL_Message::printKey(p,S(current),_temp);	l += xPL_Message::printKey(p,S(lowest),_lowestTemp);	l += xPL_Message::printKey(p,S(highest),_highestTemp);	l += xPL_Message::printKey(p,S(units),F("c"));	_publicTemp=_temp;	return l;}void xPL_SensorTemp::setTemp(float t) {	_temp=t;	if (_highestTemp<t || _reset) _highestTemp=t;	if (_lowestTemp>t || _reset) _lowestTemp=t;	_reset=false;	if (abs((float)_temp-(float)_publicTemp)>=_threshold)	{		xPL_Sensor_Message(*this,S(trig),S(current)).send();
	}}xPL_SensorTempOneWire::xPL_SensorTempOneWire(DallasTemperature& sensors, DeviceAddress addr, const VString & name):xPL_SensorTemp(name), _sensors(&sensors){	for (byte i=0;i<8;i++) _addr[i] = addr[i];	sensors.setResolution(_addr, _resolution); // TODO: parametric resolution	sensors.setWaitForConversion(false);	_sensors->requestTemperatures();	lastrequest=millis();}bool xPL_SensorTempOneWire::loop(){	if((millis()-lastrequest)> _wait)	{		setTemp(_sensors->getTempC(_addr));		_sensors->requestTemperatures();		lastrequest=millis();	}		 		return false;}
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

bool xPL_Sensor::parseMessage(xPL_MessageIn& msg) {	if (!targeted(msg)) return false;	if ( msg.schema.instance == S(request) )	{		VString req = msg.key_request();		DBG_MEM(F("av:"));		msg.deleteChilds();		DBG_MEM(F("ap:"));		xPL_Sensor_Message(*this,S(stat),req).send();

		DBG_MEM(F("a2:"));
	}	return false;}bool xPL_SchemaSensor::parseMessage(xPL_MessageIn& msg){	if (!targeted(msg)) return false;	return true;};void xPL_SchemaSensor::discoverOneWireTemp(DallasTemperature& sensors){	int n = sensors.getDeviceCount();	DBG(F("sensors:"),n);	for (int i=0;i<n;i++)	{		DeviceAddress addr;		if (sensors.getAddress(addr,i))		{			// TODO : check for already registered sensor, need to have a way to recognise 1wire sensors (group ?)			// TODO : find a better way to handle allocated vstrings			char* name = (char*)malloc(2);			name[0]='0'+i; name[1]='\0';			addChild(new xPL_SensorTempOneWire(sensors,addr, name )); 		}	}}