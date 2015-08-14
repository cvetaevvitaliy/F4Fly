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
u16 aid[5];u16 bid[5];
u16 Alt_t;
//float sum_x=0,sum_y=0;
//int v_x,v_y;//extern float sum_x,sum_y;
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
{   extern u16 Alt_ultrasonic;
	  extern S_INT16_XYZ Mag;//extern S_INT16_XYZ Mag
    flow.frame_count      = (int16_t)((flow_buf[0])|(flow_buf[1]<<8)) ;
    flow. pixel_flow_x_sum= (int16_t)((flow_buf[2])|(flow_buf[3]<<8)) ;//����ƽ���ٶ�
    flow. pixel_flow_y_sum= (int16_t)((flow_buf[4])|(flow_buf[5]<<8)) ;
    flow. flow_comp_m_x   = (int16_t)((flow_buf[6])|(flow_buf[7]<<8)) ;//�߶Ȳ������ٶ�
    flow. flow_comp_m_y   = (int16_t)((flow_buf[8])|(flow_buf[9]<<8));
	  flow.gyro_x_rate =(int16_t)((flow_buf[12])|(flow_buf[13]<<8)) ;//������
   	flow.gyro_y_rate =(int16_t)((flow_buf[14])|(flow_buf[15]<<8)) ;
   	flow.gyro_z_rate =(int16_t)((flow_buf[16])|(flow_buf[17]<<8)) ;
    flow.ground_distance  = (int16_t)((flow_buf[20])|(flow_buf[21]<<8));//�������߶�
	if(flow.ground_distance<100)flow. flow_comp_m_x=flow. flow_comp_m_y=0;
	 // sum_x=sum_x+((float)flow. flow_comp_m_x*0.000224);//xƽ���ۼ�
  //  sum_y=sum_y+((float)flow. flow_comp_m_y*0.000224);//yƽ���ۼ�
//	  Mag.x=sum_x;
//	  Mag.y=sum_y;
	//Sys_Printf(Printf_USART, "m_x:%lf\n",sum_x);//�ٶ�
	//Sys_Printf(Printf_USART, "v_x:%lf\n",sum_y);
    if(flow.ground_distance!=0&&flow.ground_distance<3000)
	{
	  bid[1]=aid[1]=aid[0];bid[2]=aid[2]=aid[1];bid[3]=aid[3]=aid[2];bid[4]=aid[4]=aid[3];
		bid[0]=aid[0] =flow.ground_distance;//if(aid[I]!=0&&aid[I]<2500)I++;//������2.5m
     for (J=0;J<4;J++)
   {for (K=0;K<4;K++)
    {if(bid[K]>bid[K+1])
		 {Alt_t= bid[K];
       bid[K] = bid[K+1];
        bid[K+1] =Alt_t;
	   }
	  }
	 }
   Alt_ultrasonic=bid[2];
	}

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
