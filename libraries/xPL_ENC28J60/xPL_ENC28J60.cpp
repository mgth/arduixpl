/*
  ArduixPL - xPL library for Arduino(tm)
  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.

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

	  Modified 2013-2-4 by Mathieu GRENET 
	  mailto:mathieu@mgth.fr
	  http://www.mgth.fr
*/
#include "xPL_ENC28J60.h"
#include "../xPL/utility/xPL_Debug.h"

//ERXFCON 0b01110000
//EMPOH:EPMOL = 0
//24h=0F:19
//2Ah=78:70:2B:6C
//32h=0A:7B:0A

//00:00:00:00:0C:3B:38
//<<388F

xPL_ENC28J60 xplAdapter;

VSHelperENC28J60 VSHelperENC28J60::helper;

 /*******************************************************************

 *******************************************************************/
static int gNextPacketPtr;
static byte Enc28j60Bank;

// ENC28J60 Control Registers
// Control register definitions are a combination of address,
// bank number, and Ethernet/MAC/PHY indicator bits.
// - Register address        (bits 0-4)
// - Bank number        (bits 5-6)
// - MAC/PHY indicator        (bit 7)
#define ADDR_MASK        0x1F
#define BANK_MASK        0x60
#define SPRD_MASK        0x80
// All-bank registers
#define EIE              0x1B
#define EIR              0x1C
#define ESTAT            0x1D
#define ECON2            0x1E
#define ECON1            0x1F
// Bank 0 registers
#define ERDPT           (0x00|0x00)
#define EWRPT           (0x02|0x00)
#define ETXST           (0x04|0x00)
#define ETXND           (0x06|0x00)
#define ERXST           (0x08|0x00)
#define ERXND           (0x0A|0x00)
#define ERXRDPT         (0x0C|0x00)
// #define ERXWRPT         (0x0E|0x00)
#define EDMAST          (0x10|0x00)
#define EDMAND          (0x12|0x00)
// #define EDMADST         (0x14|0x00)
#define EDMACS          (0x16|0x00)
// Bank 1 registers
#define EHT0             (0x00|0x20)
#define EHT1             (0x01|0x20)
#define EHT2             (0x02|0x20)
#define EHT3             (0x03|0x20)
#define EHT4             (0x04|0x20)
#define EHT5             (0x05|0x20)
#define EHT6             (0x06|0x20)
#define EHT7             (0x07|0x20)
#define EPMM0            (0x08|0x20)
#define EPMM1            (0x09|0x20)
#define EPMM2            (0x0A|0x20)
#define EPMM3            (0x0B|0x20)
#define EPMM4            (0x0C|0x20)
#define EPMM5            (0x0D|0x20)
#define EPMM6            (0x0E|0x20)
#define EPMM7            (0x0F|0x20)
#define EPMCS           (0x10|0x20)
#define EPMO            (0x14|0x20)
#define EWOLIE           (0x16|0x20)
#define EWOLIR           (0x17|0x20)
#define ERXFCON          (0x18|0x20)
#define EPKTCNT          (0x19|0x20)
// Bank 2 registers
#define MACON1           (0x00|0x40|0x80)
#define MACON2           (0x01|0x40|0x80)
#define MACON3           (0x02|0x40|0x80)
#define MACON4           (0x03|0x40|0x80)
#define MABBIPG          (0x04|0x40|0x80)
#define MAIPG           (0x06|0x40|0x80)
#define MACLCON1         (0x08|0x40|0x80)
#define MACLCON2         (0x09|0x40|0x80)
#define MAMXFL          (0x0A|0x40|0x80)
#define MAPHSUP          (0x0D|0x40|0x80)
#define MICON            (0x11|0x40|0x80)
#define MICMD            (0x12|0x40|0x80)
#define MIREGADR         (0x14|0x40|0x80)
#define MIWR            (0x16|0x40|0x80)
#define MIRD            (0x18|0x40|0x80)
// Bank 3 registers
#define MAADR1           (0x00|0x60|0x80)
#define MAADR0           (0x01|0x60|0x80)
#define MAADR3           (0x02|0x60|0x80)
#define MAADR2           (0x03|0x60|0x80)
#define MAADR5           (0x04|0x60|0x80)
#define MAADR4           (0x05|0x60|0x80)
#define EBSTSD           (0x06|0x60)
#define EBSTCON          (0x07|0x60)
#define EBSTCS          (0x08|0x60)
#define MISTAT           (0x0A|0x60|0x80)
#define EREVID           (0x12|0x60)
#define ECOCON           (0x15|0x60)
#define EFLOCON          (0x17|0x60)
#define EPAUS           (0x18|0x60)

