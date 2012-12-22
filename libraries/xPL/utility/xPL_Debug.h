#ifndef XPL_DEBUG_H
#define XPL_DEBUG_H
#include "xPL_Define.h"

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
long printMemCost(const __FlashStringHelper* msg);


#endif
