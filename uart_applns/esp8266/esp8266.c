/***************************************************************************
* Project           			: shakti devt board
* Name of the file	     		: esp8266.c
* Brief Description of file             : Sample program for using esp8266.
* Name of Author    	                :  Kotteeswaran E

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
@file esp8266.c
@brief Sample program for using esp8266.  
@detail 
*/

#include "uart.h"//Includes the definitions of uart communication protocol//
#include "string.h"

#ifndef ARTIX7_100T
/** 
 * @fn int write_to_esp8266(uart_struct * ptr_UART, char *data) 
 * @brief 
 * @details 
 * @warning
 * @param uart_struct * ptr_UART
 * @param char *data
 * @return
 */
int write_to_esp8266(uart_struct * ptr_UART, char *data) {
	while (*data != '\0') {
		write_uart_character(ptr_UART, *data);
		data++;
	}
	write_uart_character(ptr_UART,'\r');
	write_uart_character(ptr_UART,'\n');
}

/** 
 * @fn int write_enter_to_esp8266(uart_struct * ptr_UART)
 * @brief 
 * @details 
 * @warning
 * @param uart_struct * ptr_UART
 * @return
 */
int write_enter_to_esp8266(uart_struct * ptr_UART){
	write_uart_character(ptr_UART,'\r');
	write_uart_character(ptr_UART,'\n');
}

/** 
 * @fn int read_from_esp8266(uart_struct * ptr_UART, char *data) 
 * @brief 
 * @details 
 * @warning
 * @param uart_struct * ptr_UART
 * @param char *data
 * @return
 */
int read_from_esp8266(uart_struct * ptr_UART, char *data) {
	int ch;
	char *str = data;
	char *test = data;
	for(int i=0;i<198;i++) {
		read_uart_character(ptr_UART,&ch);
		//printf("read from esp %c  \n",ch);
		*str = ch;
		str++; 
		*str = '\0';
		//printf("string is %s\n",test);
		if(strstr(test,"OK") != NULL)  {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"ERROR") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,">") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"RECV") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
	}
	*str = '\0';
	return 1;
}
#endif

/** 
 * @fn void main() 
 * @brief 
 * @details 
 * @warning
 */
void main()
{	
#ifndef ARTIX7_100T
	char data[200];
	int i=0;
	int baudrate = 9600;
	printf("\n Sending data to cloud using exp8266!....\n");
	printf("\n Baud rate set to %d \n",baudrate);
/***************************************************
    AT ==> response OK
    AT+CWLAP ==> list nearby available WiFi networks
    AT+CIPMUX=0 ==> maximum one connection at any time
    AT+CIPSTART="TCP","api.thingspeak.com",80  ==> connect to the server
    AT+CIPSEND=75 ==>
    GET https://api.thingspeak.com/update?api_key=9T40C2TMUSU1ZX6E&field1=45 ==> send data
    Send \r\n to send data in the link
    AT+CIPCLOSE ==> to close the connection

**************************************************/

	set_baud_rate(uart_instance[1], 9600);
	// Connect to Wifi using esp8266
	flush_uart(uart_instance[1]);
	printf(" sending AT command to esp\n");
	write_to_esp8266(uart_instance[1], "AT");


	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);

	flush_uart(uart_instance[1]);
	printf(" sending AT Echo off command to esp\n");
	write_to_esp8266(uart_instance[1], "ATE0");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);


	printf(" write AT+CIPMUX\n");
	flush_uart(uart_instance[1]);
	write_to_esp8266(uart_instance[1], "AT+CIPMUX=0");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);
/*
	printf(" Waiting to get IP\n",data);
	delay_loop(2000,2000);
	flush_uart(UART1);
	write_to_esp8266("AT+CIFSR",UART1,baudrate);
	read_from_esp8266(data,UART1,baudrate);
	printf(" data from esp :%s\n",data);
*/
	delay_loop(2000,2000);
	printf(" Open connection to thingspeak.com\n",data);
	flush_uart(uart_instance[1]);
	write_to_esp8266(uart_instance[1], "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);

	//delay_loop(2000,2000);
	printf(" write AT+CIPSEND\n");
	flush_uart(uart_instance[1]);
	write_to_esp8266(uart_instance[1], "AT+CIPSEND=75");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);

	printf(" write Data\n");
	flush_uart(uart_instance[1]);
	write_to_esp8266(uart_instance[1], "GET https://api.thingspeak.com/update?api_key=9T40C2TMUSU1ZX6E&field1=35");

	//flush_uart(UART1);
	write_enter_to_esp8266(uart_instance[1]);
//	write_to_esp8266(uart_instance[1], "AT+CIPMUX=0");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);

	// disconnect from link
	printf(" write AT+CIPCLOSE\n");
	flush_uart(uart_instance[1]);
	write_to_esp8266(uart_instance[1], "AT+CIPCLOSE");
	read_from_esp8266(uart_instance[1], data);
	printf(" data from esp :%s\n",data);
#endif

}
