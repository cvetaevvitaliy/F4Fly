#include "PX4FLOW.h"
#include "G32_I2c.h"

#include "usr_usart.h"
#include "data_transfer.h"

#include "MPU6050.h"
#include "math.h"
/*** Ӳ�� ***/
//#define I2c_Write      HI2c_Write
//#define I2c_ReadBuffer HI2c_ReadBuffer
//#define I2c_Init       HI2c1_Init
/*** ��� ***/
#define I2c_Write      Soft_I2c1_Write
#define I2c_ReadBuffer Soft_I2c1_ReadBuffer
#define I2c_Init       Soft_I2c1_Init
/*** ��� ***/
#define NI 3;
#define I2C_FLOW_ADDRESS 		0x84	///< 7-bit address. 8-bit address is 0x84, range 0x42 - 0x49
/* PX4FLOW Registers addresses */
#define PX4FLOW_REG			0x00	///< Measure Register 22
int X,Y;
u16 I=0,K,J,T=0;
u16 aid[3];
u16 Alt_t;
float sum_x=0,sum_y=0;
int v_x,v_y;//extern float sum_x,sum_y;
typedef struct
{
    uint16_t frame_count;
    int16_t pixel_flow_x_sum;
    int16_t pixel_flow_y_sum;
    int16_t flow_comp_m_x;
    int16_t flow_comp_m_y;
    int16_t qual;
    int16_t gyro_x_rate;
    int16_t gyro_y_rate;
    int16_t gyro_z_rate;
    uint8_t gyro_range;
    uint8_t sonar_timestamp;
    int16_t ground_distance;
}FLOW;

FLOW flow;
float ByteToFloat(unsigned char* byteArry)
{
  return *((float*)byteArry);
}
u8  PX4FLOW_buffer[24];     //iic��ȡ��������
u8 FLOW_STATE[4];
u8 flow_buf[22];

///**************************ʵ�ֺ���********************************************
////��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���
//*******************************************************************************/
//#define     MPU6050_MAX     32767
//#define     MPU6050_MIN     -32768
void PX4FLOW_Dataanl(void)
{extern u16 Alt_ultrasonic;
	 extern S_INT16_XYZ Mag;//extern S_INT16_XYZ Mag
    flow.frame_count      = (int16_t)((flow_buf[0])|(flow_buf[1]<<8)) ;
    flow. pixel_flow_x_sum= (int16_t)((flow_buf[2])|(flow_buf[3]<<8)) ;//����ƽ���ٶ�
    flow. pixel_flow_y_sum= (int16_t)((flow_buf[4])|(flow_buf[5]<<8)) ;
    flow. flow_comp_m_x   = (int16_t)((flow_buf[6])|(flow_buf[7]<<8)) ;//�߶Ȳ������ٶ�
    flow. flow_comp_m_y   = (int16_t)((flow_buf[8])|(flow_buf[9]<<8));
	flow.gyro_x_rate =(int16_t)((flow_buf[12])|(flow_buf[13]<<8)) ;
	flow.gyro_y_rate =(int16_t)((flow_buf[14])|(flow_buf[15]<<8)) ;
	flow.gyro_z_rate =(int16_t)((flow_buf[16])|(flow_buf[17]<<8)) ;
    flow.ground_distance  = (int16_t)((flow_buf[20])|(flow_buf[21]<<8));//�������߶�
//	if(flow.ground_distance<200)flow. flow_comp_m_x=flow. flow_comp_m_y=0;
	  sum_x=sum_x+((float)flow. flow_comp_m_x*0.000224);//ƽ���ۼ�
    sum_y=sum_y+((float)flow. flow_comp_m_y*0.000224);//ƽ���ۼ�
//	  Mag.x=sum_x;
//	  Mag.y=sum_y;
//	Sys_Printf(Printf_USART, "m_x:%lf\n",sum_x);//�ٶ�
//	Sys_Printf(Printf_USART, "v_x:%lf\n\n",sum_y);
	
		aid[I] =flow.ground_distance;if(aid[I]!=0&&aid[I]<2500)I++;//������2.5m
    if(I==5)//��������ȡ�м�ֵ
{I=0;
  for (J=0;J<5;J++)
{
 for (K=0;K<5;K++)
{
if(aid[K]>aid[K+1])
{
Alt_t= aid[K];
aid[K] = aid[K+1];
aid[K+1] =Alt_t;
}}}
Alt_ultrasonic=aid[2];}
	
	
	
	
	
	
	
//	
//	Sys_Printf(Printf_USART, "frame:%d\n",flow.frame_count);
//	Sys_Printf(Printf_USART, "x_sum:%d\n",flow. pixel_flow_x_sum);//λ��
//	Sys_Printf(Printf_USART, "y_sum:%d\n",flow. pixel_flow_y_sum);
//	Sys_Printf(Printf_USART, "m_x:%d\n",flow. flow_comp_m_x);//�ٶ�
//	Sys_Printf(Printf_USART, "m_y:%d\n",flow. flow_comp_m_y);
//	Sys_Printf(Printf_USART, "qual:%d\n",flow.qual);
//	Sys_Printf(Printf_USART, "gyro_x:%d\n",flow.gyro_x_rate);
//	Sys_Printf(Printf_USART, "gyro_y:%d\n",flow.gyro_y_rate);
//	Sys_Printf(Printf_USART, "gyro_z:%d\n",flow.gyro_z_rate);
//	Sys_Printf(Printf_USART, "gyro_range:%d\n",flow.gyro_range);
//	Sys_Printf(Printf_USART, "timestamp:%d\n",flow.sonar_timestamp);
//	Sys_Printf(Printf_USART, "distance:%d\n\n",Alt_ultrasonic);

}

