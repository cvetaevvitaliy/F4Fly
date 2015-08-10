#ifndef __MPU6050_H_
#define __MPU6050_H_
#ifdef __cplusplus
       extern "C" {
#endif
#include "stm32f10x.h"
#define RtA 		57.324841				
#define AtR    	0.0174533				
#define Acc_G 	0.0011963	
typedef struct{
				int16_t x;
				int16_t y;
				int16_t z;}S_INT16_XYZ;
				
extern S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST,MPU6050_GYRO_Gr;	 //最新一次读取值
extern S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			         //零漂
extern u8							GYRO_OFFSET_OK;
extern u8							ACC_OFFSET_OK;
extern float     MPU6050_GYRO_Gr_x,MPU6050_GYRO_Gr_y,MPU6050_GYRO_Gr_z;  

void MPU6050_Init(void);
void MPU6050_Read(void);
void MPU6050_Dataanl(void);
void MPU6050_CalOff_Acc(void);
void MPU6050_CalOff_Gyr(void);
#ifdef __cplusplus
        }
#endif
#endif
