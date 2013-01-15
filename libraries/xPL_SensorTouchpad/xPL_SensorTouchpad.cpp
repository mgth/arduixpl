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

	  Modified 20 Mars 2012 by Mathieu GRENET
*/
#include "xPL_SensorTouchpad.h"


void xPL_SensorTouchpad::loop() 
{
	packet_t * packet;
	packet = _trackpad.getNewPacket();

	if (!_currentDimmer) {
		_currentDimmer=(xPL_SensorDimmer*)child();
		if (!_currentDimmer) return;
	}

	if (packet->z > CLICLEVEL) 
	{
		if (packet->w >=4 )
		{
		if (_fingers<2) // si on a déjà détecté deux doigts, on garde : au relachement on passe régulièrement par un doigt
			_fingers = 1; // un seul doigt;
		}
		else if (packet->w == 0) _fingers = 2; // deux doigts;
		else _fingers=0;
	}
	else _fingers=0;

	if (packet->x > _xMax) _xMax = packet->x;
	if (packet->x < _xMin && packet->x > 100 ) _xMin = packet->x;
	if (packet->y > _yMax) _yMax = packet->y;
	if (packet->y < _yMin && packet->y > 100 ) _yMin = packet->y;
        
	switch(_orientation)
	{
		case(0):
		{
		_x = (float)(packet->x - _xMin) / (_xMax-_xMin) ;
		_y = (float)(packet->y - _yMin) / (_yMax-_yMin) ;
		break;
		}
		case(1):
		{
		_y = (float)(_xMax - packet->x) / (_xMax-_xMin) ;
		_x = (float)(packet->y - _yMin) / (_yMax-_yMin) ;
		break; 
		}
		case(2):
		{
		_x = (float)(_xMax - packet->x) / (_xMax-_xMin) ;
		_y = (float)(_yMax - packet->y) / (_yMax-_yMin) ;
		break;
		}
		case(3):
		{
		_y = (float)(packet->x - _xMin) / (_xMax-_xMin) ;
		_x = (float)(_yMax - packet->y) / (_yMax-_yMin) ;
		break; 
		}
	}
       
        
	if ( _oldFingers == 0 )
	{
		if ( _fingers > 0 )      ///////////////////////////////////// start of clic
		{
		_tStart = millis();
            
		_xStart = _x;    _yStart = _y;
		_xMove  = 0;    _yMove  = 0;
		_hFix   = true; _vFix = true;
            
		_oldFingers = _fingers;
		_currentDimmer->cancel();
		}
	}
	else
	{
		if ( _fingers == 0) /////////////////////////////////////////// end of click
		{
		int tClic = millis() - _tStart;  // durée du clic
            
		if ( _hFix && _vFix && tClic<=TCLIC )      _currentDimmer->toggle();    // click
            
		if ( (_hFix && tClic > TCLIC) || !_vFix  ) _currentDimmer->validate(); // dim
		else _currentDimmer->cancel();
            
		if ( !_hFix && _vFix )                                         // dimmer selection
		{
			if (_xMove>0)
			{
			_currentDimmer = (xPL_SensorDimmer*)_currentDimmer->nextLoop();
			}
			else 
			{
			_currentDimmer = (xPL_SensorDimmer*)_currentDimmer->prevLoop();
			}
			_currentDimmer->trig();
		}
             
		_oldFingers = 0;
		}
        
		else  //////////////////////////////////////////////////////// during click
		{
		int tClic = millis() - _tStart;  // durée du clic
            
		_xMove= _x-_xStart;
		_hFix = ( abs(_xMove) < NOISE );
		_yMove= _y-_yStart;
		_vFix = ( abs(_yMove) < NOISE );
        
		if ( (_hFix && tClic > TCLIC) || !_vFix  )
			_currentDimmer->move(_yMove * (_fingers==1 ? _SlowDimmer : _FastDimmer) );
		else
			_currentDimmer->cancel();
              
		}
	}

}