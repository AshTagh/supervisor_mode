/***************************************************************************
* Project           		: shakti devt board
* Name of the file	     	: defines.h
* Brief Description of file     : Header file GPIO based SPI ADC read.
* Name of Author    	        : Aditya Dubey, Kotteeswaran 
* Email ID                      : kottee.1@gmail.com

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
@file defines.h 
@brief  SPI ADC header file
@detail Header file for ADC using GPIO based SPI.
*/

//#ifdef ECLASS
#define GPIO_START 0X00000000
#define GPIO_DIRECTION_CNTRL_REG (GPIO_START + 0X00)
#define GPIO_DATA_REG  (GPIO_START + 0x8)
//#endif

/*
#ifdef CCLASS
#define GPIO_START 0X11100
#define GPIO_DIRECTION_CNTRL_REG (GPIO_START + 0X00)
#define GPIO_DATA_REG  (GPIO_START + 0X04)
#define GPIO_DRV_CNTRL_REG0 (GPIO_START + 0X08)
#define GPIO_DRV_CNTRL_REG1 (GPIO_START + 0X0C)
#define GPIO_DRV_CNTRL_REG2 (GPIO_START + 0X10)
#define GPIO_PULL_DOWN_CNTRL_REG (GPIO_START + 0X14)
#define GPIO_MODE_SEL_REG (GPIO_START + 0X18)
#define GPIO_PROG_SLEW_CNTRL_REG (GPIO_START + 0X1C)
#define GPIO_PULL_UP_CNTRL_REG (GPIO_START + 0X20)
#define CORE_PWR_DOWN_STS_REG0 (GPIO_START + 0X24)
#define CORE_PWR_DOWN_STS_REG0 (GPIO_START + 0X28)
#endif
*/

#ifdef MOTOR_CONTROL
#define MOTOR_FORWARD_CNTRL (0xA << 24)
#define MOTOR_REVERSE_CNTRL (0x5 << 24)
#endif

#ifdef KEYPAD
#define KYPD_ROWNUM 4
#define KYPD_COLNUM 4
#define OUTPINS_OFFSET 24
#define INPINS_OFFSET 16
#define OUTPINS (0xF << OUTPINS_OFFSET)
#define INPINS  (0xF << INPINS_OFFSET)
#endif

#ifdef 	NEXYS_VIDEO
#define INOUTPIN_COUNT 21
#define INOUTPIN_MASK 0x1FFFFF

#define MOTOR_FORWARD_CNTRL (0x5 << 0)
#define MOTOR_REVERSE_CNTRL (0xA << 0)
#define MOTOR_RIGHT_CNTRL (0x6 << 0)
#define MOTOR_LEFT_CNTRL (0x9 << 0)
#define MOTOR_STOP_CNTRL (0xF << 0)
#endif

#ifdef LINE_TRACER_ARRAY_IR_SENSOR
#define LOW 0
#define HIGH 1
#endif

#define IR_1 (1 << 4)
#define IR_2 (1 << 5)
#define IR_3 (1 << 6)
#define IR_4 (1 << 7)
#define IR_5 (1 << 8)


#define SPI_MISO 1<<0 //1st bit 
#define SPI_MOSI 1<<1 //2nd bit
#define SPI_SCLK 1<<2 //3rd bit
#define SPI_SS   1<<3 //4th bit
#define SPI_ADC_IN 0xC0


