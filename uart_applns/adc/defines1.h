/***************************************************************************
* Project           		: shakti devt board
* Name of the file	     	: defines1.h
* Brief Description of file     :
* Name of Author    	        : 
* Email ID                      : 

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
@file defines1.h 
@brief  
@detail 
*/

//#define UART Base address definition

#define UART0_BASE_ADDRESS 0x11300
#define UART1_BASE_ADDRESS 0x11400

#if 0
// UART TX (W) and RX (R) buffers

#define UART_DATA_REGISTER 0
// UART interrupt enable (RW)

#define UART_INT_ENABLE_REGISTER 1
// UART interrupt identification (R)

#define UART_INT_IDENTI_REGISTER 2
// UART FIFO control (W)

#define UART_FIFO_CTRL_REGISTER 2
// UART Line Control Register (RW)

#define UART_LINE_CTRL_REGISTER 3
// UART Modem Control (W)

#define UART_MODEM_CTRL_REGISTER 4
// UART Line Status (R)

#define UART_LINE_STATUS_REGISTER 5
// UART Modem Status (R)

#define UART_MODEM_STATUS_REGISTER 6
// UART base address of peripheral in NIOS memory map

#define UART_SCRATCH_REGISTER 7

// UART Divisor Registers offsets
#define UART_DIVISOR_LSB_LATCH_REGISTER 0
#define UART_DIVISOR_MSB_LATCH_REGISTER 1
#else
#define UART_BAUD_RATE_SEL_REG 0x00
#define UART_TX_REGISTER 0x04
#define UART_RX_REGISTER 0x08
#define UART_STS_REGISTER 0x0C


#define UART_DATA_REGISTER 0
#define UART_LINE_CTRL_REGISTER 3
#define UART_LINE_STATUS_REGISTER 5
#endif

#define INPUT_CLOCK_FREQ 50000000

#define UART0 0
#define UART1 1


void WriteIntoUart(unsigned char offSet, unsigned char uartSel, unsigned short int writeData)
{
	short int * writeAddress = NULL;
	if(1 == uartSel )
		writeAddress = (UART1_BASE_ADDRESS + offSet);
	else
		writeAddress = (UART0_BASE_ADDRESS + offSet);

		*writeAddress  = writeData;

}

short int ReadFromUart(unsigned char offSet, unsigned char uartSel)
{
	short int* readAddress = NULL;
	if(1 == uartSel )
		readAddress = (UART1_BASE_ADDRESS + offSet);
	else
		readAddress = (UART0_BASE_ADDRESS + offSet);
	return (*readAddress) ;
}



void Uart1Init(int baud)
{
	int d = INPUT_CLOCK_FREQ / (16 * baud);
	printf("Set divisor to 0x%x\n",d);
#if 0
	WriteIntoUart(UART_LINE_CTRL_REGISTER, UART1, 0x83);
	ReadFromUart(UART_
	WriteIntoUart(UART_DIVISOR_MSB_LATCH_REGISTER, UART1, d>>8);
	WriteIntoUart(UART_DIVISOR_LSB_LATCH_REGISTER, UART1, d & 0xff);
	WriteIntoUart(UART_LINE_CTRL_REGISTER, UART1, 0X03);
	WriteIntoUart(UART_FIFO_CTRL_REGISTER, UART1, 0X06);
	WriteIntoUart(UART_INT_ENABLE_REGISTER, UART1, 0X00);

	printf("over initialization -----\n");
#else
	printf("\nWriting into UART 1 BAUD RATE REG\n");
	WriteIntoUart(UART_BAUD_RATE_SEL_REG, UART1, d);
	printf("\nRead from UART 1 BAUD RATE REG\n");
	printf("\nRead value is %hu",(short int) ReadFromUart(UART_BAUD_RATE_SEL_REG,UART1));


#endif
}

//#endif
unsigned int UartTxReady()
{
	unsigned int readData = 0;
	readData = ReadFromUart(UART_LINE_STATUS_REGISTER, 1);
	return readData;
}

void UartTransmitCharacter(char c)
{
printf("uart_trans started!\n");
	unsigned int status, tx_empty;


	while(1)
	{
		status = UartTxReady();
		if( 1 == ( (status >> 5) & 0x1) )
			break;
	}
	// tx must be empty now so send
	WriteIntoUart(UART_DATA_REGISTER, UART1, (char) c);
printf("trans-ready!\n");
}


void UartTransmitString(char *s)
{
	printf("hello\n");
	for(; *s != NULL; s++)
		UartTransmitCharacter(*s);
}


int UartCheckReceiveReady(void)
{
	unsigned int readData = 0;
	readData = ReadFromUart(UART_LINE_STATUS_REGISTER, UART1);
	return (readData & 0x01);
}


char UartReceiveCharacter(void)
{
	while(!UartCheckReceiveReady()) ;
	return (unsigned char) ReadFromUart(UART_DATA_REGISTER, UART1);
}


void delayLoop(int l , int m)
{
for (int i = 0;i<l;i++) for (int j = 0;j<m;j++) continue;
}

char receive_char(int timeout1)
{

int count = 0;

while(!UartCheckReceiveReady() && count<timeout1) count++;
return (unsigned char) ReadFromUart(UART_DATA_REGISTER, UART1);
}

































