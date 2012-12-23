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

#ifndef XPL_EVENT_H
#define XPL_EVENT_H

#include "xPL_Define.h"

class xPL_Node;
class xPL_Message;

class xPL_Event {
public:
	virtual bool send(xPL_Node* n)=0;
	virtual void close(xPL_Node* n) { }
};

template <class cls> class xPL_EventFunction:public xPL_Event {
private:
	bool (cls::*_func)();
	bool (cls::*_funcClose)();

public:
	xPL_EventFunction(bool(cls::* func)(),bool(cls::* funcClose)()=NULL) { _func=func; _funcClose=funcClose;  }
	virtual bool send(xPL_Node* node) { return (((cls*)node)->*_func)(); }
	virtual void close(xPL_Node* node) { if (_funcClose)  (((cls*)node)->*_funcClose)(); } 
};

template <class cls,class clsparam> class xPL_EventFunctionParam:public xPL_Event {
private:
	bool (cls::*_func)(clsparam&);
	bool (cls::*_funcClose)(clsparam&);
	clsparam* _param;

public:
	xPL_EventFunctionParam(bool(cls::* func)(clsparam&),clsparam& param, bool(cls::* funcClose)(clsparam&)=NULL)
	{
		_func=func;
		_funcClose=funcClose; 
		_param = &param;
	}
	virtual bool send(xPL_Node* node) { return (((cls*)node)->*_func)(*_param); } 
	virtual void close(xPL_Node* node) { if (_funcClose)  (((cls*)node)->*_funcClose)(*_param); } 
};


#endif