// ENC28J60 ERXFCON Register Bit Definitions
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01
// ENC28J60 EIE Register Bit Definitions
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40
#define EIE_DMAIE        0x20
#define EIE_LINKIE       0x10
#define EIE_TXIE         0x08
#define EIE_WOLIE        0x04
#define EIE_TXERIE       0x02
#define EIE_RXERIE       0x01
// ENC28J60 EIR Register Bit Definitions
#define EIR_PKTIF        0x40
#define EIR_DMAIF        0x20
#define EIR_LINKIF       0x10
#define EIR_TXIF         0x08
#define EIR_WOLIF        0x04
#define EIR_TXERIF       0x02
#define EIR_RXERIF       0x01
// ENC28J60 ESTAT Register Bit Definitions
#define ESTAT_INT        0x80
#define ESTAT_LATECOL    0x10
#define ESTAT_RXBUSY     0x04
#define ESTAT_TXABRT     0x02
#define ESTAT_CLKRDY     0x01
// ENC28J60 ECON2 Register Bit Definitions
#define ECON2_AUTOINC    0x80
#define ECON2_PKTDEC     0x40
#define ECON2_PWRSV      0x20
#define ECON2_VRPS       0x08
// ENC28J60 ECON1 Register Bit Definitions
#define ECON1_TXRST      0x80
#define ECON1_RXRST      0x40
#define ECON1_DMAST      0x20
#define ECON1_CSUMEN     0x10
#define ECON1_TXRTS      0x08
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01
// ENC28J60 MACON1 Register Bit Definitions
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01
// ENC28J60 MACON2 Register Bit Definitions
#define MACON2_MARST     0x80
#define MACON2_RNDRST    0x40
#define MACON2_MARXRST   0x08
#define MACON2_RFUNRST   0x04
#define MACON2_MATXRST   0x02
#define MACON2_TFUNRST   0x01
// ENC28J60 MACON3 Register Bit Definitions
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMLEN   0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01
// ENC28J60 MICMD Register Bit Definitions
#define MICMD_MIISCAN    0x02
#define MICMD_MIIRD      0x01
// ENC28J60 MISTAT Register Bit Definitions
#define MISTAT_NVALID    0x04
#define MISTAT_SCAN      0x02
#define MISTAT_BUSY      0x01

// ENC28J60 EBSTCON Register Bit Definitions
#define EBSTCON_PSV2     0x80
#define EBSTCON_PSV1     0x40
#define EBSTCON_PSV0     0x20
#define EBSTCON_PSEL     0x10
#define EBSTCON_TMSEL1   0x08
#define EBSTCON_TMSEL0   0x04
#define EBSTCON_TME      0x02
#define EBSTCON_BISTST    0x01

// PHY registers
#define PHCON1           0x00
#define PHSTAT1          0x01
#define PHHID1           0x02
#define PHHID2           0x03
#define PHCON2           0x10
#define PHSTAT2          0x11
#define PHIE             0x12
#define PHIR             0x13
#define PHLCON           0x14