//*******************************************************************************/
void PX4FLOW_Read(void)
{
    I2c_ReadBuffer(I2C_FLOW_ADDRESS, PX4FLOW_REG,22, flow_buf);
}
///**************************ʵ�ֺ���********************************************
//*����ԭ��:      u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
//*��������:      �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
//����  dev  Ŀ���豸��ַ
//reg    �Ĵ�����ַ
//bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
//data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
//����   �ɹ� Ϊ1
//ʧ��Ϊ0
//*******************************************************************************/
//void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
//{
//    u8 b;
//    I2c_ReadBuffer(dev, reg, 1, &b);
//    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//    I2c_Write(dev, reg, b);
//}
///**************************ʵ�ֺ���********************************************
//*����ԭ��:      u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//*��������:      �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
//����  dev  Ŀ���豸��ַ
//reg    �Ĵ�����ַ
//bitStart  Ŀ���ֽڵ���ʼλ
//length   λ����
//data    ��Ÿı�Ŀ���ֽ�λ��ֵ
//����   �ɹ� Ϊ1
//ʧ��Ϊ0
//*******************************************************************************/
//void IICwriteBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
//{

//    u8 b, mask;
//    I2c_ReadBuffer(dev, reg, 1, &b);
//    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
//    data <<= (8 - length);
//    data >>= (7 - bitStart);
//    b &= mask;
//    b |= data;
//    I2c_Write(dev, reg, b);
//}
///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_setClockSource(uint8_t source)
//*��������:      ����  MPU6050 ��ʱ��Դ
//* CLK_SEL | Clock Source
//* --------+--------------------------------------
//* 0       | Internal oscillator
//* 1       | PLL with X Gyro reference
//* 2       | PLL with Y Gyro reference
//* 3       | PLL with Z Gyro reference
//* 4       | PLL with external 32.768kHz reference
//* 5       | PLL with external 19.2MHz reference
//* 6       | Reserved
//* 7       | Stops the clock and keeps the timing generator in reset
//*******************************************************************************/
//void MPU6050_setClockSource(uint8_t source)
//{
//    IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);

//}
///** Set full-scale gyroscope range.
//* @param range New full-scale gyroscope range value
//* @see getFullScaleRange()
//* @see MPU6050_GYRO_FS_250
//* @see MPU6050_RA_GYRO_CONFIG
//* @see MPU6050_GCONFIG_FS_SEL_BIT
//* @see MPU6050_GCONFIG_FS_SEL_LENGTH
//*/
//void MPU6050_setFullScaleGyroRange(uint8_t range)
//{
//    IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
//}

///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_setFullScaleAccelRange(uint8_t range)
//*��������:      ����  MPU6050 ���ٶȼƵ��������
//*******************************************************************************/
//void MPU6050_setFullScaleAccelRange(uint8_t range)
//{
//    IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
//}
///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_setSleepEnabled(uint8_t enabled)
//*��������:      ����  MPU6050 �Ƿ����˯��ģʽ
//enabled =1   ˯��
//enabled =0   ����
//*******************************************************************************/
//void MPU6050_setSleepEnabled(uint8_t enabled)
//{
//    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
//}

///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
//*��������:      ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
//*******************************************************************************/
//void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
//{
//    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
//}

///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_setI2CBypassEnabled(uint8_t enabled)
//*��������:      ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
//*******************************************************************************/
//void MPU6050_setI2CBypassEnabled(uint8_t enabled)
//{
//    IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
//}

//void MPU6050_setDLPF(uint8_t mode)
//{
//    IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
//}
///**************************ʵ�ֺ���********************************************
//*����ԭ��:      void MPU6050_initialize(void)
//*��������:      ��ʼ��   MPU6050 �Խ������״̬��
//*******************************************************************************/
//void MPU6050_Init(void)
//{
//    I2c_Init();
//    MPU6050_setSleepEnabled(0); //���빤��״̬
//    delay_ms(200);
//    MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //����ʱ��  0x6b   0x01
//    delay_ms(50);
//    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//������������� +-2000��ÿ��
//    delay_ms(50);
//    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4); //���ٶȶ�������� +-4G
//    delay_ms(50);
//    MPU6050_setDLPF(MPU6050_DLPF_BW_10);
//    delay_ms(50);
//    MPU6050_setI2CMasterModeEnabled(0);  //����MPU6050 ����AUXI2C
//    delay_ms(50);
//    MPU6050_setI2CBypassEnabled(1);  //����������I2C��    MPU6050��AUXI2C    ֱͨ������������ֱ�ӷ���HMC5883L
//    delay_ms(50);
//}
