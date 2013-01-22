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

#ifndef XPL_SENSORTOUCHPAD_H
#define XPL_SENSORTOUCHPAD_H
#include "utility/xPL_Define.h"
#include "../xPL_Sensor/xPL_Sensor.h"
#include "utility/Trackpad.h"

#define CLICLEVEL 10 // click level detect
#define TCLIC 300    // max clic duration
#define NOISE 0.1  // click error

class xPL_SensorTouchpad : public xPL_Node {
// initial pad bounds (values auto tuned while using)
// (absolute values, no orientation)
	Trackpad _trackpad;


	float _SlowDimmer; // quantity to dim for full pad high, one finger
	float _FastDimmer; // ... two fingers.

	unsigned int _xMin; 
	unsigned int _xMax;
	unsigned int _yMin;
	unsigned int _yMax;

	float _x,_y;      // position

	float _xStart; // position at click start
	float _yStart;
	unsigned long _tStart;     // start clic time

	float _xMove;  // move since click
	float _yMove;
	boolean _hFix; // axis follow detection
	boolean _vFix;

	byte _fingers;   // number of fingers on the pad
	byte _oldFingers;

	byte _orientation; // pad orientation

	xPL_SensorDimmer* _currentDimmer;

protected:
//	virtual const __FlashStringHelper* className() const { S(touchpad); }
	virtual void parseMessage(xPL_MessageIn& msg) { parseMessageChilds(msg); };

	virtual void loop();


public:
	xPL_SensorTouchpad():_trackpad(PS2_CLK_PIN,PS2_DATA_PIN)
	{
		_xMin =1126; _xMax=5797; _yMin=818; _yMax=4959;

		_xStart = _yStart=0;

		_xMove = _yMove=0;

		_SlowDimmer=50.0;
		_FastDimmer=100.0;

		_hFix=_vFix=true;

		_fingers=_oldFingers=0;

		_currentDimmer=NULL;
	}


	xPL_Node* add(const VString& id) { return addChild(new xPL_SensorDimmer(id)); }

};

#endif
