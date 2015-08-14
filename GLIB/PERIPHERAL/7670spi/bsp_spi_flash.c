 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "sys_usart.h"
#include "usr_usart.h"
#include "minos_delay.h"
#include "bsp_spi_flash.h"/*******************************************************************************
* Module        : SPI1_DMA_Configuration
* Description   : SPI1�Լ���ӦDMAͨ����������
* Compile       : Keil MDK 4.10
* Device        ��STM32F103RBT6
* Author        : �ְ���   (�޸�2012/4/30)
* Modefied data : 2012/3/8 
* Version       ��V1.0
* Copyright(C) �ְ��� 20012-2022	 
* All rights reserved
*******************************************************************************/



vu8 SPI1_RX_Buff[buffersize] = { 0 };    //���ջ�������ʼ��       
vu8 SPI1_TX_Buff[buffersize] = {0x55 };	 //���ͻ�������ʼ��
//ԭ��ķ��͵Ķ���ͳ�ʼ���ڽ��յ�ǰ�棬ʹ��������450��Լ�����ϵĴ���ʱ���ֽ��ղ������߽�
//�ճ�������⣬��֪���������˻�������С֮��᲻�����ͬ������������������֮��������
//������û������
//vu8=volatile unsigned char,volatile�������η��Ǳ�����������α���ͬ�̷߳��ʺ��޸ĵı���
							  
/*******************************************************************************
* Function Name  : SPI1_Configuration
* Description    : SPIģ��������IO�ڵ����ã�SPI1����Ϊ���٣���Fsck=Fcpu/256
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_Configuration( void )
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��

  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5����Ϊ SPI1
	
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* ʱ�ӵĵ�2�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	/* ������Ԥ��Ƶϵ����4��Ƶ */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI1, &SPI_InitStructure);

    // Enable DMA1 Channel3 //
    DMA_Cmd(DMA1_Stream4, ENABLE);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	SPI_Cmd(SPI1, ENABLE);		/* ʹ��SPI  */
    

} 

  
/*******************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : SPI�����ٶ�Ϊ����
* Input          : u8 SpeedSet 
*                  ����ٶ���������0�������ģʽ����0�����ģʽ
*                  SPI_SPEED_HIGH   1
*                  SPI_SPEED_LOW    0
��                  ע�������ٶȻ��ȹر�SPI����������ڳ����ͽ�������
*                  һ����շ������ݶ��еȴ�������ɣ��������ﲻ��
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_SetSpeed( u8 SpeedSet )
{	
	SPI1->CR1 &= 0XFF87 ;                                    //�ر�SPI1,Fsck=Fcpu/2

	switch( SpeedSet )
	{
	    case SPI_SPEED_2   : SPI1->CR1 |= 0<<3 ; break ;     //2��Ƶ:Fsck=Fpclk/2=36Mhz;
		case SPI_SPEED_4   : SPI1->CR1 |= 1<<3 ; break ;     //4��Ƶ:Fsck=Fpclk/4=18Mhz;  
		case SPI_SPEED_8   : SPI1->CR1 |= 2<<3 ; break ;     //8��Ƶ:Fsck=Fpclk/8=9Mhz
		case SPI_SPEED_16  : SPI1->CR1 |= 3<<3 ; break ;     //16��Ƶ:Fsck=Fpclk/16=4.5Mhz
		case SPI_SPEED_32  : SPI1->CR1 |= 4<<3 ; break ;     //32��Ƶ:Fsck=Fpclk/16=2.25Mhz
		case SPI_SPEED_64  : SPI1->CR1 |= 5<<3 ; break ;     //64��Ƶ:Fsck=Fpclk/16=1.13Mhz
		case SPI_SPEED_128 : SPI1->CR1 |= 6<<3 ; break ;     //128��Ƶ:Fsck=Fpclk/16=561.5khz
		case SPI_SPEED_256 : SPI1->CR1 |= 7<<3 ; break ;     //256��Ƶ:Fsck=Fpclk/256=281.25Khz ����ģʽ
		default            : SPI1->CR1 |= 7<<3 ; break ;     //256��Ƶ:Fsck=Fpclk/256=281.25Khz ����ģʽ
	}

	SPI1->CR1 |= 1<<6 ;                                      //SPI�豸ʹ��	  
}

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_DMA_Configuration( void )
{

DMA_InitTypeDef  DMA_InitStructure;

//	//����ͨ��
//	DMA_DeInit(DMA1_Stream3);
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;//SPI1��DR�Ĵ�����ַ
//	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SPI1_TX_Buff;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  //�����ַ��Ŀ�ĵ�
//	DMA_InitStructure.DMA_BufferSize = 1;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //DMA�ڴ��ַ�Զ�����ģʽ
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;    //ѭ��ģʽ
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//���
//	DMA_Init(DMA1_Stream3, &DMA_InitStructure);

//	DMA_Cmd(DMA1_Stream3,ENABLE);

	//����ͨ��
	DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;//SPI2��DR�Ĵ�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SPI1_RX_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //�����ַ��Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //DMA�ڴ��ַ�Զ�����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;    //ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream4,ENABLE);
} 
//����DMA�洢����ַ��ע��MSIZE				 




/*----------------------------------------------------------------------
���ͺͽ��գ�����������Ϣ��<=512Byte����length����Ϣ���ȣ���λ���ֽڣ�
��Ҫ�������������Ļ���Ҫ����buffersize

------------------------------------------------------------------------*/
void SPI1_ReceiveSendByte(u8 *message,u16 length )
{
	u16 i=0;
 DMA_Cmd(DMA1_Stream3,DISABLE);
 DMA_Cmd(DMA1_Stream4,DISABLE);

	for(i=0;i<length;i++)					    //����Ҫ���͵�����ת�����ͻ�������
	{
		SPI1_TX_Buff[i] =message[i] ;	
	}

	DMA1_Stream3->NDTR = 0x0000   ;           //���������Ĵ�������
	DMA1_Stream3->NDTR = length ;             //������������Ϊbuffersize��

	DMA1_Stream4->NDTR = 0x0000   ;           //���������Ĵ�������
	DMA1_Stream4->NDTR = length ;             //������������Ϊbuffersize��

  DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3);      //��DMA������ɱ�־	DMA1->LIFCR = 0xF00 ;                        //���ͨ��3�ı�־λ
  DMA_ClearFlag(DMA1_Stream4,DMA_IT_TCIF4);
	
	SPI1->DR ;									//����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��

	while( ( SPI1->SR & 0x02 ) == 0 );
	
 DMA_Cmd(DMA1_Stream3,ENABLE);
 DMA_Cmd(DMA1_Stream4,ENABLE);

  delay_ms(10);
}