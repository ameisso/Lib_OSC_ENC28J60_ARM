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
#ifndef HWFUNK_H
#define HWFUNK_H

#include "config.h"
#include "lpc13xx.h"
#include "lpc13xx_systick.h"
#include "lpc13xx_iocon.h"
#include "lpc13xx_syscon.h"
#include "lpc13xx_gpio.h"
#include "lpc13xx_ssp.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Chip Select Port & Pin
#define ETH_CS_PORT		PORT1
#define ETH_CS_PIN		GPIO_Pin_10

//SCK pin location
//!\ if 0_10, SWD is disabled
#define SCK_LOC SCK_PIO0_6

void hwFunctionsInit();
void cli();                // disable global interrupts
void sei();                // enable interrupts
void enableChip();
void disableChip();
void initSPI();
void xferSPI(uint8_t data);
uint8_t rcvSPI();
unsigned int millis();//function that return the time elapsed since the begining
void delay(int time_millis);

char eeprom_read_byte(unsigned char* address_short);

#ifdef __cplusplus
}
#endif

#endif
