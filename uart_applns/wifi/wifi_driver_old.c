/***************************************************************************
 * Project           			: shakti devt board
 * Name of the file	     		:
 * Created date			        : 
 * Brief Description of file            : 
 * Name of Author    	                :  
 * Email ID                             :  

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
@file 
@brief 
@detail 
*/

# include <stdio.h>
# include <string.h>
# include "defines1.h"

// program for wifi module using uart protocol 

# define timeout 2000

// purpose to use esp8266 as a client
// to render a html page which counts from 1 to 100 
/*
macros to be used are : 

UartTransmitString(char *s);

UartReceiveCharacter(void);
*/

/** @fn char * send_AT(char *x , unsigned int y)
 * @brief 
 * @details 
 * @warning 
 * @param 
 * @param 
 * @return 
 */
char * send_AT(char *x , unsigned int y)
{
	UartTransmitString(x);
	char buffer2[500];

	for(int i=0;i<200;i++)
	{
	char a = receive_char(y);
	buffer2[i] = a; 
	}
	return buffer2;
}   

#define baud 115200

int connectionId = 49;

char receive_buffer[200];
char send_buffer[200];
char response[500];

// Send html files

/** @fn void send_html(const char * data)
 * @brief 
 * @details 
 * @warning 
 * @param 
 * @return 
 */
void send_html(const char * data)
{
	
	char connect[20];
	char length[20];

	sprintf(length,"%d",strlen(data));

	sprintf(connect,"%d",0);
		
	char cipSend[250] = " AT+CIPSEND=";

	strcat(cipSend,connect);
	
	strcat(cipSend,",");
	
        strcat(cipSend,length);
	
        strcat(cipSend,"\r\n");

	printf("%s\n",send_AT(cipSend,timeout));
	
        printf("%s\n",send_AT(data,timeout));
	
}

/** @fn void init_client()
 * @brief 
 * @details 
 * @warning  
 */
// initialization to connect to a AP
void init_client()
{

	strcpy(send_buffer,"AT\r\n"); // debug input
	strcpy(response,send_AT(send_buffer,timeout));

	if (strcmp(response,"OK") == 0) printf("Welcome to Esp8266-module\n"); 

	strcpy(send_buffer,"AT+RST\r\n"); //reset module
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

	strcpy(send_buffer,"AT+CWMODE=3\r\n"); // configure as client / AP
	strcpy(response,send_AT(send_buffer,timeout)); //
	printf("%s\n",response);

	// using ssid and password to connect to AP
	strcpy(send_buffer,"AT+CWJAP=\"shakti\",\"shakti123\"\r\n");
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

	strcpy(send_buffer,"AT+CIFSR\r\n"); // get ip address
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

	strcpy(send_buffer, "AT+CIPMUX=1\r\n"); // configure for multiple connections
	strcpy(response,send_AT(send_buffer,timeout));                                      
	printf("%s\n",response);


	strcpy(send_buffer,"AT+CIPSERVER=1,80\r\n"); // turn on server on port 80
		strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

	strcpy(send_buffer,"AT+CIPSEND");  // initiate UART comm with ESP866 
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);
}
// end of server initialization

/** @fn void main()
 * @brief start the server at port 80
 * @details 
 * @warning 
 */
void main()
{
	char buffer1[100];
    	
	Uart1Init(baud); // to initialize the baudrate of Uart1

	init_client();
	
        for (int i = 0; i < 1; i++)
	{
		//send the info to module
		sprintf(buffer1,"<html><h1>counter is at %d</h1></html>",i);
		
                send_html(buffer1);

		delayLoop(100,100);
	}

	// Safely disconnect

	//AT+CIPMODE=0 --> disable uart communication 
	strcpy(send_buffer,"AT+CIPMODE=0");  // initiate UART comm with ESP866 
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

	//AT+CIPCLOSE --> close the tcp/ip connection          
	strcpy(send_buffer,"AT+CIPCLOSE"); 
	strcpy(response,send_AT(send_buffer,timeout));
	printf("%s\n",response);

}
