/////////////////////////////////////////////////////////////////////////////////
/**************************
REMAP     |  0    |  1    |
SPI1_NSS  |  PA4  |  PA15 |
SPI1_SCK  |  PA5  |  PB3  |
SPI1_MISO |  PA6  |  PB4  |
SPI1_MOSI |  PA7  |  PB5  |
**************************|
SPI2_NSS  |  PB12 |       |
SPI2_SCK  |  PB13 |       |
SPI2_MISO |  PB14 |       |
SPI2_MOSI |  PB15 |       |
**************************|
*********(互联型)*********|
SPI3_NSS  |  PA15 |  PA4  |
SPI3_SCK  |  PB3  |  PC10 |
SPI3_MISO |  PB3  |  PC11 |
SPI3_MOSI |  PB3  |  PC12 |
*********(互联型)*********/
//2013/7/24
//2013/7/26
/*************************/
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
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
 * 2014/7/19 换用 wth学长程序 增加F407的支持
 * 2013/7/24
 * 2013/7/26
\**********************************************/


/*
              SPI 模块配置初始化
 */
#include "SPI.h"

#if EN_SPI0_==1

void SPI0_Init(void)
{
    SPI0_SCK_Init
    SPI0_MISO_Init
    SPI0_MOSI_Init
    SPI0_ReadWriteByte(0xff);                                            //启动传输
}
/*写一个 byte 到芯片，并返回一个 byte*/
u8 SPI0_ReadWriteByte(u8 TxData)                                        //SPI读写数据函数
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (TxData & 0x80)
        {
            Set_SPI0_MOSI
        }
        else
        {
            Clr_SPI0_MOSI
        }

        TxData = (TxData << 1);                    // shift next bit into MSB..
        Set_SPI0_SCK                             // Set SCK high..
        TxData |= MISO_SPI0;                     // capture current MISO bit
        Clr_SPI0_SCK                             // ..then set SCK low again
    }
    return (TxData);                             // return read UINT8
}
#endif
#if EN_SPI1_==1
static SPI_InitTypeDef  SPI1_InitStructure;
#if DEVICE==STM32F407
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

#ifdef SPI_1_SCK_PA5
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#ifdef SPI_1_MISO_PA6
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#ifdef SPI_1_MOSI_PA7
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#ifdef SPI_1_SCK_PB3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_1_MISO_PB4
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_1_MOSI_PB5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

    /* SPI1 configuration */                                             //初始化SPI结构体
    SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为两线全双工
    SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;                         //设置SPI为主模式
    SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //SPI发送接收8位帧结构
    SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //串行时钟在不操作时，时钟为低电平   高电平和 下面设置成 2 是一样的效果
    SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //第一个时钟沿开始采样数据
    SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //NSS信号由软件（使用SSI位）管理
    SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //SPI波特率预分频值为8
    SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //数据传输从MSB位开始
    SPI1_InitStructure.SPI_CRCPolynomial = 7;                              //CRC值计算的多项式

    SPI_Init(SPI1, &SPI1_InitStructure);   //根据SPI_InitStruct中指定的参数初始化外设SPI2寄存器
    SPI_Cmd(SPI1, ENABLE);                                      //使能SPI外设

    SPI1_ReadWriteByte(0xff);                                            //启动传输
}
void SPI1_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI1, &SPI1_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}
u8 SPI1_ReadWriteByte(u8 TxData)                                        //SPI读写数据函数
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //发送缓存标志位为空
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI1, TxData);                                    //通过外设SPI1发送一个数据
    retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)   //接收缓存标志位不为空
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI1);                                 //通过SPI1返回接收数据
}
#endif
#if EN_SPI2_==1
static SPI_InitTypeDef SPI2_InitStructure;

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
#ifdef SPI_2_SCK_PB13
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MISO_PB14
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MOSI_PB15
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_2_SCK_PI1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MISO_PI2
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MOSI_PI3
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
#endif
#ifdef SPI_2_SCK_PB10
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MISO_PC2
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#ifdef SPI_2_MOSI_PC3
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif


    SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;                         //设置SPI工作模式:设置为主SPI
    SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //串行同步时钟的空闲状态为高电平
    SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //定义波特率预分频的值:波特率预分频值为8
    SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_InitStructure.SPI_CRCPolynomial = 7;                              //CRC值计算的多项式
    SPI_Init(SPI2, &SPI2_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI2, ENABLE); //使能SPI外设

    SPI2_ReadWriteByte(0xff);//启动传输


}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频
//SPI_BaudRatePrescaler_8   8分频
//SPI_BaudRatePrescaler_16  16分频
//SPI_BaudRatePrescaler_256 256分频

