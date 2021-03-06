/**************************************************************************/
/*! 
    @file     libOsc.h
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

#ifndef LIBOSC_h
#define LIBOSC_h

#include "lpc13xx.h"
#include "EtherCard.h"
#include "config.h"
#include "OSCBundle.h"
#include "OSCMessage.h"

//Structure for the OSC reception callbacks
typedef struct {
	char* address; //OSC address, ex : "/motor/"
	void (*callback)(OSCMessage&, int); //OSC function to route to ; must take as arguments (OscMessage &msg, int addrOffset)
		//with addrOffset the offset where the address match was found
} OSC_Callback;


void OSC_Setup(); //uses the parameters set in config.h
void OSC_AddCallback(char* addr, void(*cb)(OSCMessage&, int)); //RX callbacks
void OSC_SendMessage(uint16_t destPort, uint8_t destIp[4], OSCMessage &msg);
void OSC_Update(); //must be called as regularly as possible

#endif //LIBOSC_h
