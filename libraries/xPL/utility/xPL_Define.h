#ifndef XPL_DEFINE_H
#define XPL_DEFINE_H

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

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

#if defined(RAMSIZE)
#define XPL_RAM_SIZE RAMSIZE
#elif defined(RAMSTART)
#define XPL_RAM_SIZE RAMEND-RAMSTART
#else
#define XPL_RAM_SIZE RAMEND-0x0FF
#endif

#if XPL_RAM_SIZE>3000
#define XPL_BUFFER_SIZE 1500 // if enough memory we can deal with full IP packet
#else
#define XPL_BUFFER_SIZE 300 // empiric min value 282
#endif


// buffer size for IP packets send/receive MEGA has enough memory to store a full IP packet but it must be smaller on UNO
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284P__)


// XPL_RESET_PIN : pin used to restore default config, if this pin is HIGH at boot default config is loaded
// but the config is actually stored only if device is configured.


// Options for Arduino Mega
#define XPL_RESET_PIN 46
#define XPL_LIGHTING_SCENES
//#define XPL_LIGHTING_STDCONFIG // lighting configuration will appear in config.list / config.response
#define XPL_LIGHTING_EXTCONFIG // Allow to configure via a new lighting.create schema
#else

// Options for Arduino Uno
#define XPL_RESET_PIN 4
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


#endif