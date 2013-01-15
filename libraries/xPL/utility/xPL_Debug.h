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

#ifndef XPL_DEBUG_H
#define XPL_DEBUG_H
#include "xPL_Define.h"

#ifdef XPL_DEBUG_LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#endif

#ifdef XPL_SLOWDEBUG
#define DBG(s,v) Serial.print(s);Serial.println(v); delay(500);
#elif defined(XPL_DEBUG)
#define DBG(s,v) Serial.print(s);Serial.println(v);
#else
#define DBG(s,v) ;
#endif

#define DBG_0(s,v) DBG(s,v);

#ifdef XPL_DEBUG_MEM
#define DBG_MEM(s) printMemCost(s); //delay(500);
#else
#define DBG_MEM(s) ;	
#endif

extern int  __bss_end;

//int memDelta =0;

struct __freelist
{
 size_t sz;
 struct __freelist *nx;
};

extern struct __freelist *__flp;
extern uint8_t* __brkval;

#define DELETE(o) delete o; // fix28135_malloc_bug();

//void fix28135_malloc_bug();

//extern "C" void   atexit( void );

long get_free_memory();
void printMemLCD();
long printMemCost(const __FlashStringHelper* msg);

extern LiquidCrystal_I2C lcd;
extern bool debug_flag;

#endif
