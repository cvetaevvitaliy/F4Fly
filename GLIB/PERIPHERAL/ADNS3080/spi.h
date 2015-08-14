#ifndef spi_h
 #define spi_h
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h" 
#include "usart.h"
#include "delay.h"
void SPI_init(int flag);
char SPI_SendReceive(char data);
#endif