// ENC28J60 PHY PHCON1 Register Bit Definitions
#define PHCON1_PRST      0x8000
#define PHCON1_PLOOPBK   0x4000
#define PHCON1_PPWRSV    0x0800
#define PHCON1_PDPXMD    0x0100
// ENC28J60 PHY PHSTAT1 Register Bit Definitions
#define PHSTAT1_PFDPX    0x1000
#define PHSTAT1_PHDPX    0x0800
#define PHSTAT1_LLSTAT   0x0004
#define PHSTAT1_JBSTAT   0x0002
// ENC28J60 PHY PHCON2 Register Bit Definitions
#define PHCON2_FRCLINK   0x4000
#define PHCON2_TXDIS     0x2000
#define PHCON2_JABBER    0x0400
#define PHCON2_HDLDIS    0x0100

// ENC28J60 Packet Control Byte Bit Definitions
#define PKTCTRL_PHUGEEN  0x08
#define PKTCTRL_PPADEN   0x04
#define PKTCTRL_PCRCEN   0x02
#define PKTCTRL_POVERRIDE 0x01

// SPI operation codes
#define ENC28J60_READ_CTRL_REG       0x00
#define ENC28J60_READ_BUF_MEM        0x3A
#define ENC28J60_WRITE_CTRL_REG      0x40
#define ENC28J60_WRITE_BUF_MEM       0x7A
#define ENC28J60_BIT_FIELD_SET       0x80
#define ENC28J60_BIT_FIELD_CLR       0xA0
#define ENC28J60_SOFT_RESET          0xFF

// The RXSTART_INIT must be zero. See Rev. B4 Silicon Errata point 5.
// Buffer boundaries applied to internal 8K ram
// the entire available packet buffer space is allocated

#define RXSTART_INIT        0x0000  // start of RX buffer, room for 2 packets
#define RXSTOP_INIT         0x0BFF  // end of RX buffer
                            
#define TXSTART_INIT        0x0C00  // start of TX buffer, room for 1 packet
#define TXSTOP_INIT         0x11FF  // end of TX buffer
                            
#define SCRATCH_START       0x1200  // start of scratch area
#define SCRATCH_LIMIT       0x2000  // past end of area, i.e. 3.5 Kb 
#define SCRATCH_PAGE_SHIFT  6       // addressing is in pages of 64 bytes
#define SCRATCH_PAGE_SIZE   (1 << SCRATCH_PAGE_SHIFT)

// max frame length which the conroller will accept:
// (note: maximum ethernet frame length would be 1518)
#define MAX_FRAMELEN      1500        

#define FULL_SPEED  1   // switch to full-speed SPI for bulk transfers



static void enableChip () {
    cli();
    digitalWrite(ENC28J60_PIN, LOW);
}

static void disableChip () {
    digitalWrite(ENC28J60_PIN, HIGH);
    sei();
}

static void xferSPI (byte data) {
    SPDR = data;
    while (!(SPSR&(1<<SPIF)))
        ;
}

static byte readOp (byte op, byte address) {
    enableChip();
    xferSPI(op | (address & ADDR_MASK));
    xferSPI(0x00);
    if (address & 0x80)
        xferSPI(0x00);
    byte result = SPDR;
    disableChip();
    return result;
}

static void writeOp (byte op, byte address, byte data) {
    enableChip();
    xferSPI(op | (address & ADDR_MASK));
    xferSPI(data);
    disableChip();
}

static void readBuf(word len, byte* data) {
    enableChip();
    xferSPI(ENC28J60_READ_BUF_MEM);
    while (len--) {
        xferSPI(0x00);
        *data++ = SPDR;
    }
    disableChip();
}

static byte readByte() {
	byte b;
	readBuf(sizeof(b),&b);
	return b;
}

static word readWordLE() {
	word w;
	readBuf(1,(byte*)&w+1);
	readBuf(1,(byte*)(&w));
	return w;
}
static word readWordBE() {
	word w;
	readBuf(sizeof(w),(byte*)&w);
	return w;
}

static void drop(word len) {

    enableChip();
    xferSPI(ENC28J60_READ_BUF_MEM);
    while (len--) {
        xferSPI(0x00);
        SPDR;
    }
    disableChip();
}


