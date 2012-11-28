#include "xPL_Debug.h"
#include "xpl_String.h"


uint8_t * heapptr, * stackptr;
void check_mem() {
  stackptr = (uint8_t *)malloc(4);          // use stackptr temporarily
  heapptr = stackptr;                     // save value of heap pointer
  free(stackptr);      // free up the memory again (sets stackptr to 0)
  stackptr =  (uint8_t *)(SP);           // save value of stack pointer
}


int get_free_memory()
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

int printMemCost(char* msg) {

	static int oldMem = XPL_RAM_SIZE;

	int newMem = get_free_memory();

	int mem = oldMem - newMem;
	if (mem)
	{
#ifdef XPL_DEBUG
		Serial.print(msg);
		xPL_String(S(mem)).printlnTo(Serial,':');
		Serial.print(oldMem);
		Serial.print('-');
		Serial.print(newMem);
		Serial.print(' ');
		xPL_String(S(cost)).printlnTo(Serial,':');
		Serial.println(mem);
#endif
	}
	oldMem = newMem;
	return mem;
}

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

