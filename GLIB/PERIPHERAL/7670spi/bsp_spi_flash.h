#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f4xx.h"

#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_4                  /* PA.04 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)


void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);



	
// SPI�����ٶ�����*/
#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  3
#define SPI_SPEED_32  4
#define SPI_SPEED_64  5
#define SPI_SPEED_128 6
#define SPI_SPEED_256 7 //281K

#define TIMEOUT  255
#define ERROR    200


#define buffersize 10			//��������С����������һ����Ҫ����512�ֽڵĻ����ӻ�ֻ������512�ֽ�		 		
extern vu8 SPI1_TX_Buff[buffersize] ;		//���ͻ�����
extern vu8 SPI1_RX_Buff[buffersize] ;		//���ջ�����

#define SPI1_DR_Addr ( (u32)0x4001300C )	//SPI1���ݼĴ�����ַ


/*-------------------------------------------------------------------------------------------------
��������

--------------------------------------------------------------------------------------------------*/	  		    													  
void SPI1_Configuration( void );         //SPI��ʼ��
void SPI2_Configuration( void );
void SPI1_SetSpeed( u8 SpeedSet );		 //SPI�ٶ�����
void SPI2_SetSpeed( u8 SpeedSet );
void SPI1_DMA_Configuration( void );	 //DMA��ʼ��
void SPI2_DMA_Configuration( void );
u8 SPI1_RXonly( void );					  //ֻ���գ�û�Թ�,��ӱʦ��ԭ�����
u8 SPI1_TXonly( void );					  //ֻ���ͣ�û�Թ�����ӱʦ��ԭ�����
void SPI1_ReceiveSendByte(u8 *message,u16 length );//���ͺͽ��գ�message������ָ�룬length�����鳤��
		 
#endif


