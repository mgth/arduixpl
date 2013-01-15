/*
	trackpad.h
	Written by Joe Rickerby
*/

#ifndef Trackpad_h
#define Trackpad_h

#if ARDUINO >= 100
  #include <Arduino.h> // Arduino 1.0
#else
  #include <WProgram.h> // Arduino 0022
	#include "cppfix.h"
#endif
#include "ps2.h"

typedef struct _packet_t {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
	bool leftButtonDown;
	bool rightButtonDown;
} packet_t;

class Trackpad
{
	public:
		Trackpad(int clk, int data);
		~Trackpad();
		packet_t * getNewPacket();
	private:
		void sendComSeq(byte arg, boolean setMode = false);
		PS2 * mouse;
		packet_t packet;
};

#endif /* trackpad_h */

