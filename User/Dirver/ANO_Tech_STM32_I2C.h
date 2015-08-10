#ifndef __STM32_I2C_H
#define __STM32_I2C_H
#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/

uint8_t ANO_Tech_I2C1_Write_Int(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteNum, uint8_t * WriteTemp);
//此函数执行后会立即返回，如果iic正忙，返回0，如果触发写成功，返回1
uint8_t ANO_Tech_I2C1_Write_Buf(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteNum, uint8_t * WriteTemp);
//此函数执行后会等待写操作完成后返回，完成后，返回1
uint8_t ANO_Tech_I2C1_Write_1Byte_Int(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
//此函数执行后会立即返回，如果iic正忙，返回0，如果触发写成功，返回1
uint8_t ANO_Tech_I2C1_Write_1Byte_Buf(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
//此函数执行后会等待写操作完成后返回，完成后，返回1
uint8_t ANO_Tech_I2C1_Read_Int(uint8_t DevAddr, uint8_t RegAddr, uint8_t ReadNum, uint8_t * ReadTemp);
//此函数执行后会立即返回，如果iic正忙，返回0，如果触发写成功，返回1
uint8_t ANO_Tech_I2C1_Read_Buf(uint8_t DevAddr, uint8_t RegAddr, uint8_t ReadNum, uint8_t * ReadTemp);
//此函数执行后会等待读操作完成后返回，完成后，返回1

void I2C1_INIT(u8 ownadd, u32 speed, u8 nvic_er_p, u8 nvic_er_s, u8 nvic_ev_p, u8 nvic_ev_s);
//ownadd：器件地址，speed：iic速度，nvic_er_p：iic错误中断抢占优先级。。依此类推
void I2C1_RESET(void);

void I2C1_EV_IRQ( void );
//此函数必须放入stm32F4xx_it.c中的I2C1_EV_IRQHandler函数内
void I2C1_ER_IRQ( void );
//此函数必须放入stm32F4xx_it.c中的I2C1_ER_IRQHandler函数内
/*====================================================================================================*/
/*====================================================================================================*/
#endif
