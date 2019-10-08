#include "main.h"
#include "stdio.h"
#include "mb.h"
#include "mb_m.h"
#include "user_mb_app.h"
#include "modbus_example.h"

static void MBSysMonitorTask(void const * argument);

void ModBusProcessTask(void const * argument)
{
	printf("enter %s ...\r\n", __func__);
	
#if MB_MASTER_RTU_ENABLED > 0
	eMBMasterInit(MB_RTU, 0x01, 9600, MB_PAR_NONE, 8);
	eMBMasterEnable();
  osThreadDef(MBSysMonitor, MBSysMonitorTask, osPriorityNormal, 0, MB_MONITOR_TASK_STACKSIZE);
  osThreadCreate(osThread(MBSysMonitor), NULL);
#else 
	eMBInit(MB_RTU, 0x01, 0x05, 9600, MB_PAR_NONE);
	eMBEnable();
#endif
	
	for(;;) {
#if MB_MASTER_RTU_ENABLED > 0	
		eMBMasterPoll();
#else		
		eMBPoll();
#endif
	}
}

static void MBSysMonitorTask(void const * argument)
{
	eMBMasterReqErrCode ret;
	uint8_t slaveid;
	uint8_t regaddr;
	uint8_t regnums;
	uint8_t i;
	
	for(;;) {	
		osDelay(100);
		//read coils
		slaveid = 1;
		regaddr = 1;
		regnums = 2;		
		ret = eMBMasterReqReadCoils(slaveid, regaddr, regnums, 500);
		if(ret == MB_MRE_NO_ERR) {
			printf("===eMBMasterReqReadCoils successful! values : ");
			for(i = 0; i < regnums; i++) {
				printf("%x ", xMBUtilGetBits(&ucMCoilBuf[slaveid - 1][0], regaddr + i, 1));
			}
			printf("\r\n");
		} else {
			printf("===eMBMasterReqReadCoils failed!\r\n");
		}			
		osDelay(100);
		
		//read discreteinputs
		slaveid = 2;
		regaddr = 2;
		regnums = 3;	
		ret = eMBMasterReqReadDiscreteInputs(slaveid, regaddr, regnums, 500);
		if(ret == MB_MRE_NO_ERR) {
			printf("===eMBMasterReqReadDiscreteInputs successful! values : ");
			for(i = 0; i < regnums; i++) {
				printf("%x ", xMBUtilGetBits(&ucMDiscInBuf[slaveid - 1][0], regaddr + i, 1));
			}
			printf("\r\n");
		} else {
			printf("===eMBMasterReqReadDiscreteInputs failed!\r\n");
		}	
		osDelay(100);

		//read inputregister
		slaveid = 3;
		regaddr = 0;
		regnums = 1;
		ret = eMBMasterReqReadInputRegister(slaveid, regaddr, regnums, 500);
		if(ret == MB_MRE_NO_ERR) {
			printf("===eMBMasterReqReadInputRegister successful! values : ");
			for(i = 0; i < regnums; i++) {
				printf("%x ", usMRegInBuf[slaveid-1][regaddr+i]);
			}
			printf("\r\n");
		} else {
			printf("===eMBMasterReqReadInputRegister failed!\r\n");
		}
		osDelay(100);
		

		//read holdingregister
		slaveid = 4;
		regaddr = 0;
		regnums = 1;		
		ret = eMBMasterReqReadHoldingRegister(slaveid, regaddr, regnums, 500);
		if(ret == MB_MRE_NO_ERR) {
			printf("===eMBMasterReqReadHoldingRegister successful! values : ");
			for(i = 0; i < regnums; i++) {
				printf("%x ", usMRegHoldBuf[slaveid-1][regaddr+i]);
			}
			printf("\r\n");
		} else {
			printf("===eMBMasterReqReadHoldingRegister failed!\r\n");
		}

		osDelay(5000);
	}
}

