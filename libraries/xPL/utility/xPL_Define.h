#ifndef XPL_DEFINE_H
#define XPL_DEFINE_H

//#undef PROGMEM
//#define PROGMEM __attribute__((section(".progmem.data")))

#define XPL_DEBUG
#define XPL_DEBUG_MEM

#define ENC28J60_PIN 10

#define XPL_DHCP 
//#define XPL_FLOAT

#define XPL_CONFIG_INTERVAL 1
#define XPL_HBEAT_INTERVAL 5

// xPL default port, do not change
#define XPL_PORT 3865

#if defined(__AVR_ATmega168__)
#define XPL_RAM_SIZE 1024
#elif defined(__AVR_ATmega328__)
#define XPL_RAM_SIZE 2048
#elif defined(__AVR_ATmega1280__)
#define XPL_RAM_SIZE 131072
#elif defined(__AVR_ATmega2560__)
#define XPL_RAM_SIZE 262144
#elif defined(__AVR_ATmega1284P__)
#define XPL_RAM_SIZE 16384
#else
#define XPL_RAM_SIZE 2048
#endif

// buffer size for IP packets send/receive MEGA has enough memory to store a full IP packet but it must be smaller on UNO
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284P__)


// XPL_RESET_PIN : pin used to restore default config, if this pin is HIGH at boot default config is loaded
// but the config is actually stored only if device is configured.


// Options for Arduino Mega
#define XPL_RESET_PIN 46
#define XPL_BUFFER_SIZE 1500 // on mega we can deal with full IP packet
#define XPL_LIGHTING_SCENES
//#define XPL_LIGHTING_STDCONFIG // lighting configuration will appear in config.list / config.response
#define XPL_LIGHTING_EXTCONFIG // Allow to configure via a new lighting.create schema
#else

// Options for Arduino Uno
#define XPL_RESET_PIN 4
#define XPL_BUFFER_SIZE 350 // empiric min value 324
#define XPL_LIGHTING_SCENES
//#define XPL_LIGHTING_STDCONFIG // lighting configuration will appear in config.list / config.response
#define XPL_LIGHTING_EXTCONFIG // Allow to configure via a new lighting.create schema
#endif

#if defined(XPL_LIGHTING_STDCONFIG) ||  defined(XPL_LIGHTING_EXTCONFIG)
#define XPL_LIGHTING_CONFIG
#endif

#define XPL_MAX_GROUPS 3
#define XPL_MAX_FILTERS 3

#define XPL_LIGHTING_MAX_CHANNELS 16
#define XPL_LIGHTING_MAX_SCENES 32


#include <Arduino.h>
#include "xPL_Debug.h"

#ifdef XPL_DEBUG
#endif

#endif