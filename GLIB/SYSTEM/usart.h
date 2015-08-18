#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "Usart_Config.h"


/**********************************************\
 * USART1_CK  |   PA8    |          |          *
 * USART1_TX  |   PA9    |   PB6    |          *
 * USART1_RX  |   PA10   |   PB7    |          *
 * USART1_CTS |   PA11   |          |          *
 * USART1_RTS |   PA12   |          |          *
 * ******************************************* *
 * USART2_RTS |   PA1    |   PD4    |          *
 * USART2_TX  |   PA2    |   PD5    |          *
 * USART2_RX  |   PA3    |   PD6    |          *
 * USART2_CK  |   PA4    |   PD7    |          *
 * USART2_CTS |   PD3    |          |          *
 * ******************************************* *
 * USART3_TX  |   PB10   |   PD8    |   PC10   *
 * USART3_RX  |   PB11   |   PD9    |   PC11   *
 * USART3_CK  |   PB12   |   PD10   |   PC12   *
 * USART3_CTS |   PB13   |   PD11   |          *
 * USART3_RTS |   PB14   |   PD12   |          *
 * ******************************************* *
 * UART4_TX   |   PA0    |   PC10   |          *
 * UART4_RX   |   PA1    |   PC11   |          *
 * ******************************************* *
 * UART5_TX   |   PC12   |          |          *
 * UART5_RX   |   PD2    |          |          *
 * ******************************************* *
 * USART6_TX  |   PC6    |   PG14   |          *
 * USART6_RX  |   PC7    |   PG9    |          *
 * USART6_CK  |   PC8    |   PG7    |          *
 * USART6_RTS |   PG8    |   PG12   |          *
 * USART6_CTS |   PG15   |          |          *
 * UART6_CTS  |   PG13   |          |          *
\**********************************************/

#ifdef EN_USART1
extern u8 RxBuffer1[100];
extern u8 RxCounter1;
#endif
#ifdef EN_USART2
extern u8 RxBuffer2[100];
extern u8 RxCounter2;
#endif
#ifdef EN_USART3
extern u8 RxBuffer3[100];
extern u8 RxCounter3;
#endif
#ifdef EN_UART4
extern u8 RxBuffer4[100];
extern u8 RxCounter4;
#endif
#ifdef EN_UART5
extern u8 RxBuffer5[100];
extern u8 RxCounter5;
#endif
#ifdef EN_USART6
extern u8 RxBuffer6[100];
extern u8 RxCounter6;
#endif

void USART_NVIC_Configuration(u8 USART ,u8 PriorityGroup,u8 PreemptionPriority,u8 SubPriority);
/*
 *   USART_NVIC_Configuration(1 ,1,0,2);
 */
void USART_Configuration(void);
void USART1_Configuration(u32 BaudRate);
void USART2_Configuration(u32 BaudRate);
void USART3_Configuration(u32 BaudRate);
void UART4_Configuration(u32 BaudRate);
void UART5_Configuration(u32 BaudRate);
void USART6_Configuration(u32 BaudRate);

void Send_ASCII(u8 USART , u8 *b);
void Send_Hex(u8 USART, u8 c);
void Send_USART_Hex(u8 USART, u8 *b, u8 i);

#endif

