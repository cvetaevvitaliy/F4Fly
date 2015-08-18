#include "USART.h"
/********************************************
USART1_CK  |   PA8    |          |          |
USART1_TX  |   PA9    |   PB6    |          |
USART1_RX  |   PA10   |   PB7    |          |
USART1_CTS |   PA11   |          |          |
USART1_RTS |   PA12   |          |          |
********************************************|
USART2_RTS |   PA1    |   PD4    |          |
USART2_TX  |   PA2    |   PD5    |          |
USART2_RX  |   PA3    |   PD6    |          |
USART2_CK  |   PA4    |   PD7    |          |
USART2_CTS |   PD3    |          |          |
********************************************|
USART3_TX  |   PB10   |   PD8    |   PC10   |
USART3_RX  |   PB11   |   PD9    |   PC11   |
USART3_CK  |   PB12   |   PD10   |   PC12   |
USART3_CTS |   PB13   |   PD11   |          |
USART3_RTS |   PB14   |   PD12   |          |
********************************************|
UART4_TX   |   PA0    |   PC10   |          |
UART4_RX   |   PA1    |   PC11   |          |
********************************************|
UART5_TX   |   PC12   |          |          |
UART5_RX   |   PD2    |          |          |
********************************************|
USART6_TX  |   PC6    |   PG14   |          |
USART6_RX  |   PC7    |   PG9    |          |
USART6_CK  |   PC8    |   PG7    |          |
USART6_RTS |   PG8    |   PG12   |          |
USART6_CTS |   PG15   |          |          |
UART6_CTS  |   PG13   |          |          |
********************************************/
#if EN_USART1_
u8 RxBuffer1[100] = {"1ce shi "};
u8 RxCounter1 = 0;
#endif
#if EN_USART2_
u8 RxBuffer2[100] = {"2ce shi "};
u8 RxCounter2 = 0;
#endif
#if EN_USART3_
u8 RxBuffer3[100] = {"3ce shi "};
u8 RxCounter3 = 0;
#endif
#if EN_UART4_
u8 RxBuffer4[100] = {"4ce shi "};
u8 RxCounter4 = 0;
#endif
#if EN_UART5_
u8 RxBuffer5[100] = {"5ce shi "};
u8 RxCounter5 = 0;
#endif
#if EN_USART6_
u8 RxBuffer6[100] = {"6ce shi "};
u8 RxCounter6 = 0;
#endif

#if EN_USART1_
void USART1_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#if EN_USART1_PA_9_PA_10
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
#endif
#if EN_USART1_PB_6_PB_7
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
#endif

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if EN_USART1_PA_9_PA_10
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if  EN_USART1_PB_6_PB_7
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}
#endif
#if EN_USART2_
void USART2_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#if EN_USART2_PA_2_PA_3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
#endif
#if EN_USART2_PD_5_PD_6
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
#endif

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if EN_USART2_PA_2_PA_3
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if EN_USART2_PD_5_PD_6
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}
#endif
#if EN_USART3_
void USART3_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

#if EN_USART3_PB_10_PB_11
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
#endif
#if EN_USART3_PC_10_PC_11
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
#endif
#if EN_USART3_PD_8_PD_9
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
#endif
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if EN_USART3_PB_10_PB_11
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#if EN_USART3_PC_10_PC_11
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#if EN_USART3_PD_8_PD_9
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}
#endif
#if EN_UART4_
void UART4_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
#if EN_UART4_PC_10_PC_11
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
#endif
#if EN_UART4_PA_0_PA_1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
#endif
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if EN_UART4_PC_10_PC_11
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#if EN_UART4_PA_0_PA_1
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);
}
#endif
#if EN_UART5_
void UART5_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART5, &USART_InitStructure);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART5, ENABLE);
}
#endif
#if EN_USART6_
void USART6_Configuration(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
#if EN_USART6_PG_14_PG_9
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
#endif
#if EN_USART6_PC_6_PC_7
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
#endif
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#if EN_USART6_PG_14_PG_9
    GPIO_Init(GPIOG, &GPIO_InitStructure);
#endif
#if EN_USART6_PC_6_PC_7
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART6, &USART_InitStructure);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART6, ENABLE);
}
#endif

