#include "xPL_Message.h"
#include "xPL_String.h"
#include "xPL_Adapter.h"

#include "xPL_Numeric.h"

xPL_Key::xPL_Key() {
	_allocated=false;
	_value=NULL;
}
xPL_Key::xPL_Key( xPL_String& buffer)
{
	_allocated=false;
	parse(buffer);
}

void xPL_Key::parse(xPL_String& buffer)
{
	if (_allocated)
	{
		DELETE(_value);
		_allocated=false;
	}
	_key = buffer;
	_key.parseTo('\n',buffer);
	_value = new xPL_String();
	if (_value)
	{
		_key.parseTo('=',sValue());
		_allocated=true;
	}
}

xPL_Key::~xPL_Key() { if (_allocated) { DELETE(_value) } }

bool xPL_Key::sendEventConfigure() {
		class :public xPL_Event {
		public:
			xPL_Key* key;
			virtual bool send(xPL_Node* n) { return n->configure(*key); } 
		} evt;

		evt.key = this;
#ifdef XPL_DEBUG
		Serial.println("parse:");
		Serial.println(*this);
#endif
		xPL.sendEvent(&evt);
		return false;
}

size_t xPL_Key::printTo(Print& p) const {
	return xPL_Printable::emit_p(p, PSTR("§=$"),&_key,_value);
}

size_t xPL_Address::printTo(Print& p) const {
	//return xPL_Printable::emit_p(p, PSTR("§-§.$"),&vendor,&device,&instance);
	size_t len=0;
	if (vendor) len+=vendor.printlnTo(p,'-');
	if (device) len+=device.printlnTo(p,'.');
	len+=instance.printTo(p);
	return len;
}

size_t xPL_String_Index::printTo(Print& p) const {
	return xPL_Printable::emit_p(p, PSTR("&[$]"),_key,&_index);
}

void xPL_Address::parse(const xPL_String& s) {
	vendor = s;
	vendor.parseTo('.',instance);
	if (!instance) {
		vendor.parseTo('-',instance); device.clear();
		if (!instance) { instance = vendor; vendor.clear(); }
	}
	else {
		vendor.parseTo('-',device);
		if (!device) { device=vendor; vendor.clear(); }	
	}
}

bool xPL_Address::operator==(const xPL_Address& addr) {
	if ( instance != addr.instance ) return false;
	if ( device != addr.device ) return false;
	if ( vendor != addr.vendor ) return false;
	return true;
}

xPL_Message::xPL_Message()
{
	_state.targeted = false;
	_state.matchFilter = true;
	_state.contentParsed = false;
}

xPL_Message::~xPL_Message() { deleteChilds(); }

xPL_Message::xPL_Message(const prog_char* mType,const prog_char* sClass,const prog_char* sType)
{
	msgType.msgVendor() = S(xpl);
	msgType.msgType() = mType;
	hop=1;
	source = xPL.source();
	target.setAny();
	schema.schClass() = sClass;
	schema.schType() = sType;
}

bool xPL_Message::isTypeCommand (const prog_char* type,const prog_char* cmd )
{
	return (schema.schType()==type && key_command()==cmd);
}

xPL_Key* xPL_Message::getKey(const prog_char* name)
{
	if (parseContent())
	{
		return (xPL_Key*)findChild(name);
	}
	return NULL;
}

xPL_String xPL_Message::getValue(const prog_char* s)
{
	xPL_String r;
	xPL_Key*k = getKey(s);
	if(k) r = k->sValue();
	return r;
}

xPL_Key* xPL_Message::getKeyCopy(const prog_char* name)
{
		xPL_String* value = new xPL_String;
		if (parent()) value->load( ((xPL_Message*)parent())->getValue(name) );

		xPL_Key* k= new xPL_Key( name, value, true);

		return k;
}


void xPL_Message::addKey(const prog_char* key,const xPL_Printable* value, bool alloc) {
	addChild(new xPL_Key(key,value,alloc)); 
}

