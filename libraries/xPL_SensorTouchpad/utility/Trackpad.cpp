/*
  trackpad.cpp - Library for interfacing with synaptic trackpads.
  Created by Joe Rickerby, July 2008.
  Released into the public domain.
*/
#include "Trackpad.h"

#define BITPOS(n) (1<<(n))
#define DUMP(d) Serial.print(#d ": ");\
Serial.print(d,HEX);Serial.print(" ");\
Serial.print(d,BIN);Serial.println()

Trackpad::Trackpad(int clk, int data)
{
	//I've created my own new operator, see the cppfix.cpp file
	mouse = new PS2(clk,data);
	
	mouse->write(0xff);	// reset
	mouse->read();	 // ack byte
	mouse->read();	 // blank 
	mouse->read();	 // blank 
	mouse->write(0xf0);	// remote mode
	mouse->read();	 // ack
	
	//set mode byte- absolute mode, wmode
	sendComSeq(0x81, true);
	delayMicroseconds(100);
}

Trackpad::~Trackpad()
{
	delete mouse;
}

//Details are available in the synaptic interfacing guide- this is a bit of a nasty one.
//Synaptic special commands are hidden in Set Resolution commands, so that the data will make
//it through the BIOS on a PC unscathed.
void Trackpad::sendComSeq(byte arg, boolean setMode)
{
	
	mouse->write(0xe8);
	mouse->write((arg & 0xc0)>>6);
	mouse->read();	 // ack byte
	mouse->write(0xe8);
	mouse->write((arg & 0x30)>>4);
	mouse->read();	 // ack byte
	mouse->write(0xe8);
	mouse->write((arg & 0x0c)>>2);
	mouse->read();	 // ack byte
	mouse->write(0xe8);
	mouse->write(arg & 0x03);
	mouse->read();	 // ack byte

	if(setMode == false) {
		mouse->write(0xe9);
		mouse->read();	 // ack byte
		/*
		//If you want to read a ComSeq, you can grab the data here. I'm not actually using
		//these ComSeqs for anything but setting mode bits, so I've left it out.
		d1=mouse->read();
		d2=mouse->read();
		d3=mouse->read();
		*/
	} else {
		mouse->write(0xf3);
		mouse->read();	 // ack byte
		mouse->write(0x14);
		mouse->read();	 // ack byte
	}

}

/*
	See the Synaptic interfacing guide for the derivation of all that bitwise nonsense
	down there.
*/
packet_t * Trackpad::getNewPacket()
{
	byte p1;
	byte p2;
	byte p3;
	byte p4;
	byte p5;
	byte p6;
	
	mouse->write(0xeb);	 // give me data!
	mouse->read();			// ignore ack
	p1 = mouse->read();
	p2 = mouse->read();
	p3 = mouse->read();
	p4 = mouse->read();
	p5 = mouse->read();
	p6 = mouse->read();
	
	packet.x = p5;
	packet.x |= (p2 & 0x0f) << 8;
	packet.x |= (p4 & 0x10) << 8;
	
	packet.y = p6;
	packet.y |= ((p2 & 0xf0) << 4);
	packet.y |= ((p4 & 0x20) << 7);
	
	packet.z = p3;
	
	packet.w = 0;
	packet.w |= (p4 & 0x04) >> 2;
	packet.w |= (p1 & 0x04) >> 1;
	packet.w |= (p1 & 0x30) >> 2;
	
	packet.leftButtonDown = p1 & 0x01;
	packet.rightButtonDown = (p1 & 0x02) >> 1;

	return &packet;
}
