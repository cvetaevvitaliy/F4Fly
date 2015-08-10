#ifndef spi3080_h
 #define spi3080_h
#include "stm32f10x.h"
#include "stm32f10x_spi.h" 
#include "delay.h"
#define  ON_CS()    {GPIO_ResetBits(GPIOA,GPIO_Pin_9);}//delay_us(1);
#define  OFF_CS()   {GPIO_SetBits(GPIOA,GPIO_Pin_9);}//delay_us(1);
void SPI_init(int flag);
char SPI_SendReceive(char data);
#endif

