#ifndef __MPU6050_H_
#define __MPU6050_H_

#include "stm32f4xx.h"
#include "ANO_Tech_STM32_I2C.h"
#include "flash_eeprom.h"

#define devAddr  0xD0

typedef struct{
				int16_t x;
				int16_t y;
				int16_t z;}S_INT16_XYZ;
extern S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值
extern S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂
extern u8							GYRO_OFFSET_OK;
extern u8							ACC_OFFSET_OK;

void MPU6050_Init(void);
void MPU6050_Read(void);
void MPU6050_Dataanl(void);

#endif
