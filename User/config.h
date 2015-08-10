#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "stm32f4xx.h"
#include "ANO_Tech_STM32_I2C.h"
#include "MPU6050.h"
#include "Imu.h"
#include "data_transfer.h"
#include "flash_eeprom.h"


#define USE_BARO
#define USE_COMPASS
#define DATA_TRANSFER_USE_USART
//#define DATA_TRANSFER_USE_NRF
#define RC_CHANL_NUM	7
#define RC_MIDDLE 		1500
#define RC_MIN				1000
#define	RC_MAX				2000
#define RC_DEADBAND		5
#define RC_USE_DEADBAND




void Timer_Sys_Irq(void);
void Nvic_Init(void);

#endif 
