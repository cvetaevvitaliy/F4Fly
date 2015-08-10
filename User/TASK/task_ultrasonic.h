#ifdef __cplusplus
extern "C" {
#endif
#include "stdio.h"
#include "pfilter.h"
#include "usr_usart.h"
#include "math.h"
	
//#define N 3
u16 Alt_ultrasonic=0;
u16 Ultrasonic=0;	
u16 Alt_ultrasonic1t;
u16 Alt_ultrasonic2t;

u16 Alt_temp;

u8 flag_ALT;

//u16 i=0,k,j,t=0;
//u16 mid[3];
//	void Delay(vu32 nCount)
//{
//	for(; nCount != 0; nCount--);
//}
int task_ultrasonic(void)
{
    _SS
//    Ultrasonic_Init();
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);
//	delay_ms(100);
     while (1)
   { flag_ALT = 1;
//		 	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)0);//低电平触发
//			 delay_us(10);//保证大于10uS
//		   t++;
//			GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);//恢复高电平
////Sys_Printf(Printf_USART,"Distance\n");
//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
//	{	 //	程序停在这里一直到ECHO检测到搞电平	
//		TIM_Cmd(TIM1, ENABLE);											//					 //开始计时，计时时间
//		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1);		 //	高电平持续
//		TIM_Cmd(TIM1, DISABLE);		 //					关闭时钟
//   Ultrasonic=(int)((180*pow(10.0,-6)*((float)TIM_GetCounter(TIM1)))*1000);
//		TIM_SetCounter(TIM1,0x00);	
////	Sys_Printf(Printf_USART,"Distance=%d\r\n", att_in->pit);
//	mid[i] =Ultrasonic;if(mid[i]!=0)i++;
//    if(i==N)
//	{i=0;
//  for (j=0;j<N;j++)
//{
// for (k=0;k<N;k++)
//{
//if(mid[k]>mid[k+1])
//{
//Alt_temp= mid[k];
//mid[k] = mid[k+1];
//mid[k+1] =Alt_temp;
//}}}
//Alt_ultrasonic=mid[1];}
////		Sys_Printf(Printf_USART,"Distance=%d\r\n",Alt_ultrasonic);
//		///////////////////////////////////////////////////////////
//				TIM_SetCounter(TIM1,0);t=0;
//						   WaitX(50);			
//						}
//if(t>50) WaitX(30);		

   }


    _EE
}
#ifdef __cplusplus
}
#endif
