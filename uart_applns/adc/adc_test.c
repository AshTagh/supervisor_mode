/***************************************************************************
 * Project           	           : shakti devt board
 * Name of the file	     	   : adc_test.c
 * Brief Description of file       : code for spi adc which reads using GPIO as SPI.
 * Name of Author    	           : Aditya Dubey, Kotteeswaran.E
 * Email ID                        : kottee.1@gmail.com

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
@file adc_test.c 
@brief  configures the SPI and reads Data
@detail Configures and reads the ADC information using GPIO as SPI.
*/

#include "defines.h"

/** @fn static void writebyte(unsigned char writeData, unsigned char delay)
 * @brief writes a byte to ADC.
 * @details Writes a byte of data to ADC using GPIO over SPI.
 * @param writedata -- Data that needs to be written.
 * @param delay -- Delay for write operation.
 */
static void writebyte(unsigned char writeData, unsigned char delay)
{
	unsigned char j=0,k;
	unsigned long readData = 0;
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SS)) );

	while (j < 5){
	k = writeData;
	k = ( k << j ) & 0x80;
	
        if (j < 4){
        //write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK) );

        if (k == 0)  write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_MOSI)) );
	else write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_MOSI) );
	
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK) );	
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
	}

        else if(j == 4){ 
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));
	write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG)  & ~(SPI_SCLK)));
        }
	++j;
       }        
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) |SPI_SS ))                        ;
}

/** @fn int readbyte(unsigned char writeData)
 * @brief reads a ADC info from ADC.
 * @details Reads ADC using GPIO over SPI.
 * @param delay -- Delay for write operation.
 * @return read value
 */
int readbyte( unsigned char delay)
{
	int  j = 0, i = 0;
	int  bitValue;
	int  readData = 1<<0;
        int  readGpioData = 0;
	
	write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SS)));

	while (j < 12){
	
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));
        	
        readGpioData = read_word(GPIO_DATA_REG)  & SPI_MISO;
        
        if (readGpioData != 0) bitValue = 1;
	else bitValue = 0;
    
        readData = readData << 1;
	readData = readData | bitValue;

        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
        ++j;
        }
        
	//write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SS));
        return readData;
}

/** @fn int total_process(int writeData)
 * @brief configures ADC and reads ADC information.
 * @details Configures ADC and reads the Digital equivalent of the analog signal using GPIO as SPI.
 * @param writeData --- Data that needs to be written.
 * @return Digital equivalent of the analog signal.
 */
int total_process(int writeData)
{
	unsigned char j=0,k;
	// unsigned long readData1 = 0;
	int  bitValue;
	int  readData = 0;
	int  readGpioData = 0;
        int start_bit = 0;

	write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SS)) );

#if 0
	while (j < 17)
	{
		if (j < 4){
			k = writeData; 
			k = ( k << j ) & 0x80;

			if (k == 0)  write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_MOSI)) );
			else write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_MOSI) );

			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK) );  
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
		}
		else if(j == 4){
			write_word(GPIO_DATA_REG,(read_word(GPIO_DATA_REG) &  ~(SPI_MOSI)));
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG)  & ~(SPI_SCLK)));
		}

		else if (j > 4)
		{
			write_word(GPIO_DATA_REG,(read_word(GPIO_DATA_REG) & ~(SPI_MOSI))); 
			
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));

			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));

			readGpioData = read_word(GPIO_DATA_REG)  & SPI_MISO;

			if(readGpioData != 0) bitValue = 1;
			else bitValue = 0;
                        
			if (j == 4){
				j
			}
			
			readData = readData << 1;
			readData = readData | bitValue;	
			//write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
		}
		j++;
	}
	//write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));

#else

       write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));

	while(j < 17)
	{
		if (j < 4)
		{
			k = writeData; 
			k = ( k << j ) & 0x80;

			if (k == 0)  write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_MOSI)) );
			else write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_MOSI) );

			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK) );  
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
		}
	
		else if (j == 4){
			write_word(GPIO_DATA_REG,(read_word(GPIO_DATA_REG) &  ~(SPI_MOSI)));
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG)  & ~(SPI_SCLK)));
		}
		
		else if (j >= 5){
			
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SCLK));
			write_word(GPIO_DATA_REG,(read_word(GPIO_DATA_REG) & ~(SPI_MOSI))); 

			readGpioData = read_word(GPIO_DATA_REG)  & SPI_MISO;

			if (readGpioData != 0) bitValue = 1;
			else bitValue = 0;
			
        	 	//printf("Bit Value: %x, reaData: %x \n",bitValue, readData);
			readData = readData << 1;
			readData = readData | bitValue;
			
			write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
			//write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));
		}

		j++;

	}

#endif	
        // printf("value --> %x \n",readData);
       //printf("value --> %d \n",readData);
        write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) | SPI_SS));
	return readData;
}

/*
   1.) MISO_pin  --> 1st GPIO //0 
   2.) MOSI_pin -->  2nd GPIO // 1
   3.) SCLK_pin -->  3rth GPIO // 1
   4.) SS_Pin   -->  4th GPIO  // 1
 */

/** @fn int main()
 * @brief reads adc value and prints the same.
 * @details Configures ADC and reads the digital value and prints the same.
 * @param None.
 * @return Zero.
 */
int main()
{
        int stuff;
	int ans; 

        write_word(GPIO_DIRECTION_CNTRL_REG, (read_word(GPIO_DIRECTION_CNTRL_REG) & ~(SPI_MISO)));
	write_word(GPIO_DIRECTION_CNTRL_REG, (read_word(GPIO_DIRECTION_CNTRL_REG) | SPI_MOSI));
	write_word(GPIO_DIRECTION_CNTRL_REG, (read_word(GPIO_DIRECTION_CNTRL_REG) | SPI_SS));
	write_word(GPIO_DIRECTION_CNTRL_REG, (read_word(GPIO_DIRECTION_CNTRL_REG) | SPI_SCLK));

	write_word(GPIO_DATA_REG, (read_word(GPIO_DATA_REG) & ~(SPI_SCLK)));

	for (;;)
	{
		       //writebyte(0xC0,1);	

		stuff = total_process(0xD0);
		
		ans = (stuff*500)/4096;
		
		printf("temperature --> %d\n",stuff);
		printf("temp2 ---> %d\n",ans);
		
                //writebyte(0xC0,1);
		//stuff = readbyte(1);
                delay_loop(150,150);
    
    //write_word(GPIO_DIRECTION_CNTRL_REG, (read_word(GPIO_DIRECTION_CNTRL_REG) & ~(SPI_SCLK)));
    
    //stuff = readbyte(50);
 
        }
return 0;
}



