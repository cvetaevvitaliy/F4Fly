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
*********(������)*********|
SPI3_NSS  |  PA15 |  PA4  |
SPI3_SCK  |  PB3  |  PC10 |
SPI3_MISO |  PB3  |  PC11 |
SPI3_MOSI |  PB3  |  PC12 |
*********(������)*********/
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
 * **********(������)******** *
 * SPI3_NSS  |  PA15 |  PA4   *
 * SPI3_SCK  |  PB3  |  PC10  *
 * SPI3_MISO |  PB4  |  PC11  *
 * SPI3_MOSI |  PB5  |  PC12  *
 * *********(������)********* *
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
 * **********(������)**************** *
 * SPI3_NSS  |  PA15 |  PA4  |        *
 * SPI3_SCK  |  PB3  |  PC10 |        *
 * SPI3_MISO |  PB4  |  PC11 |        *
 * SPI3_MOSI |  PB5  |  PC12 |        *
 * *********(������)***************** *
\**********************************************/

/********************************************
***********************************************\
 * 2014/7/19 ���� wthѧ������ ����F407��֧��
 * 2013/7/24
 * 2013/7/26
\**********************************************/


/*
              SPI ģ�����ó�ʼ��
 */
#include "SPI.h"

#if EN_SPI0_==1

void SPI0_Init(void)
{
    SPI0_SCK_Init
    SPI0_MISO_Init
    SPI0_MOSI_Init
    SPI0_ReadWriteByte(0xff);                                            //��������
}
/*дһ�� byte ��оƬ��������һ�� byte*/
u8 SPI0_ReadWriteByte(u8 TxData)                                        //SPI��д���ݺ���
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

    /* SPI1 configuration */                                             //��ʼ��SPI�ṹ��
    SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ����ȫ˫��
    SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;                         //����SPIΪ��ģʽ
    SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //SPI���ͽ���8λ֡�ṹ
    SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ   �ߵ�ƽ�� �������ó� 2 ��һ����Ч��
    SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //��һ��ʱ���ؿ�ʼ��������
    SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //NSS�ź��������ʹ��SSIλ������
    SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //SPI������Ԥ��ƵֵΪ8
    SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ
    SPI1_InitStructure.SPI_CRCPolynomial = 7;                              //CRCֵ����Ķ���ʽ

    SPI_Init(SPI1, &SPI1_InitStructure);   //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI2�Ĵ���
    SPI_Cmd(SPI1, ENABLE);                                      //ʹ��SPI����

    SPI1_ReadWriteByte(0xff);                                            //��������
}
void SPI1_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI1, &SPI1_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}
u8 SPI1_ReadWriteByte(u8 TxData)                                        //SPI��д���ݺ���
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI1, TxData);                                    //ͨ������SPI1����һ������
    retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)   //���ջ����־λ��Ϊ��
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI1);                                 //ͨ��SPI1���ؽ�������
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


    SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;                         //����SPI����ģʽ:����Ϊ��SPI
    SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
    SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_InitStructure.SPI_CRCPolynomial = 7;                              //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI2_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

    SPI2_ReadWriteByte(0xff);//��������


}
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ

void SPI2_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI2, &SPI2_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
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

    SPI3_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI3_InitStructure.SPI_Mode = SPI_Mode_Master;                      //����SPI����ģʽ:����Ϊ��SPI
    SPI3_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI3_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI3_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI3_InitStructure.SPI_NSS = SPI_NSS_Soft;                          //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI3_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
    SPI3_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI3_InitStructure.SPI_CRCPolynomial = 7;                           //CRCֵ����Ķ���ʽ
    SPI_Init(SPI3, &SPI3_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����

    SPI3_ReadWriteByte(0xff);//��������


}
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ

void SPI3_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    SPI3_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
    SPI_Init(SPI3, &SPI3_InitStructure);
    SPI_Cmd(SPI3, ENABLE);
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if (retry > 200)return 0;
    }
    SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ������
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����
}
#endif
#endif

#if DEVICE==STM32F103
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01
//SPI�ڳ�ʼ��
//SPI1��ʼ��
#if EN_SPI1_==1
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ��
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,  ENABLE );//SPI2ʱ��ʹ��

    //����ֻ���SPI�ڳ�ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5.6.7��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB
    //PA5.6.7����
    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                       //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                               //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

    SPI1_ReadWriteByte(0xff);//��������
}
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI1->CR1 &= 0XFFC7;
    SPI1->CR1 |= SPI_BaudRatePrescaler; //����SPI2�ٶ�
    SPI_Cmd(SPI1, ENABLE);
}
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u16 retry = 0;
    while ((SPI1->SR & 1 << 1) == 0) //�ȴ���������
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //��ʱ�˳�
    }
    SPI1->DR = TxData;              //����һ��byte
    retry = 0;
    while ((SPI1->SR & 1 << 0) == 0) //�ȴ�������һ��byte
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //��ʱ�˳�
    }
    return SPI1->DR;                //�����յ�������
}
//u8 SPI1_ReadWriteByte(u8 TxData)
//{
//  u8 retry=0;
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//  SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
//  retry=0;

//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//  return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
//}

#endif
#if EN_SPI2_==1
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ��
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
    //PB13/14/15����
    GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;    //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;     //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

    //SPI2_ReadWriteByte(0xff);//��������
}
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ

void SPI2_SetSpeed(u16 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SPI_BaudRatePrescaler; //����SPI2�ٶ�
    SPI_Cmd(SPI2, ENABLE);
}

/*SPI2 ��дһ���ֽ�
TxData:Ҫд����ֽ�
����ֵ:��ȡ�����ֽ�*/
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u16 retry = 0;
    while ((SPI2->SR & 1 << 1) == 0) //�ȴ���������
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //��ʱ�˳�
    }
    SPI2->DR = TxData;              //����һ��byte
    retry = 0;
    while ((SPI2->SR & 1 << 0) == 0) //�ȴ�������һ��byte
    {
        retry++;
        if (retry >= 0XFFFE)return 0; //��ʱ�˳�
    }
    return SPI2->DR;                //�����յ�������
}
//u8 SPI2_ReadWriteByte(u8 dat)
//{
//      u8 retry=0;
//    /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//    /* ͨ�� SPI2����һ�ֽ����� */
//    SPI_I2S_SendData(SPI2, dat);
//    /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
//      {
//      retry++;
//      if(retry>200)return 0;
//      }
//    /* Return the byte read from the SPI bus */
//    return SPI_I2S_ReceiveData(SPI2);
//}
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ
#endif
/********************************************************************/
#endif
