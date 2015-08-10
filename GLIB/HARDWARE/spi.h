#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "GPIO.h"
/****************************************/
/***STM32F103vct6***/
/********************************************
***********************************************\
 * REMAP     |  0    |  1     *
 * SPI1_NSS  |  PA4  |  PA15  *
 * SPI1_SCK  |  PA5  |  PB3   *
 * SPI1_MISO |  PA6  |  PB4   *
 * SPI1_MOSI |  PA7  |  PB5   *
 * ************************** *
 * SPI2_NSS  |  PB12 |        *
 * SPI2_SCK  |  PB13 |        *
 * SPI2_MISO |  PB14 |        *
 * SPI2_MOSI |  PB15 |        *
 * ************************** *
 * **********(互联型)******** *
 * SPI3_NSS  |  PA15 |  PA4   *
 * SPI3_SCK  |  PB3  |  PC10  *
 * SPI3_MISO |  PB4  |  PC11  *
 * SPI3_MOSI |  PB5  |  PC12  *
 * *********(互联型)********* *
//2013/7/24 
\**********************************************/
/****************************************/
/***STM32F407***/
/********************************************
***********************************************\
 * REMAP     |  0    |  1    |  3     *
 * SPI1_NSS  |  PA4  |  PA15 |        *
 * SPI1_SCK  |  PA5  |  PB3  |        *
 * SPI1_MISO |  PA6  |  PB4  |        *
 * SPI1_MOSI |  PA7  |  PB5  |        *
 * ********************************** *
 * SPI2_NSS  |  PB12 |       |        *
 * SPI2_SCK  |  PB13 |  PB10 |  PI1   *
 * SPI2_MISO |  PB14 |  PC2  |  PI2   *
 * SPI2_MOSI |  PB15 |  PC3  |  PI3   *
 * ********************************** *
 * **********(互联型)**************** *
 * SPI3_NSS  |  PA15 |  PA4  |        *
 * SPI3_SCK  |  PB3  |  PC10 |        *
 * SPI3_MISO |  PB4  |  PC11 |        *
 * SPI3_MOSI |  PB5  |  PC12 |        *
 * *********(互联型)***************** *
\**********************************************/

/********************************************
***********************************************\
 * GFLY程序使用
 * 2014/7/19 换用 wth学长程序 增加F407的支持
 * 2013/7/24
 * 2013/7/26
\**********************************************/
//////////////////////////////////////////////////////////////////////////////////
#define DEVICE STM32F103
#define EN_SPI0_ (1) //Enable(1)/Disable(0)//SPI_SOFT
#define EN_SPI1_ (0) //Enable(1)/Disable(0)
#define EN_SPI2_ (1) //Enable(1)/Disable(0)
#define EN_SPI3_ (0) //Enable(1)/Disable(0)
#define SPI1_AFIO (0)
#define SPI2_AFIO (0)
#define SPI3_AFIO (0)

#if EN_SPI0_==1
/********************  软件SPI  ***************************************/
#define SPI0_SCK_Init   PA0_OUT
#define Set_SPI0_SCK    Set_A0
#define Clr_SPI0_SCK    Clr_A0
#define SPI0_MISO_Init  PC2_In
#define MISO_SPI0       C2
#define SPI0_MOSI_Init  PC3_OUT
#define Set_SPI0_MOSI   Set_C3
#define Clr_SPI0_MOSI   Clr_C3
/********************  硬件SPI  ***************************************/
#endif
#if EN_SPI1_==1
/*   SPI 1   */
#if SPI1_AFIO==0
#define SPI_1_SCK_PA5
#define SPI_1_MISO_PA6
#define SPI_1_MOSI_PA7
#elif SPI1_AFIO==1
#define SPI_1_SCK_PB3
#define SPI_1_MISO_PB4
#define SPI_1_MOSI_PB5
#else
#error SPI1_AFIO Error
#endif
#endif
#if EN_SPI2_==1
/*   SPI 2   */
#if SPI2_AFIO==3
#define SPI_2_SCK_PI1
#define SPI_2_MISO_PI2
#define SPI_2_MOSI_PI3
#elif SPI2_AFIO==0
#define SPI_2_SCK_PB13
#define SPI_2_MISO_PB14
#define SPI_2_MOSI_PB15
#elif SPI2_AFIO==1
#define SPI_2_SCK_PB10
#define SPI_2_MISO_PC2
#define SPI_2_MOSI_PC3
#else
#error SPI2_AFIO Error
#endif
#endif
#if EN_SPI3_==1
/*   SPI 3   */
#if SPI3_AFIO==0
#define SPI_3_SCK_PB3
#define SPI_3_MISO_PB4
#define SPI_3_MOSI_PB5
#elif SPI3_AFIO==1
#define SPI_3_SCK_PC10
#define SPI_3_MISO_PC11
#define SPI_3_MOSI_PC12
#else
#error SPI3_AFIO Error
#endif
#endif


#if EN_SPI0_==1
void SPI0_Init(void);
u8   SPI0_ReadWriteByte(u8 TxData);
#endif
#if EN_SPI1_==1
void SPI1_Init(void);            //初始化SPI口
void SPI1_SetSpeed(u16 SPI_BaudRatePrescaler);
u8   SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
#endif
#if EN_SPI2_==1
void SPI2_Init(void);
void SPI2_SetSpeed(u16 SPI_BaudRatePrescaler);
u8   SPI2_ReadWriteByte(u8 TxData);
#endif
#if EN_SPI3_==1
void SPI3_Init(void);
void SPI3_SetSpeed(u16 SPI_BaudRatePrescaler);
u8   SPI3_ReadWriteByte(u8 TxData);
#endif


#endif

// void SPI1_Init(void);            //初始化SPIx口
// void SPI1_SetSpeed(u8 SpeedSet); //设置SPIx速度
// u8   SPI1_ReadWriteByte(u8 TxData);//SPIx总线读写一个字节

// void SPI2_Init(void);            //初始化SPIx口
// void SPI2_SetSpeed(u8 SpeedSet); //设置SPIx速度
// u8   SPI2_ReadWriteByte(u8 TxData);//SPIx总线读写一个字节

// #endif

