#ifndef XPL_MESSAGE_H
#define XPL_MESSAGE_H

#include "xPL_Node.h"

class xPL_Key: public xPL_Node {
protected:
public:
//	virtual const __FlashStringHelper* className() const { return S(message); }	
	VString id;
	VString value;
	virtual bool is(const VString& cmpid) { return cmpid==id; }

	void parse(VString& buffer);

	xPL_Key(VString& buffer);
	xPL_Key(xPL_Key* key);
	xPL_Key() {};
	xPL_Key(const VString& key,const VString&  val):id(key),value(val) {}

	size_t printTo(Print& p) const;
};


class xPL_Address
{
public:

	VString vendor;
	VString device;
	VString instance;

	xPL_Address() {}
	xPL_Address(const VString& v,const VString& d,const VString& i)
		:vendor(v),device(d),instance(i){}
	void parse(const VString& s);

	void setAny() { instance=S(_asterisk); vendor.clear(); device.clear(); }
	bool isAny() { return instance.charAt(0)=='*'; } 

	bool operator==(const xPL_Address& addr);

	size_t printTo(Print& p) const;
};



class xPL_Message 
{
protected:	xPL_Node* _node;public:
	xPL_Message() {};
	xPL_Message(xPL_Node& node);

	static size_t printKeyTo(Print& p, const VString& key);
	static size_t printKeyTo(Print& p, const VString& key,const VString& value);
	static size_t printKeyTo(Print& p, const VString& key,const Printable& value);
	static size_t printKeyTo(Print& p, const VString& key,int value);
	static size_t printKeyTo(Print& p, const VString& key,float value,int dec=2);
	static size_t printOptionKeyTo(Print& p, const __FlashStringHelper* value);
	static size_t printOptionKeyTo(Print& p, const __FlashStringHelper* value,int index);

	size_t send();

	size_t printTo(Print& p) const;
	size_t len() { nullPrinter p; return printTo(p); }

	virtual size_t printContentTo(Print& p) const;
	virtual size_t printTargetTo(Print& p) const { return p.print('*'); }
	virtual const __FlashStringHelper* msgType() const { return S(stat); }	virtual const __FlashStringHelper* schClass() const;	virtual const __FlashStringHelper* schType() const { return S(basic); }};

class xPL_MessageGeneric : public xPL_Message
{
	const __FlashStringHelper* _msgType;
	const __FlashStringHelper* _schClass;
	const __FlashStringHelper* _schType;

	xPL_Address _target;

	xPL_Node _keys;

public:
	xPL_MessageGeneric(const __FlashStringHelper* msgType,const  __FlashStringHelper* schClass,const __FlashStringHelper* schType, const xPL_Address& target=xPL_Address()):
		_msgType(msgType),_schClass(schClass),_schType(schType),_target(target)
	{
	}


	xPL_Key* addKey(const VString& key,const VString& value) { return (xPL_Key*)_keys.addChild(new xPL_Key(key,value)); }

protected:
	virtual const __FlashStringHelper* msgType() const { return _msgType; }	virtual const __FlashStringHelper* schClass() const { return _schClass; }	virtual const __FlashStringHelper* schType() const { return _schType; }
	virtual size_t printContentTo(Print& p) const
	{
		size_t len;
		xPL_Node* n = const_cast<xPL_Node*>(&_keys)->child();
		while (n)
		{
			len += ((xPL_Key*)n)->printTo(p);
			len += p.print('\n');
			n=n->next();
		}
		return len;
	}
	virtual size_t printTargetTo(Print& p) const { return _target.printTo(p); }
};

class xPL_MessageIn {
private:
	VString _message;
	VString _input;
	xPL_Node _keys;

	struct
	{
		bool targeted : 1;
		bool matchFilter : 1 ;
		bool contentParsed : 1;
	} _state;

public:
	xPL_Address msgType;
	byte hop;
	xPL_Address source;
	xPL_Address target;
	xPL_Address schema;

	xPL_MessageIn(VString& buffer);

	void setTargeted(bool b=true )  { _state.targeted=b; }
	void setMatchFilter(bool b=true){ _state.matchFilter=b; }

	bool targeted() const { return _state.targeted && _state.matchFilter; }

	xPL_Key* getKey(const __FlashStringHelper* name);
	VString getValue(const __FlashStringHelper* name);
	xPL_Key* getKeyCopy(const __FlashStringHelper* name);

	xPL_Key* keys() { return (xPL_Key*)_keys.child(); }

	bool parseHeader();
	bool parseContent();

	bool parse();

	bool isTypeCommand (const __FlashStringHelper* type,const __FlashStringHelper* cmd );

#undef C
#undef CF
#define C(s) const VString key_##s() { return getValue(S(s)); }
#define CF(f,s) const VString key_##f() { return getValue(S(f)); }
#include "xPL_Stringlist.h"
};

#endif