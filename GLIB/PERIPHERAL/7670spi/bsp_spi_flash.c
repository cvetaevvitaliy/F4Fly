 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi flash µ×²ãÓ¦ÓÃº¯Êýbsp 
  ******************************************************************************
  * @attention
  *
  * ÊµÑéÆ½Ì¨:Ò°»ð ISO-MINI STM32 ¿ª·¢°å 
  * ÂÛÌ³    :http://www.chuxue123.com
  * ÌÔ±¦    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "sys_usart.h"
#include "usr_usart.h"
#include "minos_delay.h"
#include "bsp_spi_flash.h"/*******************************************************************************
* Module        : SPI1_DMA_Configuration
* Description   : SPI1ÒÔ¼°¶ÔÓ¦DMAÍ¨µÀÇý¶¯º¯Êý
* Compile       : Keil MDK 4.10
* Device        £ºSTM32F103RBT6
* Author        : ÁÖ°¢´ô   (ÐÞ¸Ä2012/4/30)
* Modefied data : 2012/3/8 
* Version       £ºV1.0
* Copyright(C) ÁÖ°¢´ô 20012-2022	 
* All rights reserved
*******************************************************************************/



vu8 SPI1_RX_Buff[buffersize] = { 0 };    //½ÓÊÕ»º³åÇø³õÊ¼»¯       
vu8 SPI1_TX_Buff[buffersize] = {0x55 };	 //·¢ËÍ»º³åÇø³õÊ¼»¯
//Ô­°æµÄ·¢ËÍµÄ¶¨ÒåºÍ³õÊ¼»¯ÔÚ½ÓÊÕµÄÇ°Ãæ£¬Ê¹µÃÖ÷»úÔÚ450£¨Ô¼£©ÒÔÉÏµÄ´«ÊäÊ±³öÏÖ½ÓÊÕ²»µ½»òÕß½Ó
//ÊÕ³ö´íµÄÎÊÌâ£¬²»ÖªµÀºóÀ´¸ÄÁË»º³åÇø´óÐ¡Ö®ºó»á²»»á³öÏÖÍ¬ÖÖÇé¿ö£¬·´Õý½«Æäµ÷»»Ö®ºó£¬ÔÚÔö´ó»º
//³åÇø¾ÍÃ»ÎÊÌâÁË
//vu8=volatile unsigned char,volatileÀàÐÍÐÞÊÎ·ûÊÇ±»Éè¼ÆÓÃÀ´ÐÞÊÎ±»²»Í¬Ïß³Ì·ÃÎÊºÍÐÞ¸ÄµÄ±äÁ¿
							  
