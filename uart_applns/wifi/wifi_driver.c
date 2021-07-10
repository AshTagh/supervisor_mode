/***************************************************************************
* Project           			: shakti devt board
* Name of the file	     		: wifi_driver.c
* Brief Description of file             : driver for esp8266 module
* Name of Author    	                : 
* Email ID                              : 

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
@file wifi_driver.c
@brief driver for esp8266 module
@detail 
*/

# include <stdio.h>
# include <string.h>
//# include "defines1.h"
//# include "defines_uart_update.h"
// program for wifi module using uart protocol 


# define timeout 100
// purpose to use esp8266 as a client
// to render a html page which counts from 1 to 100 
/*
macros to be used are : 
UartTransmitString(char *s);
UartReceiveCharacter(void);
*/

#define Tx_reg 0x11404
#define Rx_reg 0x11408
#define baud_reg 0x11400 

/** @fn void Uart1Init(int baud1)
 * @brief 
 * @details 
 * @warning 
 * @param  
 * @return
 */
void Uart1Init(int baud1)
{
   short int * baud; 
   baud = baud_reg;
   (*baud) = (short int) 27;
}

/** @fn void delayLoop(int l ,int m)
 * @brief 
 * @details 
 * @warning 
 * @param
 * @param
 * @return 
 */
void delayLoop(int l ,int m)
{
for (int i = 0; i < l; i++) for (int j = 0; j < m; j++) continue;
}

/** @fn char receive_char(int timeout1) 
 * @brief 
 * @details 
 * @warning 
 * @param
 * @return 
 */
char receive_char(int timeout1) 
{
char * val ;
val = Rx_reg;
return  *val;
}

/** @fn char * UartTransmitCharacter(char c)
 * @brief 
 * @details 
 * @warning 
 * @param 
 * @return 
 */
char * UartTransmitCharacter(char c)
{
        char * val1;
	val1 = Tx_reg;
	delayLoop(10,10);
        (*val1) = c;	
	delayLoop(10,10);
}

/** @fn char * UartTransmitString(char * s)
 * @brief 
 * @details 
 * @warning 
 * @param 
 * @return 
 */
char * UartTransmitString(char * s)
{
for (; *s!=NULL;s++) UartTransmitCharacter(*s);
}

/** @fn void send_AT(char *x , unsigned int y)
 * @brief 
 * @details 
 * @warning 
 * @param 
 * @param
 * @return 
 */
void send_AT(char *x , unsigned int y)
{
	printf("start send\n");
	UartTransmitString(x);
	char buffer2[500];

	for(int i=0;i<200;i++)
	{
	char a = receive_char(y);
	buffer2[i] = a; 
	delayLoop(8,8);
	}
	printf("Out -> %s\n",buffer2);
	//return buffer2;
}   

#define baud 115200

int connectionId = 49;

char receive_buffer[200];
char send_buffer[200];
char response[500];

/** @fn void send_html(const char * data)
 * @brief 
 * @details 
 * @warning 
 * @param
 * @return
 */
// Send html files
void send_html(const char * data)
{
	printf("1\n");
	char cipSend[200];	
	/*	
	char connect[40];
	char length[40];
	sprintf(length,"%d",strlen(data));
	sprintf(connect,"%d",0);
	char cipSend[250] = "AT+CIPSEND=";
	strcat(cipSend,connect);
	strcat(cipSend,",");
        strcat(cipSend,length);
	*/

	strcpy(cipSend,"AT+CIPSEND=0,45");
	
	printf("%s\n",cipSend);
        //strcat(cipSend,"\r\n");
	send_AT(cipSend,timeout);
	delayLoop(20,20);
	send_AT(data,timeout);	
	printf("send!\n");
	//printf("%s\n",send_AT(cipSend,timeout));	
        //printf("%s\n",send_AT(data,timeout));
}

/** @fn void init_client()
 * @brief 
 * @details 
 * @warning 
 */
// initialization to connect to a AP
void init_client()
{
	char response[500];
	strcpy(send_buffer,"AT"); // debug input
	//strcpy(response,send_AT(send_buffer,timeout));	
	send_AT(send_buffer,timeout);
	//if (strcmp(response,"OK")==0) printf("Welcome to Esp8266-module\n"); 
	//printf("%s --> 12\n",response);
	delayLoop(1000,1000);
	
	strcpy(send_buffer,"AT+RST"); //reset module
	
	//strcpy(response,send_AT(send_buffer,timeout));
        send_AT(send_buffer,timeout);
        //printf("%s\n",response);
		
	delayLoop(10000,3000);
	
	strcpy(send_buffer,"AT+CWMODE=3"); // configure as client / AP
	//strcpy(response,send_AT(send_buffer,timeout)); //

	send_AT(send_buffer,timeout); //
	//printf("%s\n",response);
	
	delayLoop(1500,1500);

	// using ssid and password to connect to AP
	strcpy(send_buffer,"AT+CIFSR"); // get ip address
		
	//strcpy(response,send_AT(send_buffer,timeout));
	
	send_AT(send_buffer,timeout);
	//printf("%s\n",response);
	
	delayLoop(1500,1500);
	
	strcpy(send_buffer, "AT+CIPMUX=1"); // configure for multiple connection
	//strcpy(response,send_AT(send_buffer,timeout));                                      printf("%s\n",response);		
	send_AT(send_buffer,timeout);
			
	delayLoop(1500,1500);
	
	strcpy(send_buffer,"AT+CIPSERVER=1,80"); // turn on server on port 80
	//strcpy(response,send_AT(send_buffer,timeout));
	//printf("%s\n",response);
	send_AT(send_buffer,timeout);

	delayLoop(1500,1500);

	printf(" ------------------------------------------------ \n");
}
// end of server initialization

// start the server at port 80

/** @fn void main()
 * @brief 
 * @details 
 * @warning 
 */
void main()
{
	char buffer1[200] = "<html><h1>Hi I am Shakti E class!</h1></html>";
    	
	Uart1Init(baud); // to initialize the baudrate of Uart1

	init_client();
	
        for ( int i = 0; i < 50; i++)
	{
		//send the info to module
		printf("%d\n",i);
		//sprintf(buffer1,"<html><h1>counter is at %d</h1></html>",i);
                send_html(buffer1);
		delayLoop(1500,1500);
	}
	// Safely disconnect
	//AT+CIPMODE=0 --> disable uart communication 
	delayLoop(500000,500000);
	//AT+CIPCLOSE --> close the tcp/ip connection          
	//printf("closing the wifi connection\n");

	//strcpy(send_buffer,"AT+CWQAP"); // turn on server on port 80
	//strcpy(response,send_AT(send_buffer,timeout)); 
	//send_AT(send_buffer,timeout);
	//printf("%s\n",response);
}
