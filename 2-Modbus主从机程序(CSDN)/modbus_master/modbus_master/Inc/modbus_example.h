#ifndef __MODBUS_EXAMPLE_H__
#define __MODBUS_EXAMPLE_H__

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define MB_MONITOR_TASK_STACKSIZE 512
#define MB_TASK_STACKSIZE         512

extern void ModBusProcessTask(void const * argument);

#endif
