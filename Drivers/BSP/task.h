#ifndef __TASK_H__
#define __TASK_H__
#include "./SYSTEM/sys/sys.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
// #include "./BSP/LCD/lcd.h"
#include "./BSP/BEEP/beep.h"
// #include "./BSP/R9/joystic.h"
#include "./BSP/R9/underpanControl.h"
#include "./BSP/R9/moterdriver.h"
#include "./BSP/R9/brake.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/Common/common.h"
#include "./BSP/R9/overcurrent_protection.h"
// #include "./BSP/R9/hallcap.h"
// #include "./BSP/R9/stmflash.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/R9/mpu6050.h"
#include "./BSP/R9/inv_mpu.h"
#include "./BSP/R9/inv_mpu_dmp_motion_driver.h" 

#include "./BSP/R9/rs485.h"


//任务列表
void LedFlash(void);
void UnderpanDrive(void);
void Hard_devInit(void);
void Beep_run(void);
void linearactuatorDrive(void);
void GetADC_AllData(void);
// void flashwriteandred(void);
// void GetjoysticData(void);
void gyroscopeData(void);
void DypA21 (void);


#endif

//     { 0,       1,              0,           linearactuatorDrive},
//     { 0,       2,              0,           gyroscopeData},
//	 { 0,       3,              0,                   UnderpanDrive},
//   	 { 0,       4,              0,                GetADC_AllData},
//	 { 0,       200,            0,		  	           LedFlash},
//	 { 0,       500,            0,                      Beep_run}