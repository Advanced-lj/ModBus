/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/
static uint8_t g_recv_byte;
/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	UART_HandleTypeDef *rs485_uart;
	
	//printf("===rs485 baud = %d, databits = %d, parity = %d\r\n", ulBaudRate, ucDataBits, eParity);
	
	rs485_uart = &RS485_UART;	
	
	rs485_uart->Init.BaudRate = ulBaudRate;
	
	if(eParity == MB_PAR_NONE) {
		rs485_uart->Init.Parity = UART_PARITY_NONE;
	} else if(eParity == MB_PAR_ODD) {
		rs485_uart->Init.Parity = UART_PARITY_ODD;
		ucDataBits++;
	} else if(eParity == MB_PAR_EVEN) {
		rs485_uart->Init.Parity = UART_PARITY_EVEN;
		ucDataBits++;
	} else {
		return FALSE;
	}
	
	if(ucDataBits == 7) {
		rs485_uart->Init.WordLength = UART_WORDLENGTH_7B;
	} else if(ucDataBits == 8) {
		rs485_uart->Init.WordLength = UART_WORDLENGTH_8B;
	} else if(ucDataBits == 9) {
		rs485_uart->Init.WordLength = UART_WORDLENGTH_9B;
	} else {
		return FALSE;
	}
	
  if (HAL_UART_Init(rs485_uart) != HAL_OK) {
    return FALSE;
  }

	return TRUE;
}

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if(xTxEnable == TRUE) {
		set_rs485_mode(RS485_TX);
		__HAL_UART_ENABLE_IT(&RS485_UART, UART_IT_TC);		
	} else {
		set_rs485_mode(RS485_RX);
		__HAL_UART_DISABLE_IT(&RS485_UART, UART_IT_TC);			
	}
	
	if(xRxEnable == TRUE) {				
		__HAL_UART_ENABLE_IT(&RS485_UART, UART_IT_RXNE);		
		HAL_UART_Receive_IT(&RS485_UART, &g_recv_byte, 1);
	} else {
		__HAL_UART_DISABLE_IT(&RS485_UART, UART_IT_RXNE);
	}	
}

void vMBMasterPortClose(void)
{

}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
	HAL_UART_Transmit_IT(&RS485_UART, (uint8_t *)&ucByte, 1);
	return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = g_recv_byte;
	HAL_UART_Receive_IT(&RS485_UART, &g_recv_byte, 1);
	return TRUE;
}
#endif
