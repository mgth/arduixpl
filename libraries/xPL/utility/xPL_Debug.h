#ifndef XPL_DEBUG_H
#define XPL_DEBUG_H
#include "xPL_Define.h"

extern int  __bss_end;

//int memDelta =0;

struct __freelist
{
 size_t sz;
 struct __freelist *nx;
};

extern struct __freelist *__flp;
extern uint8_t* __brkval;

#define DELETE(o) delete o; fix28135_malloc_bug();

void fix28135_malloc_bug();

extern "C" void   atexit( void );

int get_free_memory();
int printMemCost(char* msg);


#endif
