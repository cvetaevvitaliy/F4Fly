#include "sys.h"
#include <stdio.h>
//#include "stm32_config.h"
//#include "usart.h"
#include "sys_usart.h"

void USART_DMA_Config(USART_TypeDef *USARTx, u32 SendBuff);
/*
//��
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
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos ʹ��
#endif
//__align(4) char USART_TX_BUF[USART_SEND_LEN];

//////////////////////////////////////////////////////////////////

#if EN_USART3_RX   //���ʹ���˽���
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA = 0;                //����״̬���
void ATK_Usart3_IQR(void)
{
    u8 res;
#ifdef OS_TICKS_PER_SEC     //���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
    OSIntEnter();
#endif
    if (USART3->SR & (1 << 5)) //���յ�����
    {
        res = USART3->DR;
        if ((USART3_RX_STA & 0x8000) == 0) //����δ���
        {
            if (USART3_RX_STA & 0x4000) //���յ���0x0d
            {
                if (res != 0x0a)USART3_RX_STA = 0; //���մ���,���¿�ʼ
                else USART3_RX_STA |= 0x8000; //���������
            }
            else  //��û�յ�0X0D
            {
                if (res == 0x0d)USART3_RX_STA |= 0x4000;
                else
                {
                    USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
                    USART3_RX_STA++;
                    if (USART3_RX_STA > (USART3_REC_LEN - 1))USART3_RX_STA = 0; //�������ݴ���,���¿�ʼ����
                }
            }
        }
    }
}
#endif

void uart3_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE); //ʹ��GPIOAʱ���Լ����ù���ʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
    //USART3_TX   PB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART3_RX   PB.11
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PB11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    //USART3 NVIC ����
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
	  USART_ClearFlag(USART3, USART_FLAG_TC);
		
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�


}
void USART_ConfigForCom_TX(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef	NVIC_InitStructure;
   USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
   //����PA2��USART2_TX_
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
   GPIO_Init(GPIOA,&GPIO_InitStructure);
   //����PA3��USART2_RX
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA,&GPIO_InitStructure);
   //����USART2���ж�����
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //���ȼ�Ϊ��0��
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   //����USART2
/******************************************************
     ������Ϊ115200,�ֳ�Ϊ8������żУ��λ����ֹλΪ1.
******************************************************/
   USART_InitStructure.USART_BaudRate=256000;
   USART_InitStructure.USART_WordLength=USART_WordLength_8b;
   USART_InitStructure.USART_StopBits=USART_StopBits_1;
   USART_InitStructure.USART_Parity=USART_Parity_No;
   USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
   USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
   USART_Init(USART2,&USART_InitStructure);
   //��USART2�жϴ�����ʽΪ�����ж�
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���ܿ�IT-TXE�ж�ʹ�ܣ���Ϊ��ʼ��ʱҲ����TXE��λ����Ϊ��ʱ�������ݼĴ���Ϊ��
   //ʹ��USART2��ʱ��
   USART_Cmd(USART2,ENABLE);
   //���USART2�ķ��ͽ�����־λ�������һ���ַ����Ͳ���ȥ
   USART_ClearFlag(USART2,USART_FLAG_TC);
   //����2������� 
}


/*
 * ��������DMA_Config
 * ����  ��DMA ���ڵĳ�ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
