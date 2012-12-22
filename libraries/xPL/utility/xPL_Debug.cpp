#include "xPL_Debug.h"
#include "vstring.h"


uint8_t * heapptr, * stackptr;
void check_mem() {
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the memory again (sets stackptr to 0)
  stackptr =  (uint8_t *)(SP);           // save value of stack pointer
}


long get_free_memory()
{
	check_mem();
	return stackptr-heapptr;

/*	int free_memory;

	if((int)__brkval == 0)
		free_memory = ((int)&free_memory) - ((int)&__bss_end);
	else
		free_memory = ((int)&free_memory) - ((int)__brkval);

	return free_memory;*/
}

long printMemCost(const __FlashStringHelper* msg) {

	static long oldMem = XPL_RAM_SIZE;

	long newMem = get_free_memory();

	long mem = oldMem - newMem;
	if (mem)
	{
#ifdef XPL_DEBUG
		Serial.print(msg);
		Serial.print(F(" mem:"));
		Serial.print(oldMem);
		Serial.print('-');
		Serial.print(newMem);
		Serial.print(F(" cost:"));
		Serial.println(mem);
#endif
	}
	oldMem = newMem;
	return mem;
}
/*
void fix28135_malloc_bug()
 {
   for (__freelist *fp = __flp, *lfp = 0; fp; fp = fp->nx)
   {
     if (((uint8_t*)fp + fp->sz + 2) == __brkval)
     {
       __brkval = (uint8_t*)fp;
       if (lfp)
         lfp->nx = 0;
       else
         __flp = 0;
       break;
     }
     lfp = fp;
   }
 }


void   atexit( void ) {}
*/