void USART_NVIC_Configuration(u8 USART ,u8 PriorityGroup,u8 PreemptionPriority,u8 SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;

    if(PriorityGroup==0)NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	else if(PriorityGroup==1)NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	else if(PriorityGroup==2)NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	else if(PriorityGroup==3)NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	else if(PriorityGroup==4)NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	else {}
  
    if(USART==1)NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	else if(USART==2)NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	else if(USART==3)NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	else if(USART==4)NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	else if(USART==5)NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	else if(USART==6)NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	else {}
    if(PreemptionPriority!=0)NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
//void USART_NVIC_Configuration(u8 USART , u8 PreemptionPriority, u8 SubPriority)
//{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    if      (USART == 1)NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//    else if (USART == 2)NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//    else if (USART == 3)NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    else if (USART == 4)NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//    else if (USART == 5)NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//    else if (USART == 6)NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
//    else {}
//    if (PreemptionPriority != 0)NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
/*************************ASCII**********************************************/
void Send_ASCII(u8 USART , u8 *b)
{
    if (USART == 1)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(USART1, *b);
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
    }
    else if (USART == 2)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(USART2, *b);
            while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        }
    }
    else if (USART == 3)
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(USART3, *b);
            while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        }
    }
    else if (USART == 4)
    {
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(UART4, *b);
            while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
        }
    }
    else if (USART == 5)
    {
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(UART5, *b);
            while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
        }
    }
    else if (USART == 6)
    {
        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
        for (; *b != '\0'; b++)
        {
            USART_SendData(USART6, *b);
            while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
        }
    }
}

/*************************??16???**********************************************/
void Send_Hex(u8 USART, u8 c)
{
    if (USART == 1)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, c);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
    else if (USART == 2)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, c);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    else if (USART == 3)
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, c);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    else if (USART == 4)
    {
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
        USART_SendData(UART4, c);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
    }
    else if (USART == 5)
    {
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
        USART_SendData(UART5, c);
        while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
    }
    else if (USART == 6)
    {
        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
        USART_SendData(USART6, c);
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
    }
}
void Send_USART_Hex(u8 USART, u8 *b, u8 i)
{
    u8 j;
    for (j = 0; j < i; j++)Send_Hex(USART, *(b + j));
}
#if EN_USART1_
void myusart1(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        if (RxCounter1 == 100)
            USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
}
//void WTH_USART1_IRQHandler(void)
//{
//    Uart1_IRQ();
//}
#endif
#if EN_USART2_
void WTH_USART2_IRQHandler(void)
{

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        RxBuffer2[RxCounter2++] = USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        if (RxCounter2 == 100)
            USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    }
}
#endif
//#if EN_USART3_
//void USART3_IRQHandler(void)
//{

//    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//    {
//        RxBuffer3[RxCounter3++] = USART_ReceiveData(USART3);
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//        if (RxCounter3 == 100)
//            USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//    }
//}
//#endif
#if EN_UART4_
void UART4_IRQHandler(void)
{

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        RxBuffer4[RxCounter4++] = USART_ReceiveData(UART4);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        if (RxCounter4 == 100)
            USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    }
}
#endif
#if EN_UART5_
void UART5_IRQHandler(void)
{

    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        RxBuffer5[RxCounter5++] = USART_ReceiveData(UART5);
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        if (RxCounter5 == 100)
            USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
    }
}
#endif
#if EN_USART6_
void USART6_IRQHandler(void)
{
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        RxBuffer6[RxCounter6++] = USART_ReceiveData(USART6);
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        if (RxCounter6 == 100)
            USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
    }
}
#endif
