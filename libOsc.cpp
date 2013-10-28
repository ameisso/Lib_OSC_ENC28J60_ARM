/**************************************************************************/
/*! 
    @file     libOsc.cpp
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

#include "libOsc.h"

uint8_t Ethernet::buffer[500]; // tcp/ip send and receive buffer

//Array of the registered reception callbacks
OSC_Callback registeredCallbacks[MAX_OSC_CALLBACKS];
int nbOscCallbacks = 0;



//************************************************************************
//Private functions
//************************************************************************

//callback that sends received UDP packets to the OSC server
void udpToOscServer(uint16_t port, uint8_t ip[4], const char *data, int len)
{
	//The received packet may be an OSC bundle or message
	if(*data == '#') //Bundle
	{
		OSCBundle bundleIN;
		if(len > 0)
		{
			for(int i = 0; i < len; i++)
				bundleIN.fill(data[i]);

			if(!bundleIN.hasError())
			{
				//Route the incoming messages to the registered OSC callbacks
				for(int i = 0; i < nbOscCallbacks; i++)
					bundleIN.route((registeredCallbacks[i]).address, (registeredCallbacks[i]).callback);
			}
		}
	}
	else //message or error
	{
		OSCMessage msgIN;
		if(len > 0)
		{
			for(int i = 0; i < len; i++)
				msgIN.fill(data[i]);

			if(!msgIN.hasError())
			{
				//Route the incoming messages to the registered OSC callbacks
				for(int i = 0; i < nbOscCallbacks; i++)
					msgIN.route((registeredCallbacks[i]).address, (registeredCallbacks[i]).callback);
			}
		}
	}
}

//************************************************************************
//Public functions
//************************************************************************

/* Set up the OSC connection using the parameters set in config.h :
 * IP, MAC, ports
 */
void OSC_Setup()
{
	//Initialize ethernet controller
	ether.begin(sizeof(Ethernet::buffer), mymac);

	//Setup IP
#if STATIC_IP
	ether.staticSetup(myip, gwip);
#else
	ether.dhcpSetup();
#endif

	//register udpToOscServer() to incoming port
	ether.udpServerListenOnPort(&udpToOscServer, inPort);
}


/* Add a RX callback to the list
 *
 * The following information must be provided :
 *  addr (string) : the address to route the messages to
 *  cb (void*) : a callback function to  treat the messages with parameters(OSCMessage &msg, int addrOffset)
 */
void OSC_AddCallback(char* addr, void(*cb)(OSCMessage&, int))
{
	if(nbOscCallbacks < MAX_OSC_CALLBACKS)
	{
		(registeredCallbacks[nbOscCallbacks]).address = addr;
		(registeredCallbacks[nbOscCallbacks]).callback = cb;

		nbOscCallbacks++;
	}
}


/* Proceeds the incoming packets
 * Must be called as regularly as possible (i.e. in the main loop)
 */
void OSC_Update()
{
	uint16_t nbRecu = ether.packetReceive();
	ether.packetLoop(nbRecu);
}

/* Send an OSC Message to the specified IP and port
 */
void OSC_SendMessage(uint16_t destPort, uint8_t destIp[4], OSCMessage &msg)
{
	ether.udpPrepare(outPort, destIp, destPort);

	BufferFiller bufferFill = BufferFiller(ether.buffer+UDP_DATA_P); //Extends Print -> necessary for OSCMessage
	//UDP_DATA_P is the length of the data written by udpPrepare (UDP packet header)

	msg.send(bufferFill);

	ether.udpTransmit(msg.bytes());
}

