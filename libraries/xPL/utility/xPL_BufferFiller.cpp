#include "xPL_BufferFiller.h"
#include <stdarg.h>
#include <avr/eeprom.h>
/*
static char* wtoa (word value, char* ptr) {
  if (value > 9)
    ptr = wtoa(value / 10, ptr);
  *ptr = '0' + value % 10;
  *++ptr = 0;
  return ptr;
}
*/
  xPL_BufferFiller::xPL_BufferFiller(size_t s)  {
		_start= (uint8_t*)malloc(s);
		_ptr = _start;
  }

  xPL_BufferFiller::xPL_BufferFiller(uint8_t* buf)  {
	  _ptr = _start = buf;
  }

  xPL_BufferFiller::~xPL_BufferFiller()  {
	  if (_start) free(_start);
  }

  uint8_t* xPL_BufferFiller::buffer() const { return _start; }
  size_t xPL_BufferFiller::position() const { return _ptr - _start; }

  size_t xPL_BufferFiller::write (uint8_t v) {
	  if (v==0x0D) return 0;
	  *_ptr++ = v;
	  return 1;
  }

