/**************************************************************************/
/*! 
    @file     hwFunk.h
    @author   Tom magnier / Antoine Meissonnier
		magnier.tom@gmail.com / ameisso@gmail.com
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2011, Tom magnier / Antoine Meissonnier

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include <stdint.h>
#include "libosc/ethercard/net.h"
#ifndef HWFUNK_H
#define HWFUNK_H
//TODO define the terms and the default values in comment.s
#define SS 0x42
#define MOSI 0x42
#define MISO 0x42
#define SCK 0x42
#define SPCR 0x42
#define SPE 0x42
#define MSTR 0x42
#define SPSR 0x42
#define SPI2X 0x42
#define SPDR;
//#define SPSR 0 uncomment to redefine
#define SPIF 0x42

//bit manipulation (from arduino.h)
#define bit(b) (1UL << (b))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


unsigned int millis();//function that return the time elapsed since the begining
void pinMode(int pinNbr, int direction);
void digitalWrite(int pinNbr, int value);
void cli();                // disable global interrupts
void sei();                // enable interrupts
void delay(int time_millis);//delay before exectuting next instructions
char pgm_read_byte(const char*address_short) ;//maybe it's not int...
char eeprom_read_byte(unsigned char* address_short);

char* itoa 	( 	int  	__val,		char *  	__s,		int  	__radix	);//see avr doc 	
char* ltoa 	( long int  	__val,		char *  	__s,		int  	__radix	);//see avr doc 	

//ENC28J60 FUNCS
class ENC28J60 {
public:


  static uint8_t buffer[];
  static uint16_t bufferSize;
  
  static uint8_t* tcpOffset () { return buffer + 0x36; }

  static void initSPI ();//replaced by : void SSP_Init(SSP_TypeDef *SSPx, SSP_CFG_Type *SSP_ConfigStruct)
  static uint8_t initialize (const uint16_t size, const uint8_t* macaddr,
                             uint8_t csPin =8);
  static bool isLinkUp ();
  
  static void packetSend (uint16_t len);
  static uint16_t packetReceive ();
  
  static void copyout (uint8_t page, const uint8_t* data);
  static void copyin (uint8_t page, uint8_t* data);
  static uint8_t peekin (uint8_t page, uint8_t off);

  static void powerDown();  // contrib by Alex M.
  static void powerUp();    // contrib by Alex M.
  
  static void enableBroadcast();
  static void disableBroadcast();
  static void disableMulticast ();
  static uint8_t doBIST(uint8_t csPin =8);
	};

typedef ENC28J60 Ethernet;
#endif