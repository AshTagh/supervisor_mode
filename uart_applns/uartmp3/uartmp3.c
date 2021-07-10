/***************************************************************************
* Project           			: shakti devt board
* Name of the file	     		: uartmp3.c
* Brief Description of file             : plays a song with the help of uart protocol .
* Name of Author    	                : Kotteeswaran
* Email ID                              : kottee.1@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
/**
@file uartmp3.c  
@brief plays a song with the help of uart protocol . 
@detail Configures UART 1 & MP3 decoder and then plays first song 
*/

#include <stdint.h>//
#include <string.h>//includes the definitions of string functions//
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>//includes the definitions of standard input/output functions//
#include <sys/signal.h>
#include "uart1mp3.h"//incldes the definitions of uart communication protocol//

#define DELAY1 1000//Determines the duration of delay1//
#define DELAY2 2000//Determines the duration of delay2//
#define REPEAT 1

/** @fn void sendCommand(unsigned char command, unsigned char dat1, unsigned char dat2)
 * @brief Sends command to  mp3 decoder.
 * @details Sends the command to mp3 decoder in a specified format.
 * @param command ---> command that is to be written
 * @param dat1 - arguments required for the command
 * @param dat2 - arguments required for the command      
 */
void sendCommand(unsigned char command, unsigned char dat1, unsigned char dat2)
{
	//  delay(20);
	Send_buf[0] = 0x7E;    //
	Send_buf[1] = 0xFF;    //
	Send_buf[2] = 0x06;    // Len
	Send_buf[3] = command; //
	Send_buf[4] = 0x01;    // 0x00 NO, 0x01 feedback
	Send_buf[5] = dat1;    // datah
	Send_buf[6] = dat2;    // datal
	Send_buf[7] = 0xEF;    //
	printf("Sending: ");

	uart_write(UART_DATA_REGISTER, UART1, Send_buf[0]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[1]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[2]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[3]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[4]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[5]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[6]) ;
	uart_write(UART_DATA_REGISTER, UART1, Send_buf[7]) ;
}

/** @fn void Delay(uint16_t Cnt1, uint16_t Cnt2)
 * @brief Maintains the required delay to perform an operation. 
 * @details provides Cnt1 * Cnt2 amount of cycles delay (approximate value only) in order 
 *          perform certain operations.
 * @param cnt1
 * @param cnt2 (Multiplication of these two approximates the required delay)
 */
void Delay(uint16_t Cnt1, uint16_t Cnt2)
{
	uint16_t i = 0, j = 0;
	for( i = 0; i < Cnt1; i++)
		for( j = 0; j < Cnt2; j++);
}

/** @fn void setVolume(int volume)
 * @brief   Does the volume adjustments as required . 
 * @details The volume can be increased from level 0 to level 30 by sending command over UART.
 * @param volume
 */
void setVolume(int volume)
{
	sendCommand(0x06, 0, volume); // Set the volume (0x00~0x30)
	Delay(DELAY1,DELAY2);
}

/** @fn void playFirst()
 * @brief plays the first song in the SD card. 
 * @details Do Mp3 Decoder init, set volume and plays the first song in the SD card.
 */
void playFirst()
{
	printf("song playing command sent\n");


	sendCommand(0x3F, 0, 0);
	Delay(DELAY1,DELAY2);
	setVolume(10);
	Delay(DELAY1,DELAY2);

	sendCommand(0x11,0,0x1);
	Delay(DELAY1,DELAY2);
	printf("song playing command sent complete\n");
}

/** @fn void main()
 * @brief Plays first song.. 
 * @details This Configures mp3 decoder and play the first song in the SD Card. 
 */
void main()
{
	#if 0
	uart_0_init();
	uart_1_init(9600);
	#endif
	playFirst();

	while(1);
}