void xPL_Message::addKey(const prog_char* key, const prog_char* value) {
	addKey(key, new xPL_String(value), true);
}

void xPL_Message::addOptionKey(const prog_char* value) {
	addKey(S(option), value);
}

void xPL_Message::addOptionKey(const prog_char* value,int index)
{
	addKey(
		S(option),
		new xPL_String_Index( value, index),
		true
	);
}

bool xPL_Message::parseHeader(char* buffer) {
     enum {
	  END, ERROR, MSG_TYPE, SCHEMA_NAME, START_SECTION, IN_SECTION
     } state = MSG_TYPE;

	_message.set(XPL_STRING_RAM,buffer);
	_state.contentParsed = true;

	xPL_Key k;

	do
	{
		k.parse(_message);
		if (!k.key()) { state=ERROR; }
		char c = k.key().charAt(0);

		switch(state)
		{
			case MSG_TYPE:
				msgType.parse(k.key()); 
				if (msgType.msgVendor() == S(xpl))
					state=START_SECTION;
				else
					state=ERROR;
				break;
			case SCHEMA_NAME:
				schema.parse(k.key()); 
				state=END;
				break;
			case START_SECTION:
				if (c=='{') state=IN_SECTION;
				break;
			case IN_SECTION:
				if (c=='}') { state=SCHEMA_NAME; }
				else
				{
						if ( k == S(hop) ) {
							hop=xPL_Int(k.sValue());
						}
						else if ( k == S(source) ) {
							source.parse(k.sValue());
						}
						else if ( k == S(target) ) {
							target.parse(k.sValue());
						}
				}
				break;
			case END:
			case ERROR:
				break;
		}
	} while(state>ERROR);

	return state==END;
}

bool xPL_Message::parseContent() //390
{
	if (_state.contentParsed) return true; //14

     enum {
	  END, ERROR, START_SECTION, IN_SECTION
     } state = START_SECTION;

	xPL_Key* k;
	
	do 
	{
		
		k = new xPL_Key(_message); //278
		if (!k) { state=ERROR; } //12

		char c = k->key().charAt(0);

		switch(state)
		{
			case START_SECTION:
				if (c=='{') state=IN_SECTION; //110
				else state=ERROR;
				break;
			case IN_SECTION:
				if (c=='}') { //56
					state=END;
				}
				else {
					addChild(k);
					k=NULL;
				}
				break;
			case END: break;
			case ERROR: break;
		}

		if (k) { DELETE(k); } //30

	} while(state>ERROR);

	_state.contentParsed=true;
	return state==END;
}

bool xPL_Message::parse(char* buffer) { 
	if (parseHeader(buffer)) return parseContent();
	return false;
}

size_t xPL_Message::printTo(Print& p) const //710
{
/*
  size_t l =0;

  l += p.println(msgType); //70
  l += p.println('{'); //16

  l += xPL_String(hop).printKey(p,S(hop)); //55

  l += source.printKey(p,S(source)); //22

  l += target.printKey(p,S(target)); //22

  l += p.println('}');//16
  l += schema.printlnTo(p); //22
  l += p.println('{'); //16

 xPL_EventPrintlnTo evt(p,l);

  const_cast<xPL_Message*>(this)->sendEvent(&evt,true); //148
          
  l += p.println('}');//16

  return l;
 */ 
	
  xPL_String h((int)hop);

  xPL_ChildsPrinter content(*(xPL_Node*)this);

	return xPL_Printable::emit_p(p,
		PSTR(
		"$\n"
		"{\n"
		"hop=$\n"
		"source=$\n"
		"target=$\n"
		"}\n"
		"$\n"
		"{\n"
		"$\n"
		"}\n"
		),
		&msgType,
		&h,
		&source,
		&target,
		&schema,
		&content
		);
		
}

bool xPL_Message::send(bool del){
	bool r = xPL.sendMessage(*this);
	if ( del ) { DELETE(this); }
	return r;
}