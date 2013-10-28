/**************************************************************************/
/*! 
    @file     hwFunk.c
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
#include "hwfunk.h"

//*************************************************************************
//SPI Config
SSP_CFG_Type SPIcfg = {
	SSP_DATABIT_8, 		//.Databit
	SSP_CPHA_FIRST, 	//.CPHA
	SSP_CPOL_LO, 		//.CPOL
	SSP_MASTER_MODE, 	//.Mode
	SSP_FRAME_SPI, 		//.FrameFormat
	8000000}; 			//.ClockRate

void hwFunctionsInit()
{
	//Systick (1 tick every ms)
	systickInit();

	//SPI
    initSPI();

    //GPIO & CS pin
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_GPIO, ENABLE);
	GPIO_SetDir(ETH_CS_PORT, ETH_CS_PIN, 1); //OUTPUT
	GPIO_SetBits(ETH_CS_PORT, ETH_CS_PIN);
}



void cli()
{
	__disable_irq();
}

void sei()
{
	__enable_irq();
}


void enableChip () {
    cli();
    GPIO_ResetBits(ETH_CS_PORT, ETH_CS_PIN);
}

void disableChip () {
    GPIO_SetBits(ETH_CS_PORT, ETH_CS_PIN);
    sei();
}

void initSPI ()
{
	SSP_SSPPinsInit(SCK_LOC, DISABLE); //!\ SWCLK et SCLK partagent la même patte 0_10 ! => pas de SWD si SPI actif
	SSP_Init(LPC_SSP, &SPIcfg);
	SSP_Cmd(LPC_SSP, ENABLE);
}

void xferSPI(uint8_t data)
{

	//wait for the TX FIFO to be "not full" if necessary
	while(!(LPC_SSP->SR & SSP_SR_TNF)) ;

	//flush the RX FIFO
	while(LPC_SSP->SR & SSP_SR_RNE)
		SSP_ReceiveData(LPC_SSP);

	LPC_SSP->DR = data;

	//wait for the TX FIFO to be empty (data sent)
	while((LPC_SSP->SR & (SSP_SR_TFE|SSP_SR_BSY)) != SSP_SR_TFE) ;
}

//Returns the last received byte
uint8_t rcvSPI()
{
	uint8_t result = 0;

	//Wait for the SSP controller to be idle
	while(LPC_SSP->SR & SSP_SR_BSY) ;

	//Read the last received data (flush the RX FIFO -> discards the previously received data)
	while(LPC_SSP->SR & SSP_SR_RNE)
		result = (uint8_t)(SSP_ReceiveData(LPC_SSP));

	return result;
}


unsigned int millis()
{
	return systickGetMillisecondsActive();
}

void delay(int time_millis)
{
	systickDelayMs(time_millis);
}
