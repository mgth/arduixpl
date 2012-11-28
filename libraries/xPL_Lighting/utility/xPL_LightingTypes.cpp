#include "xPL_LightingTypes.h"

void xPL_LightingFaderate::store(uint16_t val, uint8_t dec) {
	_fadetime= val*1000/divider(dec);
}

uint16_t xPL_LightingFaderate::load(uint8_t dec) const {
	return (_fadetime*divider(dec)) /1000;
}

void xPL_LightingLevel::store(uint16_t val, uint8_t dec) {
	_level= ((val * maxLevel) / (divider(dec)*100));
}

uint16_t xPL_LightingLevel::load(uint8_t dec) const {
	return ((long)value() * (divider(dec)*100)) / maxLevel;
}

void xPL_LightingLevel::fromEeprom(xPL_Eeprom& eeprom) {  eeprom.readAny(_level); faderate().fromEeprom(eeprom); }
void xPL_LightingLevel::toEeprom(xPL_Eeprom& eeprom) { eeprom.writeAny(_level); faderate().toEeprom(eeprom); }
