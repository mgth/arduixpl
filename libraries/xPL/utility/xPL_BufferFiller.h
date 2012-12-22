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

	  Modified Aug 31, 2012 by Mathieu GRENET
*/

#ifndef XPL_BUFFERFILLER_H 
#define XPL_BUFFERFILLER_H

#include "xPL_Define.h"
#include "VString.h"

class xPL_BufferFiller : public Print {
  uint8_t *_start, *_ptr;

public:
  xPL_BufferFiller(size_t s);
  xPL_BufferFiller(uint8_t* buf);
  virtual ~xPL_BufferFiller();
      
  // HTML
#ifdef XPL_HTML_SUPPORT
  void printTag(const VString& t,bool close=false) {
	  print('<');
	  if (close) print('/');
	  print(t);
	  print('>');
  }

  void printTagClose(const VString& t) { printTag(t,true); }
  void printTagContent_P(const VString& tag, const VString& content)
  {
	  printTag(tag);
	  print(content);
	  printTagClose(tag);
  }
  void printTagContent(const VString& tag, const VString& content)
  {
	  printTag(tag);
	  print(content);
	  printTagClose(tag);
  }
#endif  

  size_t emit_p (PGM_P fmt, ...);

  uint8_t* buffer() const;
  size_t position() const;
  
  size_t write (uint8_t v);

  void unlink() { _start = _ptr = NULL; }

};

#endif