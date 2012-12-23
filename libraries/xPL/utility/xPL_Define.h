/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of ArduixPL.

    ArduixPL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduixPL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.

	  Modified Dec 23, 2012 by Mathieu GRENET
*/

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