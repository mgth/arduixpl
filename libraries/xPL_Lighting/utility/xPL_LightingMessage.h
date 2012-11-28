#ifndef XPL_LIGHTINGMESSAGE_H
#define XPL_LIGHTINGMESSAGE_H

#include <xPL.h>
#include "../xPL_Lighting.h"

#include "xPL_LightingTypes.h"
#include "xPL_LightingNode.h"

#include "xPL_LightingChannel.h"
#include "xPL_LightingScene.h"


#include "utility/xPL_Message.h"

class xPL_LightingMessage : public xPL_Message {
public:

	xPL_LightingMessage():xPL_Message() { schema.schClass() = S(lighting); }

	xPL_LightingGateway* gateway();

	xPL_LightingLevel level(xPL_LightingLevel* lastLevel=NULL, xPL_LightingLevel* defaultLevel=NULL);

	bool isConfigCreate () { return isTypeCommand( S(config),S(create) ); }

	xPL_Message* statusMessage(xPL_LightingNode* node, const prog_char* reply,xPL_Key* k=NULL);

	bool configCreate(xPL_Node* node, const prog_char* field);


};

#endif