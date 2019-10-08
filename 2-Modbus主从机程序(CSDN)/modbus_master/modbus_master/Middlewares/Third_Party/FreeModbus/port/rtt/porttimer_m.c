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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "string.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
static void TIM_Period_Reload(TIM_HandleTypeDef *htim, uint32_t period)
{
	uint32_t tmpcr1 = htim->Instance->CR1;
	
	tmpcr1 &= ~TIM_CR1_ARPE;
	
	htim->Instance->CR1 = tmpcr1;
	
	htim->Instance->ARR = period;
	
	//HAL_TIM_GenerateEvent(htim, TIM_EventSource_Update);	
}

BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
	usT35TimeOut50us = usTimeOut50us;
	
  htim6.Init.Period = usT35TimeOut50us;
	
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	__HAL_TIM_URS_ENABLE(&htim6);
	
  return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
	/* Set current timer mode, don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_T35);
	
	HAL_TIM_Base_Stop_IT(&htim6);
	
	TIM_Period_Reload(&htim6, usT35TimeOut50us);
	
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	
	HAL_TIM_Base_Start_IT(&htim6);
}

void vMBMasterPortTimersConvertDelayEnable()
{
	uint32_t period;
	
  period = MB_MASTER_DELAY_MS_CONVERT * 1000/50;	
	if(period > 65534) {
		period = 65534;
	}

	/* Set current timer mode, don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
	
	HAL_TIM_Base_Stop_IT(&htim6);
	
	TIM_Period_Reload	(&htim6, period);
	
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	
	HAL_TIM_Base_Start_IT(&htim6);	
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
	uint32_t period;
	
	period = MB_MASTER_TIMEOUT_MS_RESPOND * 1000/50;
	if(period > 65534) {
		period = 65534;
	}

	/* Set current timer mode, don't change it.*/
	vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
	
	HAL_TIM_Base_Stop_IT(&htim6);	
	
	TIM_Period_Reload	(&htim6, period);
	
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	
	HAL_TIM_Base_Start_IT(&htim6);
	
	//uint32_t tickstart = HAL_GetTick();
	//printf("===tickstart1 = %d, arr = %d, cnt = %d\r\n", tickstart, htim6.Instance->ARR, htim6.Instance->CNT);
}

void vMBMasterPortTimersDisable()
{
	HAL_TIM_Base_Stop_IT(&htim6);
}

#endif
