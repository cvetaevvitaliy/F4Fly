#ifdef __cplusplus
extern "C" {
#endif
#include "stdio.h"
#include "pfilter.h"
#include "usr_usart.h"
#include "math.h"
#include "ultrasonic.h"	
#define N 3
u16 Alt_ultrasonic=0;
u16 Ultrasonic=0;	
u16 Alt_ultrasonic1t;
u16 Alt_ultrasonic2t;

int Alt_temp;

u8 flag_ALT;

u16 i=0,k,j,t=0;
u16 mid[3];
	void Delay(vu32 nCount)
{
	for(; nCount != 0; nCount--);
}
int task_ultrasonic(void)
{
    _SS
    Ultrasonic_Init();
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);
	delay_ms(100);
     while (1)
   {
		 flag_ALT = 1;
		 	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)0);//低电平触发
			 delay_us(10);//保证大于10uS
			GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)1);//恢复高电平
          t++;
if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
	{	 //	程序停在这里一直到ECHO检测到搞电平	
		t=0;
		TIM_Cmd(TIM1, ENABLE);											//					 //开始计时，计时时间
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1);		 //	高电平持续
		TIM_Cmd(TIM1, DISABLE);		 //					关闭时钟
    Ultrasonic=(int)((180*pow(10.0,-6)*((double)TIM_GetCounter(TIM1)/2.0))*1000);
		TIM_SetCounter(TIM1,0x00);	
Alt_temp=Ultrasonic-mid[0];
if(Ultrasonic!=0&&Alt_temp<200&&Alt_temp>-200)
{
	mid[2]=mid[1];mid[1]=mid[0];mid[0]=Ultrasonic;
  Alt_ultrasonic=(mid[0]+mid[1]+mid[2])/3;
}
 // Sys_Printf(Printf_USART,"Distance= %d\r\n",Alt_ultrasonic);

						   WaitX(25);			
						}
	if(t>100){WaitX(25);t=0;}
   }


    _EE
}
#ifdef __cplusplus
}
#endif
