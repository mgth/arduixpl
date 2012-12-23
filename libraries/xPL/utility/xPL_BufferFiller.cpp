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

