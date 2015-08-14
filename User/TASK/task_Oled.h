#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "Oled.h"
#include "pfilter.h"
#include "usr_usart.h"
#include "math.h"
extern u16 Alt_ultrasonic;
extern unsigned char RxBuffer[10];
extern int flag;
	
int task_Oled(void)
{
    _SS
	delay_ms(200);
	Oled_Init();
	OLED_Fill(0x00);//ȫ����
	OLED_ShowStr(0,1,(unsigned char*)"Wildfire Tech",1);
     while (1)
   {	
		while(RxBuffer[flag]!='\0')
		Sys_Printf(Printf_USART, "%c",RxBuffer[flag++]);//�ٶ�
		
     WaitX(2);
//		OLED_ShowStr(0,0,(unsigned char*)"%c,i",1);				//����6*8�ַ�
//		OLED_ShowStr(0,1,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�
//		OLED_ShowStr(0,2,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�		 
//		OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�
//		OLED_ShowStr(0,4,(unsigned char*)"Hello wildfire",1);				//����8*16�ַ�
//		OLED_ShowStr(0,5,(unsigned char*)"Hello wildfireWie",2);				//����8*16�ַ�
//		OLED_ShowStr(0,6,(unsigned char*)"Hello wildfdfire",2);				//����8*16�ַ�
//		OLED_ShowStr(0,7,(unsigned char*)"Hello eWildfire",2);				//����8*16�ַ�
   }


    _EE
}
#ifdef __cplusplus
}
#endif
