#include "xPL_LightingMessage.h"
#include "xPL_LightingNetwork.h"

xPL_LightingLevel xPL_LightingMessage::level(xPL_LightingLevel* lastLevel, xPL_LightingLevel* defaultLevel)
{
	
	xPL_LightingLevel val;
	xPL_String s;

	s = key_level();
	if (defaultLevel && s==S(default)) val = *defaultLevel;
	else if (lastLevel && s==S(last)) val = *lastLevel;
	else val.fromString(s);

	s = key_fade_rate();
	if (defaultLevel && s==S(default)) val.faderate() = defaultLevel->faderate();
	else val.faderate().fromString(s);

	return val; 
}


xPL_Message* xPL_LightingMessage::statusMessage(xPL_LightingNode* node, const prog_char* reply, xPL_Key* k)
{
	xPL_Message* msgStatus;

	if ( schema.schType()==S(request) )
	{
		if ( key_request()==reply)
		{
			msgStatus = new xPL_Message(S(stat),S(lighting),reply);
			if (msgStatus)
			{
				const prog_char* status;
				if (k)
				{
					status = S(not_found);
					msgStatus->addChild(k); 
				}
				else
				{
					status = S(ok);
					node->msgAddIdPath(*this);
				}
				msgStatus->addKey( S(status),status );
			}
			return msgStatus;
		}
	}
	return NULL;
}

bool xPL_LightingMessage::configCreate(xPL_Node* node, const prog_char* field)
{
	if ( isConfigCreate() )
	{
		xPL_String d = getValue(field);

		if (d) { node->findOrAdd(d); }
		else return false;
	}
	return true;
}


//xPL_LightingGateway* xPL_LightingMessage::gateway() { return xPL_SchemaLighting::instance().gateway; }