static void writeBuf(word len, const byte* data) {
    enableChip();
    xferSPI(ENC28J60_WRITE_BUF_MEM);
    while (len--)
        xferSPI(*data++);
    disableChip();
}

static void SetBank (byte address) {
    if ((address & BANK_MASK) != Enc28j60Bank) {
        writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1|ECON1_BSEL0);
        Enc28j60Bank = address & BANK_MASK;
        writeOp(ENC28J60_BIT_FIELD_SET, ECON1, Enc28j60Bank>>5);
    }
}

static byte readRegByte (byte address) {
    SetBank(address);
    return readOp(ENC28J60_READ_CTRL_REG, address);
}

static word readReg(byte address) {
	return readRegByte(address) + (readRegByte(address+1) << 8);
}

static void writeRegByte (byte address, byte data) {
    SetBank(address);
    writeOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

static void writeReg(byte address, word data) {
    writeRegByte(address, data);
    writeRegByte(address + 1, data >> 8);
}

static word readPhyByte (byte address) {
    writeRegByte(MIREGADR, address);
    writeRegByte(MICMD, MICMD_MIIRD);
    while (readRegByte(MISTAT) & MISTAT_BUSY)
        ;
    writeRegByte(MICMD, 0x00);
    return readRegByte(MIRD+1);
}

static void writePhy (byte address, word data) {
    writeRegByte(MIREGADR, address);
    writeReg(MIWR, data);
    while (readRegByte(MISTAT) & MISTAT_BUSY)
        ;
}
 byte xPL_ENC28J60::initialize () {
    if (bitRead(SPCR, SPE) == 0) initSPI();

    pinMode(ENC28J60_PIN, OUTPUT);
    disableChip();
    
    writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
    delay(2); // errata B7/2
    while (!readOp(ENC28J60_READ_CTRL_REG, ESTAT) & ESTAT_CLKRDY)
        ;
        
    gNextPacketPtr = RXSTART_INIT;
    writeReg(ERXST, RXSTART_INIT);
    writeReg(ERXRDPT, RXSTART_INIT);
    writeReg(ERXND, RXSTOP_INIT);
    writeReg(ETXST, TXSTART_INIT);
    writeReg(ETXND, TXSTOP_INIT);
    //enableBroadcast(); // change to add ERXFCON_BCEN recommended by epam

	filterXplCmnd();


//    writeReg(EPMM0, 0x303f);
//    writeReg(EPMCS, 0xf7f9);
    writeRegByte(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
    writeRegByte(MACON2, 0x00);
    writeOp(ENC28J60_BIT_FIELD_SET, MACON3,
                        MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
    writeReg(MAIPG, 0x0C12);
    writeRegByte(MABBIPG, 0x12);
    writeReg(MAMXFL, MAX_FRAMELEN); 

	xPL_Eeprom epr(0);

    writeRegByte(MAADR5, epr.read());
    writeRegByte(MAADR4, epr.read());
    writeRegByte(MAADR3, epr.read());
    writeRegByte(MAADR2, epr.read());
    writeRegByte(MAADR1, epr.read());
    writeRegByte(MAADR0, epr.read());
    writePhy(PHCON2, PHCON2_HDLDIS);
    SetBank(ECON1);
    writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
    writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

    byte rev = readRegByte(EREVID);
    // microchip forgot to step the number on the silcon when they
    // released the revision B7. 6 is now rev B7. We still have
    // to see what they do when they release B8. At the moment
    // there is no B8 out yet
    if (rev > 5) ++rev;
    return rev;
}

 void xPL_ENC28J60::initSPI () {
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);   
    pinMode(MISO, INPUT);
    
    digitalWrite(MOSI, HIGH);
    digitalWrite(MOSI, LOW);
    digitalWrite(SCK, LOW);

    SPCR = bit(SPE) | bit(MSTR); // 8 MHz @ 16
    bitSet(SPSR, SPI2X);
}

void xPL_ENC28J60::filterXpl () {
	writeRegByte(ERXFCON, ERXFCON_ANDOR|ERXFCON_CRCEN|ERXFCON_PMEN);
//	writeReg(EPMO, 0x0000);
	writeReg(EPMM0, 0x3000); // 0C = 08:00 = IP
	writeReg(EPMM2, 0x0080); // 17 = 11    = UDP
	writeReg(EPMM4, 0x3C30); // 24 = 0F:19       = Port xPL 
							 // 2A = 78:70:6C:2D = "xpl-"
	writeReg(EPMM6, 0x001C); // 32 = 0A:7B:0A    = "/n{/n"*/

    writeReg(EPMCS, 0xB4F7);
}

void xPL_ENC28J60::filterXplCmnd () {
	writeRegByte(ERXFCON, ERXFCON_ANDOR|ERXFCON_CRCEN|ERXFCON_PMEN);
//	writeReg(EPMO, 0x0000);
	writeReg(EPMM0, 0x3000); // 0C = 08:00 = IP
	writeReg(EPMM2, 0x0080); // 17 = 11    = UDP
	writeReg(EPMM4, 0xFC30); // 24 = 0F:19       = Port xPL 
							 // 2A = 78:70:6C:2D:63:6D = "xpl-cm"
	writeReg(EPMM6, 0x001F); // 32 = 6E:64:0A:7B:0A    = "nd/n{/n"*/

    writeReg(EPMCS, 0xE325);
}


bool xPL_ENC28J60::begin()
{
	DBG(F("<begin ENC28J60>"),);

	xPL_Eeprom epr(0);
	_mac.toEeprom(epr);

	//   Stash::initMap(56);
	initialize();

//	enableBroadcast();

	return true;
}

bool xPL_ENC28J60::connection()
{
	return true;
}


class xPL_EncParser {
	int _addr;
public:
	xPL_EncParser(int addr):_addr(addr) {}
};


size_t printWord(Print& p, word w) { 
	size_t len=0;

	len += p.print((char)(w>>8));
	len += p.print((char)w);

	return len;
}

size_t xPL_ENC28J60::printETH(Print& p)
{
	size_t len = 0;

	for (byte i=0;i<6;i++) len += p.print('\xFF');       // broadcast mac
	len +=_mac.printBinTo(p);
	len += printWord(p,0x800);

	return len;
}

size_t xPL_ENC28J60::printIP(Print& p, word totallen, word chksum)
{

	size_t len=0;

	len += printWord(p,0x4500);
	len += printWord(p,totallen);
	len += printWord(p,0);
	len += printWord(p,0x4000);
	len += printWord(p,0x4011);

	len += printWord(p,chksum);

#ifdef XPL_IP
	for (byte i=0;i<4;i++) len += p.print(_ip.bin[i]);
#else
	for (byte i=0;i<4;i++) len += p.print('\0');
#endif
	for (byte i=0;i<4;i++) len += p.print('\xFF');       // Ip source and dest set to all ones.

	return len;
}


size_t xPL_ENC28J60::printUDP(Print& p, xPL_Message& data, word len_udp, word chksum)
{

	size_t len =0;

	len += printWord(p, XPL_PORT);
	len += printWord(p, XPL_PORT);

	len += printWord(p,len_udp);

	len += printWord(p,chksum);

	len += data.printTo(p);

	return len;
}

class checksum : public Print {
	bool _high;
	uint32_t _sum;
	uint32_t _len;
public:
	checksum() { reset(); }

	void reset() {
		_sum = 0;
		_len = 0;
		_high = true;
	}

	size_t write(uint8_t val)
	{
		word w = val;
		if (_high) w<<=8;

		_sum += w;
		_high=!_high;
		_len ++;
		return 1;
	}

	word sum_udp()
	{
		uint32_t s = _sum + 0x11 + _len -8;
		while (s>>16)
			s = (word) s + (s >> 16);
	
		return ~(word)s;
	}

	word sum()
	{
		uint32_t s = _sum;
		while (s>>16)
			s = ((word) s) + (s >> 16);
	
		return ~(word)s;
	}
	word len() { return _len; }
};

class encFiller: public Print {
protected:
	size_t write(uint8_t val) { xferSPI(val); return 1; }

public:
	void start(size_t len) {
		while (readOp(ENC28J60_READ_CTRL_REG, ECON1) & ECON1_TXRTS)
        if (readRegByte(EIR) & EIR_TXERIF) {
            writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
            writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        }
		writeReg(EWRPT, TXSTART_INIT);
		writeReg(ETXND, TXSTART_INIT+ len);
		writeOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

		enableChip();
		xferSPI(ENC28J60_WRITE_BUF_MEM);

	}

	void send() {
		disableChip();

		writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
	}
};

class debugFiller: public Print {
protected:
	size_t _len;
	size_t write(uint8_t val) {
		size_t len=0;
		len += Serial.print(val,HEX);len +=Serial.print(':');
		_len++;
		if (!(_len%8)) len += Serial.println();
		return len;
	}

public:
	void start(size_t len) {
		DBG(F("start"),len);
		_len=0;
	}

	void send() {
		DBG(F("send"),_len);
	}
};

void xPL_ENC28J60::loop()
{
	if (!connection()) { return; }

	size_t addr = gNextPacketPtr; 

	word receiveLen = packetReceive();
	if (!receiveLen) { return; }
	DBG(F("<ENC28J60 ???>"),receiveLen);

	drop(23); 

	if ( readByte() == 0x11 ) // it is an UDP packet
	{
		drop(12);addr+=13;
		
		word udp_port=readWordLE();

		if (udp_port == XPL_PORT)
		{
			DBG(F("<ENC28J60 xPL>"),receiveLen);

			VString s(addr + 35,receiveLen-42,VSHelperENC28J60::helper);
			xPL_MessageIn msg(s);
			xPL.parseMessage(msg);
		}
	}
	packetRelease();
}


bool xPL_ENC28J60::sendMessage(xPL_Message& msg)
{
		size_t datalen = msg.len();

		DBG(F("<send_ENC28J60> "),datalen);

		if (connection()) 
		{
			checksum chk_ip;
			checksum chk_udp;
			encFiller filler;

			printIP(chk_ip,20 + 8 + datalen);
		//DBG(F("chk_ip "),chk_ip.len());

#ifdef XPL_IP
	for (byte i=0;i<4;i++) chk_udp.print(_ip.bin[i]);
#else
	for (byte i=0;i<4;i++) chk_udp.print('\0');
#endif
			for (byte i=0; i<4; i++) chk_udp.print('\xFF');
			printUDP(chk_udp, msg, 8 + datalen);

		//DBG(F("chk_udp "),chk_udp.len());

			filler.start(14 + 20 + 8 + datalen);

			printETH(filler);
			printIP(filler,20 + 8 + datalen,chk_ip.sum());
			printUDP(filler, msg, 8 + datalen ,chk_udp.sum_udp());
			filler.send();

			return true;
		}
		return false;
}


word xPL_ENC28J60::packetReceive() {
    word len = 0;
    if (readRegByte(EPKTCNT) > 0) {
        writeReg(ERDPT, gNextPacketPtr);

        struct {
            word nextPacket;
            word byteCount;
            word status;
        } header;
        
        readBuf(sizeof header, (byte*) &header);

        gNextPacketPtr  = header.nextPacket;
        len = header.byteCount - 4; //remove the CRC count

		if ((header.status & 0x80)==0)
		{
            len = 0;
			packetRelease();
		}
    }
    return len;
}

void xPL_ENC28J60::packetRelease() {
        if (gNextPacketPtr - 1 > RXSTOP_INIT)
            writeReg(ERXRDPT, RXSTOP_INIT);
        else
            writeReg(ERXRDPT, gNextPacketPtr - 1);
        writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
}


char VSHelperENC28J60::charAt(size_t pos, const VString& s) const {
	writeReg(ERDPT, s.addr()+pos);
	return readByte();
}