void SPI2_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI2, &SPI2_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}
#endif
#if EN_SPI3_==1
static SPI_InitTypeDef  SPI3_InitStructure;

void SPI3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

#ifdef SPI_3_SCK_PC10
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#ifdef SPI_3_MISO_PC11
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#ifdef SPI_3_MOSI_PC12
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#ifdef SPI_3_SCK_PB3
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_3_MISO_PB4
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef SPI_3_MOSI_PB5
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

    SPI3_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI3_InitStructure.SPI_Mode = SPI_Mode_Master;                      //设置SPI工作模式:设置为主SPI
    SPI3_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI3_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //串行同步时钟的空闲状态为高电平
    SPI3_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI3_InitStructure.SPI_NSS = SPI_NSS_Soft;                          //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI3_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //定义波特率预分频的值:波特率预分频值为8
    SPI3_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI3_InitStructure.SPI_CRCPolynomial = 7;                           //CRC值计算的多项式
    SPI_Init(SPI3, &SPI3_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI3, ENABLE); //使能SPI外设

    SPI3_ReadWriteByte(0xff);//启动传输


}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频
//SPI_BaudRatePrescaler_8   8分频
//SPI_BaudRatePrescaler_16  16分频
//SPI_BaudRatePrescaler_256 256分频

void SPI3_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI3_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI3, &SPI3_InitStructure);
    SPI_Cmd(SPI3, ENABLE);
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据
}
#endif
#endif

#if DEVICE==STM32F103
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01
//SPI口初始化
//SPI1初始化
#if EN_SPI1_==1
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,  ENABLE );//SPI2时钟使能

    //这里只针对SPI口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5.6.7复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB
    //PA5.6.7上拉
    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                       //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                               //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI1, ENABLE); //使能SPI外设

    SPI1_ReadWriteByte(0xff);//启动传输
}
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI1->CR1 &= 0XFFC7;
    SPI1->CR1 |= SPI_BaudRatePrescaler; //设置SPI2速度
    SPI_Cmd(SPI1, ENABLE);
}
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u16 retry = 0;
    while ((SPI1->SR & 1 << 1) == 0) //等待发送区空
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //超时退出
    }
    SPI1->DR = TxData;              //发送一个byte
    retry = 0;
    while ((SPI1->SR & 1 << 0) == 0) //等待接收完一个byte
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //超时退出
    }
    return SPI1->DR;                //返回收到的数据
}
//u8 SPI1_ReadWriteByte(u8 TxData)
//{
//  u8 retry=0;
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//  SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
//  retry=0;

//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//  return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
//}

#endif
#if EN_SPI2_==1
//这里针是对SPI2的初始化
void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
    //PB13/14/15上拉
    GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;    //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;     //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI2, ENABLE); //使能SPI外设

    //SPI2_ReadWriteByte(0xff);//启动传输
}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频
//SPI_BaudRatePrescaler_8   8分频
//SPI_BaudRatePrescaler_16  16分频
//SPI_BaudRatePrescaler_256 256分频

void SPI2_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SPI_BaudRatePrescaler; //设置SPI2速度
    SPI_Cmd(SPI2, ENABLE);
}

/*SPI2 读写一个字节
TxData:要写入的字节
返回值:读取到的字节*/
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u16 retry = 0;
    while ((SPI2->SR & 1 << 1) == 0) //等待发送区空
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //超时退出
    }
    SPI2->DR = TxData;              //发送一个byte
    retry = 0;
    while ((SPI2->SR & 1 << 0) == 0) //等待接收完一个byte
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //超时退出
    }
    return SPI2->DR;                //返回收到的数据
}
//u8 SPI2_ReadWriteByte(u8 dat)
//{
//      u8 retry=0;
//    /* 当 SPI发送缓冲器非空时等待 */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//    /* 通过 SPI2发送一字节数据 */
//    SPI_I2S_SendData(SPI2, dat);
//    /* 当SPI接收缓冲器为空时等待 */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//    /* Return the byte read from the SPI bus */
//    return SPI_I2S_ReceiveData(SPI2);
//}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频
//SPI_BaudRatePrescaler_8   8分频
//SPI_BaudRatePrescaler_16  16分频
//SPI_BaudRatePrescaler_256 256分频
#endif
/********************************************************************/
#endif
