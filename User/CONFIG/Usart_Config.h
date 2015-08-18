#ifndef __USART_CONFIG_
#define __USART_CONFIG_
/****************************************/
/***USART***/
/********************************************
***********************************************\
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

/*  选择 Printf() 函数用的串口 */
/*
USART1
USART2
USART3
UART4
UART5
USART6
*/
//#define Printf_USART USART2

#define EN_USART1_ (1) //Enable(1)/Disable(0)
#define EN_USART2_ (0) //Enable(1)/Disable(0)
#define EN_USART3_ (1) //Enable(1)/Disable(0)
#define EN_UART4_  (0) //Enable(1)/Disable(0)
#define EN_UART5_  (0) //Enable(1)/Disable(0)
#define EN_USART6_ (0) //Enable(1)/Disable(0)

#define EN_USART1_PB_6_PB_7   (0)
#define EN_USART1_PA_9_PA_10  (1)

#define EN_USART2_PA_2_PA_3   (1)
#define EN_USART2_PD_5_PD_6   (0)

#define EN_USART3_PB_10_PB_11 (1)
#define EN_USART3_PC_10_PC_11 (0)
#define EN_USART3_PD_8_PD_9   (0)

#define EN_UART4_PC_10_PC_11  (1)
#define EN_UART4_PA_0_PA_1    (0)

#define EN_UART5_PC_12_PD_2   (1)

#define EN_USART6_PG_14_PG_9  (1)
#define EN_USART6_PC_6_PC_7   (0)
/***USART***/
/****************************************/
#endif
