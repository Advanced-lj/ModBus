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
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

#include "port.h"
#include "usart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Static variables ---------------------------------*/
static uint8_t recv_byte;

/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(xRxEnable == TRUE) {
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
		HAL_UART_Receive_IT(&huart2, &recv_byte, 1);
	} else {
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
	}
	
	if(xTxEnable == TRUE) {
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
		set_rs485_mode(RS485_TX);
	} else {
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);	
	}
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	UART_HandleTypeDef *cur_uart;
	
	if(ucPORT == 1) {
		cur_uart = &huart1;
	} else if(ucPORT == 2) {
		cur_uart = &huart2;
	} else {
		return FALSE;
	}			

	if(ucDataBits == 7) {
		cur_uart->Init.WordLength = UART_WORDLENGTH_7B;
	} else if(ucDataBits == 8) {
		cur_uart->Init.WordLength = UART_WORDLENGTH_8B;
	} else if(ucDataBits == 9) {
		cur_uart->Init.WordLength = UART_WORDLENGTH_9B;
	} else {
		return FALSE;
	}
	
	if(eParity == MB_PAR_NONE) {
		cur_uart->Init.Parity = UART_PARITY_NONE;
	} else if(eParity == MB_PAR_ODD) {
		cur_uart->Init.Parity = UART_PARITY_ODD;
	} else if(eParity == MB_PAR_EVEN) {
		cur_uart->Init.Parity = UART_PARITY_EVEN;
	} else {
		return FALSE;
	}
	
	cur_uart->Init.BaudRate = ulBaudRate;

  if (HAL_UART_Init(cur_uart) != HAL_OK)
  {
    return FALSE;
  }
	
	return TRUE;
}

void vMBPortClose(void)
{

}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
	HAL_UART_Transmit_IT(&huart2, (uint8_t *)&ucByte, 1);
	return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = recv_byte;
	HAL_UART_Receive_IT(&huart2, &recv_byte, 1);
	return TRUE;
}