/*******************************************************************************
* Function Name  : SPI1_Configuration
* Description    : SPIÄ£¿é°üÀ¨Ïà¹ØIO¿ÚµÄÅäÖÃ£¬SPI1ÅäÖÃÎªµÍËÙ£¬¼´Fsck=Fcpu/256
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_Configuration( void )
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOAÊ±ÖÓ
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//Ê¹ÄÜSPI1Ê±ÖÓ

  //GPIOFB3,4,5³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PB3~5¸´ÓÃ¹¦ÄÜÊä³ö	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3¸´ÓÃÎª SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4¸´ÓÃÎª SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5¸´ÓÃÎª SPI1
	
	/* ÅäÖÃSPIÓ²¼þ²ÎÊý */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* Êý¾Ý·½Ïò£º2ÏßÈ«Ë«¹¤ */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32µÄSPI¹¤×÷Ä£Ê½ £ºÖ÷»úÄ£Ê½ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* Êý¾ÝÎ»³¤¶È £º 8Î» */
	/* SPI_CPOLºÍSPI_CPHA½áºÏÊ¹ÓÃ¾ö¶¨Ê±ÖÓºÍÊý¾Ý²ÉÑùµãµÄÏàÎ»¹ØÏµ¡¢
	   ±¾ÀýÅäÖÃ: ×ÜÏß¿ÕÏÐÊÇ¸ßµçÆ½,µÚ2¸ö±ßÑØ£¨ÉÏÉýÑØ²ÉÑùÊý¾Ý)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			/* Ê±ÖÓÉÏÉýÑØ²ÉÑùÊý¾Ý */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* Ê±ÖÓµÄµÚ2¸ö±ßÑØ²ÉÑùÊý¾Ý */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Æ¬Ñ¡¿ØÖÆ·½Ê½£ºÈí¼þ¿ØÖÆ */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	/* ²¨ÌØÂÊÔ¤·ÖÆµÏµÊý£º4·ÖÆµ */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* Êý¾ÝÎ»´«Êä´ÎÐò£º¸ßÎ»ÏÈ´« */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC¶àÏîÊ½¼Ä´æÆ÷£¬¸´Î»ºóÎª7¡£±¾Àý³Ì²»ÓÃ */
	SPI_Init(SPI1, &SPI_InitStructure);

    // Enable DMA1 Channel3 //
    DMA_Cmd(DMA1_Stream4, ENABLE);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	SPI_Cmd(SPI1, ENABLE);		/* Ê¹ÄÜSPI  */
    

} 

  
/*******************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : SPIÉèÖÃËÙ¶ÈÎª¸ßËÙ
* Input          : u8 SpeedSet 
*                  Èç¹ûËÙ¶ÈÉèÖÃÊäÈë0£¬ÔòµÍËÙÄ£Ê½£¬·Ç0Ôò¸ßËÙÄ£Ê½
*                  SPI_SPEED_HIGH   1
*                  SPI_SPEED_LOW    0
¡Á                  ×¢ÒâÉèÖÃËÙ¶È»áÏÈ¹Ø±ÕSPI£¬ËùÒÔ×îºÃÔÚ³ÌÐò·¢ËÍ½ÓÊÕÍêºó¸Ä
*                  Ò»°ã½ÓÊÕ·¢ËÍÊý¾Ý¶¼ÓÐµÈ´ý½ÓÊÕÍê³É£¬ËùÒÔÕâÀï²»¼Ó
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_SetSpeed( u8 SpeedSet )
{	
	SPI1->CR1 &= 0XFF87 ;                                    //¹Ø±ÕSPI1,Fsck=Fcpu/2

	switch( SpeedSet )
	{
	    case SPI_SPEED_2   : SPI1->CR1 |= 0<<3 ; break ;     //2·ÖÆµ:Fsck=Fpclk/2=36Mhz;
		case SPI_SPEED_4   : SPI1->CR1 |= 1<<3 ; break ;     //4·ÖÆµ:Fsck=Fpclk/4=18Mhz;  
		case SPI_SPEED_8   : SPI1->CR1 |= 2<<3 ; break ;     //8·ÖÆµ:Fsck=Fpclk/8=9Mhz
		case SPI_SPEED_16  : SPI1->CR1 |= 3<<3 ; break ;     //16·ÖÆµ:Fsck=Fpclk/16=4.5Mhz
		case SPI_SPEED_32  : SPI1->CR1 |= 4<<3 ; break ;     //32·ÖÆµ:Fsck=Fpclk/16=2.25Mhz
		case SPI_SPEED_64  : SPI1->CR1 |= 5<<3 ; break ;     //64·ÖÆµ:Fsck=Fpclk/16=1.13Mhz
		case SPI_SPEED_128 : SPI1->CR1 |= 6<<3 ; break ;     //128·ÖÆµ:Fsck=Fpclk/16=561.5khz
		case SPI_SPEED_256 : SPI1->CR1 |= 7<<3 ; break ;     //256·ÖÆµ:Fsck=Fpclk/256=281.25Khz µÍËÙÄ£Ê½
		default            : SPI1->CR1 |= 7<<3 ; break ;     //256·ÖÆµ:Fsck=Fpclk/256=281.25Khz µÍËÙÄ£Ê½
	}

	SPI1->CR1 |= 1<<6 ;                                      //SPIÉè±¸Ê¹ÄÜ	  
}

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ÅäÖÃSPI1_RXµÄDMAÍ¨µÀ2£¬SPI1_TXµÄDMAÍ¨µÀ3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_DMA_Configuration( void )
{

DMA_InitTypeDef  DMA_InitStructure;

//	//·¢ËÍÍ¨µÀ
//	DMA_DeInit(DMA1_Stream3);
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;//SPI1µÄDR¼Ä´æÆ÷µØÖ·
//	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SPI1_TX_Buff;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  //ÍâÉèµØÖ·ÊÇÄ¿µÄµØ
//	DMA_InitStructure.DMA_BufferSize = 1;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //DMAÄÚ´æµØÖ·×Ô¶¯Ôö¼ÓÄ£Ê½
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;    //Ñ­»·Ä£Ê½
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//´æ´¢Æ÷Í»·¢µ¥´Î´«Êä
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//ÍâÉ
//	DMA_Init(DMA1_Stream3, &DMA_InitStructure);

//	DMA_Cmd(DMA1_Stream3,ENABLE);

	//½ÓÊÕÍ¨µÀ
	DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;//SPI2µÄDR¼Ä´æÆ÷µØÖ·
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SPI1_RX_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //ÍâÉèµØÖ·ÊÇÄ¿µÄµØ
	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //DMAÄÚ´æµØÖ·×Ô¶¯Ôö¼ÓÄ£Ê½
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;    //Ñ­»·Ä£Ê½
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream4,ENABLE);
} 
//ÉèÖÃDMA´æ´¢Æ÷µØÖ·£¬×¢ÒâMSIZE				 




/*----------------------------------------------------------------------
·¢ËÍºÍ½ÓÊÕ£¬ÈÎÒâÊýÁ¿ÐÅÏ¢£¨<=512Byte£©£¬lengthÊÇÐÅÏ¢³¤¶È£¬µ¥Î»ÊÇ×Ö½Ú£¬
ÈôÒªÔö´ó´«ÊäÊý¾ÝÁ¿µÄ»°£¬ÒªÉèÖÃbuffersize

------------------------------------------------------------------------*/
void SPI1_ReceiveSendByte(u8 *message,u16 length )
{
	u16 i=0;
 DMA_Cmd(DMA1_Stream3,DISABLE);
 DMA_Cmd(DMA1_Stream4,DISABLE);

	for(i=0;i<length;i++)					    //½«ËùÒª·¢ËÍµÄÊý¾Ý×ªµ½·¢ËÍ»º³åÇøÉÏ
	{
		SPI1_TX_Buff[i] =message[i] ;	
	}

	DMA1_Stream3->NDTR = 0x0000   ;           //´«ÊäÊýÁ¿¼Ä´æÆ÷ÇåÁã
	DMA1_Stream3->NDTR = length ;             //´«ÊäÊýÁ¿ÉèÖÃÎªbuffersize¸ö

	DMA1_Stream4->NDTR = 0x0000   ;           //´«ÊäÊýÁ¿¼Ä´æÆ÷ÇåÁã
	DMA1_Stream4->NDTR = length ;             //´«ÊäÊýÁ¿ÉèÖÃÎªbuffersize¸ö

  DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3);      //ÇåDMA·¢ËÍÍê³É±êÖ¾	DMA1->LIFCR = 0xF00 ;                        //Çå³ýÍ¨µÀ3µÄ±êÖ¾Î»
  DMA_ClearFlag(DMA1_Stream4,DMA_IT_TCIF4);
	
	SPI1->DR ;									//½ÓËÍÇ°¶ÁÒ»´ÎSPI1->DR£¬±£Ö¤½ÓÊÕ»º³åÇøÎª¿Õ

	while( ( SPI1->SR & 0x02 ) == 0 );
	
 DMA_Cmd(DMA1_Stream3,ENABLE);
 DMA_Cmd(DMA1_Stream4,ENABLE);

  delay_ms(10);
}