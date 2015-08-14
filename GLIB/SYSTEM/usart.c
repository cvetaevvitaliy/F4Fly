#include "sys.h"
#include <stdio.h>
//#include "stm32_config.h"
//#include "usart.h"
#include "sys_usart.h"

void USART_DMA_Config(USART_TypeDef *USARTx, u32 SendBuff);
/*
//例
    if(USART2_RX_STA&0x8000)
    {
        u16 vol=0;
        u8 i;
        for(i=0;i<(USART2_RX_STA&0x3fff);i++)
        {
            vol=vol*10+(USART2_RX_BUF[i]-0x30);
        }
        USART2_RX_STA=0;
        USART_OUT(USART2,"%d\r\n",vol);
        Dac1_Set_Vol(vol);
    }
*********************************************
REMAP |  00      |   01         | 11        |
      |  TX  RX  |   TX  RX     | TX  RX    |
******|**********|**************|*******    |
USART1|  A9  A10 |   B6  B7     |           |
******|**********|**************|*******    |
USART2|  A2  A3  |   (D5  D6)   |           |
      |          |  (100)(144)  |           |
******|**********|**************|*******    |
USART3|  B10 B11 |   (C10 C11)  | (D8  D9)  |
      |          | (64 100 144) | (100 144) |
*********************************************
2013/8/27                            ********
2013/7/21                            ********
*/
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#endif
//__align(4) char USART_TX_BUF[USART_SEND_LEN];

//////////////////////////////////////////////////////////////////

#if EN_USART3_RX   //如果使能了接收
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0;                //接收状态标记
void ATK_Usart3_IQR(void)
{
    u8 res;
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntEnter();
#endif
    if (USART3->SR & (1 << 5)) //接收到数据
    {
        res = USART3->DR;
        if ((USART3_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART3_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)USART3_RX_STA = 0; //接收错误,重新开始
                else USART3_RX_STA |= 0x8000; //接收完成了
            }
            else  //还没收到0X0D
            {
                if (res == 0x0d)USART3_RX_STA |= 0x4000;
                else
                {
                    USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
                    USART3_RX_STA++;
                    if (USART3_RX_STA > (USART3_REC_LEN - 1))USART3_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}
#endif

void uart3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE); //使能GPIOA时钟以及复用功能时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1
    //USART3_TX   PB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART3_RX   PB.11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PB11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口
    //USART3 NVIC 配置
    USART_Cmd(USART3, ENABLE);                    //使能串口
	  USART_ClearFlag(USART3, USART_FLAG_TC);
		
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断


}
void USART_ConfigForCom_TX(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef	NVIC_InitStructure;
   USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
   //配置PA2即USART2_TX_
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
   GPIO_Init(GPIOA,&GPIO_InitStructure);
   //配置PA3即USART2_RX
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA,&GPIO_InitStructure);
   //进行USART2的中断配置
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //优先级为第0级
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   //配置USART2
/******************************************************
     波特率为115200,字长为8，无奇偶校验位，终止位为1.
******************************************************/
   USART_InitStructure.USART_BaudRate=256000;
   USART_InitStructure.USART_WordLength=USART_WordLength_8b;
   USART_InitStructure.USART_StopBits=USART_StopBits_1;
   USART_InitStructure.USART_Parity=USART_Parity_No;
   USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
   USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   USART_Init(USART2,&USART_InitStructure);
   //开USART2中断触发方式为接收中断
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//不能开IT-TXE中断使能，因为初始化时也会有TXE置位，因为此时发送数据寄存器为空
   //使能USART2的时钟
   USART_Cmd(USART2,ENABLE);
   //清除USART2的发送结束标志位，以免第一个字符发送不出去
   USART_ClearFlag(USART2,USART_FLAG_TC);
   //串口2设置完成 
}


/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
