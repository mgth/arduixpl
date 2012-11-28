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