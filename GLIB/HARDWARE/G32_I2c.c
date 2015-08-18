#include "G32_I2c.h"
#include "G32_I2c_Config.h"
/*****************************************************
REMAP        |   00    |   01    |   10    |   11    |
*****************************************************|
Soft_I2cx2_SCL     |   PB10  |                       |
Soft_I2cx2_SDA     |   PB11  |                       |
*****************************************************|
Soft_I2cx1_SMBAI   |   PB5                           |
Soft_I2cx1_SCL     |   PB6   |  PB8                  |
Soft_I2cx1_SDA     |   PB7   |  PB9                  |
*****************************************************/
/*
软件模拟I2cx
2014/7/8
2013/7/19//Soft_I2cx_SDA_L;Soft_I2cx_SCL_L;
*/
#define	SDA_IN()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_IN<<14;}//sda输入
#define	SDA_OUT()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_OUT<<14;}//sda输出

//IO操作设置
#define	IIC_SDA0	GPIOB->BSRRH=GPIO_Pin_7;//sda=0
#define	IIC_SDA1	GPIOB->BSRRL=GPIO_Pin_7;//sda=1
#define IIC_SCL0	GPIOB->BSRRH=GPIO_Pin_6;
#define	IIC_SCL1	GPIOB->BSRRL=GPIO_Pin_6;
#define Soft_I2cx_SCL_H Soft_I2cx_SCL_GPIO->BSRRL = Soft_I2cx_SCL_PIN//GPIO_SetBits(Soft_I2cx_SCL_GPIO,Soft_I2cx_SCL_PIN)
#define Soft_I2cx_SCL_L Soft_I2cx_SCL_GPIO->BSRRH = Soft_I2cx_SCL_PIN//GPIO_ResetBits(Soft_I2cx_SCL_GPIO,Soft_I2cx_SCL_PIN)

#define Soft_I2cx_SDA_H Soft_I2cx_SDA_GPIO->BSRRL = Soft_I2cx_SDA_PIN//GPIO_SetBits(Soft_I2cx_SDA_GPIO,Soft_I2cx_SDA_PIN)
#define Soft_I2cx_SDA_L Soft_I2cx_SDA_GPIO->BSRRH = Soft_I2cx_SDA_PIN//GPIO_ResetBits(Soft_I2cx_SDA_GPIO,Soft_I2cx_SDA_PIN)

#define Soft_I2cx_SCL_Read ((Soft_I2cx_SCL_GPIO->IDR&(Soft_I2cx_SCL_PIN))?1:0)//GPIO_ReadInputDataBit(Soft_I2cx_SCL_GPIO,Soft_I2cx_SCL_PIN)
#define Soft_I2cx_SDA_Read ((Soft_I2cx_SDA_GPIO->IDR&(Soft_I2cx_SDA_PIN))?1:0)//GPIO_ReadInputDataBit(Soft_I2cx_SDA_GPIO,Soft_I2cx_SDA_PIN)

/*****************************************************/
#define RCC_Soft_I2cx_SCL RCC_Soft_I2c1_SCL
#define Soft_I2cx_SCL_GPIO    Soft_I2c1_SCL_GPIO
#define Soft_I2cx_SCL_PIN     Soft_I2c1_SCL_PIN
#define RCC_Soft_I2cx_SDA RCC_Soft_I2c1_SDA
#define Soft_I2cx_SDA_GPIO    Soft_I2c1_SDA_GPIO
#define Soft_I2cx_SDA_PIN     Soft_I2c1_SDA_PIN
#define Soft_I2cx_Init        Soft_I2c1_Init
#define Soft_I2cx_Delay       Soft_I2c1_Delay
#define Soft_I2cx_Start       Soft_I2c1_Start
#define Soft_I2cx_Stop        Soft_I2c1_Stop
#define Soft_I2cx_Ack         Soft_I2c1_Ack
#define Soft_I2cx_NoAck       Soft_I2c1_NoAck
#define Soft_I2cx_WaitAck     Soft_I2c1_WaitAck
#define Soft_I2cx_SendBety    Soft_I2c1_SendBety
#define Soft_I2cx_ReceiveBety Soft_I2c1_ReceiveBety
#define Soft_I2cx_Write       Soft_I2c1_Write
#define Soft_I2cx_WriteBuffer Soft_I2c1_WriteBuffer
#define Soft_I2cx_Read        Soft_I2c1_Read
#define Soft_I2cx_ReadBuffer  Soft_I2c1_ReadBuffer
/*****************************************************/
void Soft_I2cx_Init(void)
{
//    GPIO_InitTypeDef Soft_I2cx_GPIO;
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能Px端口时钟
//    Soft_I2cx_GPIO.GPIO_Pin = Soft_I2cx_SCL_PIN|Soft_I2cx_SDA_PIN;
//    Soft_I2cx_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
//    Soft_I2cx_GPIO.GPIO_Mode = GPIO_Mode_OUT;
//	  Soft_I2cx_GPIO.GPIO_OType=GPIO_OType_PP;
//	  Soft_I2cx_GPIO.GPIO_PuPd=GPIO_PuPd_UP;
//    GPIO_Init(GPIOB, &Soft_I2cx_GPIO);
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;//SDA,SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	IIC_SDA1;
	IIC_SCL1;

}

