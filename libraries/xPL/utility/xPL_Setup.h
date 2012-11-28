#ifdef XPL_DEBUG
	Serial.begin(57600); 
	S(ArduixPL).printDebug();
#endif

#ifdef XPL_ADAPTERETHERNET_H
//	xPL.addChild( &xplAdapter );
	xPL.setAdapter( &xplAdapter );
#endif
/*#ifdef XPL_ADAPTERETHERCARD_H
	xPL.addChild( &xplAdapter );
	xPL.setAdapter( &xplAdapter );
#endif

#ifdef XPL_SCHEMACONFIG_H
	xPL.addChild( &xplConfig );
#endif
#ifdef XPL_SCHEMAGROUP_H
	xPL.addChild( &xplGroup );
#endif
#ifdef XPL_SCHEMAGROUP_H
	xPL.addChild( &xplFilter );
#endif

#ifdef XPL_SCHEMALIGHTING_H
	xPL.addChild(&xplLighting);
#endif
	*/
	