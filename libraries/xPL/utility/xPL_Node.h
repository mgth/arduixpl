#ifndef XPL_NODE_H
#define XPL_NODE_H

#include "xPL_Define.h"

#include "xPL_String.h"
#include "xPL_BufferFiller.h"
#include "xPL_Eeprom.h"
#include "xPL_Event.h"
class xPL_Main;
class xPL_Key;
class xPL_Message;
class xPL_Node;
class xPL_Schema;



class xPL_Node : public xPL_Printable {
private:
	xPL_Node* _parent;
	xPL_Node* _next;
	xPL_Node* _child;
	
	//int8_t _nextLevel;
  
protected:
	virtual bool targeted(xPL_Message& msg) { return false; }


public:

// Properties
	virtual const prog_char* className() const { return S(xpl); }
	virtual const xPL_String* id() const { return NULL; }

// Constructors
	xPL_Node(){
	 _parent =  _next = _child = NULL;
}


	virtual ~xPL_Node();
// Tree
	xPL_Node* parent() const;
	xPL_Node* next() const;
	xPL_Node* child() const;
	virtual xPL_Schema* schema() { return (parent())?parent()->schema():NULL; }

	virtual xPL_Node* add(const xPL_String& id) { return NULL; }
	xPL_Node* addChild(xPL_Node* node);
	xPL_Node* findOrAdd(const xPL_String& id);

	void deleteNode(xPL_Node* previous, bool deleteAll=false);
	void deleteChilds();

	xPL_Node* find(const xPL_String& cmpid);
	xPL_Node* findChild(const xPL_String& id);

	xPL_Node* defaultNode() const;
	xPL_Node* defaultChild() const;

	int count() const;

// events

	void sendEvent(xPL_Event* evt,bool childsOnly=false, bool all=false);
	template<class cls> void sendEvent( bool(cls::*func)(),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunction<cls> evt(func);
		return sendEvent(&evt,childsOnly,all);
	}
	template<class cls,class clsparam> void sendEvent( bool(cls::*func)(clsparam&),clsparam& param, bool childsOnly=false, bool all=false)
	{
		xPL_EventFunctionParam<cls,clsparam> evt(func,param);
		return sendEvent(&evt,childsOnly,all);
	}

	template<class cls> void sendEvent( bool(cls::*func)(),bool(cls::*funcClose)(),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunction<cls> evt(func,funcClose);
		return sendEvent(&evt,childsOnly,all);
	}

	template<class cls,class clsparam> void sendEvent( bool(cls::*func)(clsparam&),clsparam& param,bool(cls::*funcClose)(clsparam&),bool childsOnly=false, bool all=false)
	{
		xPL_EventFunctionParam<cls,clsparam> evt(func,funcClose);
		return sendEvent(&evt,childsOnly,all);
	}



	template<class cls> void sendEventConst( bool(cls::*func)(),bool childsOnly=false, bool all=false) const
	{
		xPL_EventFunction<cls> evt(func);
		return const_cast<xPL_Node*>(this)->sendEvent(&evt,childsOnly,all);
	}


	virtual bool loop(){ return true; }
	virtual bool parseMessage(xPL_Message& msg) { return false; }
	virtual bool checkTargeted(xPL_Message& msg) { return false; }

	void msgAddKey(const xPL_String& key,const xPL_Printable* value, bool alloc=false) const ;

//Config
	virtual xPL_Node* readConfig(xPL_Eeprom& eeprom);
	virtual bool loadConfig(xPL_Eeprom& eeprom);
	virtual bool storeConfig(xPL_Eeprom& eeprom);
	virtual void storeConfigClose(xPL_Eeprom& eeprom);

	virtual bool loadDefaultConfig() { return false; }

	virtual bool configure(xPL_Key& key) { return false; }
	virtual bool msgAddConfigList(xPL_Message& msg) { return false; }
	virtual bool msgAddConfigCurrent(xPL_Message& msg) { return false; }
//Printable
	virtual size_t printTo(Print& p) const {
		if (id()) 
			return p.print(*id());
		else return 0;
	}

//Debug

	void print();
};

class xPL_ChildsPrinter : xPL_Printable {

	xPL_Node* _node;

public:
	xPL_ChildsPrinter(xPL_Node& node) { _node=&node; }

	virtual size_t printTo(Print& p) const {

		class :public xPL_Event {
		public:
			size_t len;
			Print* print;

			virtual bool send(xPL_Node* n){ len += n->printlnTo(*print); return false; }
		} evt;

		evt.print=&p;
		evt.len=0;

		const_cast<xPL_Node*>(_node)->sendEvent(&evt,true);
		return evt.len;
	}
};


#endif