static void Soft_I2cx_Delay(void)
{
    volatile s8 i = 9;
    while (i)
        i--;
}
static bool Soft_I2cx_Start(void)
{
	SDA_OUT(); //SDA线输出
	IIC_SDA1;
	IIC_SCL1;
	Soft_I2cx_Delay();
	IIC_SDA0;
	Soft_I2cx_Delay();
	IIC_SCL0; 
	return 1;
}
static void Soft_I2cx_Stop(void)
{
	SDA_OUT();
	IIC_SDA0;
	IIC_SCL0;
	Soft_I2cx_Delay();
	IIC_SCL1;
	Soft_I2cx_Delay();
	IIC_SDA1;
	Soft_I2cx_Delay();
	IIC_SCL0;
}
static void Soft_I2cx_Ack(void)
{
	IIC_SCL0;
	SDA_OUT();
	IIC_SDA0;
	Soft_I2cx_Delay();
	IIC_SCL1;
	Soft_I2cx_Delay();
	IIC_SCL0;
}
static void Soft_I2cx_NoAck(void)
{
	IIC_SCL0;
	SDA_OUT();
	IIC_SDA1;
	Soft_I2cx_Delay();
	IIC_SCL1;
	Soft_I2cx_Delay();
	IIC_SCL0;
}
static bool Soft_I2cx_WaitAck(void)
{
	u8	i;
	SDA_IN();
	IIC_SCL0;
	Soft_I2cx_Delay();
	IIC_SCL1;
	Soft_I2cx_Delay();
    while (Soft_I2cx_SDA_Read)
    {		i++;
		if(i>250)
	  {
        Soft_I2cx_Stop();
        return false;
		}
    }
  IIC_SCL0;
	Soft_I2cx_Delay();
    return true;
}
static void Soft_I2cx_SendBety(u8 byte)
{
    u8 i = 8;
		SDA_OUT();
	  Soft_I2cx_Delay();
    while (i--)
    {
        IIC_SCL0;
        if (byte & 0x80)
				{ IIC_SDA1;}
        else
        {    IIC_SDA0;}
					Soft_I2cx_Delay();
        byte <<= 1;
				IIC_SCL1;
        Soft_I2cx_Delay();
    }
   	IIC_SCL0;
	  Soft_I2cx_Delay();
}
static u8 Soft_I2cx_ReceiveBety(void)
{
    u8 i = 8;
    u8 byte = 0;
		SDA_IN();//SDA设为输入
		IIC_SDA0;
		Soft_I2cx_Delay();
    while (i--)
    {
			  IIC_SCL0;
		    Soft_I2cx_Delay();
	    	IIC_SCL1;
        byte <<= 1;
        Soft_I2cx_Delay();
        if (Soft_I2cx_SDA_Read)
            byte++;
				Soft_I2cx_Delay();
    }
    IIC_SCL0;
    return byte;
}
void WriteCmd (u8 byte)
{
  Soft_I2cx_Write(0x78,0x00,byte);
}
void I2C_WriteByte(uint8_t data)
{
	Soft_I2cx_Write(0x78,0x40,data);
}
/******************************************************************************
/ 函数功能:单字节写入
/ 输入参数:
/   @arg SlaveAddress   从器件地址
/   @arg REG_Address    寄存器地址
/   @arg data       欲写入的字节数据
/ 输出参数: 读出的字节数据
/ 使用说明:这时一个完整的单字节读取函数
******************************************************************************/
bool Soft_I2cx_Write(u8 SlaveAddress, u8 REG_Address, u8 data)
{
    if (!Soft_I2cx_Start())
        return false;
    Soft_I2cx_SendBety(SlaveAddress);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    Soft_I2cx_SendBety(REG_Address);
    Soft_I2cx_WaitAck();
    Soft_I2cx_SendBety(data);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    Soft_I2cx_Stop();
    return true;
}
bool Soft_I2cx_WriteBuffer(u8 addr, u8 reg, u8 len , u8 *data)
{
    s8 i;
    if (!Soft_I2cx_Start())
        return false;
    Soft_I2cx_SendBety(addr);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    Soft_I2cx_SendBety(reg);
    Soft_I2cx_WaitAck();
    for (i = 0; i < len; i++)
    {
        Soft_I2cx_SendBety(data[i]);
        if (!Soft_I2cx_WaitAck())
        {
            Soft_I2cx_Stop();
            return false;
        }
    }
    Soft_I2cx_Stop();
    return true;
}
uint8_t Soft_I2cx_Read(u8 addr, u8 reg)
{
    uint8_t REG_data;
    if (!Soft_I2cx_Start())return false;
    Soft_I2cx_SendBety(addr); //Soft_I2cx_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//'Eè"O~A,ssAEd^E 1/4 uO"O·+AE÷ 1/4 thuO"O·
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    Soft_I2cx_SendBety(reg);
    Soft_I2cx_WaitAck();
    Soft_I2cx_Start();
    Soft_I2cx_SendBety(addr + 1);
    Soft_I2cx_WaitAck();

    REG_data = Soft_I2cx_ReceiveBety();
    Soft_I2cx_NoAck();
    Soft_I2cx_Stop();
    return REG_data;
}
bool Soft_I2cx_ReadBuffer(u8 addr, u8 reg, u8 len , u8 *data)
{
    if (!Soft_I2cx_Start())
        return false;
    Soft_I2cx_SendBety(addr);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    Soft_I2cx_SendBety(reg);
    Soft_I2cx_WaitAck();

    if (!Soft_I2cx_Start())return false;
    Soft_I2cx_SendBety(addr + 1);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return false;
    }
    while (len--)
    {
        *data++ = Soft_I2cx_ReceiveBety();
        if (len < 1)
            Soft_I2cx_NoAck();
        else
            Soft_I2cx_Ack();
    }
    Soft_I2cx_Stop();
    return true;
}
/***MUP6050 Soft_I2cx***/
int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    s8 i;
    if (!Soft_I2cx_Start())
        return FALSE;
    Soft_I2cx_SendBety(addr << 1 | 0x00);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return FALSE;
    }
    Soft_I2cx_SendBety(reg);
    Soft_I2cx_WaitAck();
    for (i = 0; i < len; i++)
    {
        Soft_I2cx_SendBety(data[i]);
        if (!Soft_I2cx_WaitAck())
        {
            Soft_I2cx_Stop();
            return FALSE;
        }
    }
    Soft_I2cx_Stop();
    return TRUE;
}
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    if (!Soft_I2cx_Start())
        return FALSE;
    Soft_I2cx_SendBety(addr << 1 | 0x00);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return FALSE;
    }
    Soft_I2cx_SendBety(reg);
    Soft_I2cx_WaitAck();

    if (!Soft_I2cx_Start())return FALSE;
    Soft_I2cx_SendBety(addr << 1 | 0x01);
    if (!Soft_I2cx_WaitAck())
    {
        Soft_I2cx_Stop();
        return FALSE;
    }
    while (len)
    {
        *data = Soft_I2cx_ReceiveBety();
        if (len == 1)
            Soft_I2cx_NoAck();
        else
            Soft_I2cx_Ack();
        data++;
        len--;
    }
    Soft_I2cx_Stop();
    return TRUE;
}
/***MUP6050 Soft_I2cx***/
/*****************************************************/
#undef RCC_Soft_I2cx_SCL
#undef Soft_I2cx_SCL_GPIO
#undef Soft_I2cx_SCL_PIN
#undef RCC_Soft_I2cx_SDA
#undef Soft_I2cx_SDA_GPIO
#undef Soft_I2cx_SDA_PIN
#undef Soft_I2cx_Init
#undef Soft_I2cx_Delay
#undef Soft_I2cx_Start
#undef Soft_I2cx_Stop
#undef Soft_I2cx_Ack
#undef Soft_I2cx_NoAck
#undef Soft_I2cx_WaitAck
#undef Soft_I2cx_SendBety
#undef Soft_I2cx_ReceiveBety
#undef Soft_I2cx_Write
#undef Soft_I2cx_WriteBuffer
#undef Soft_I2cx_Read
#undef Soft_I2cx_ReadBuffer
