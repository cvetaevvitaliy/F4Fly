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
	OLED_Fill(0x00);//È«ÆÁÃð
	OLED_ShowStr(0,1,(unsigned char*)"Wildfire Tech",1);
     while (1)
   {	
		while(RxBuffer[flag]!='\0')
		Sys_Printf(Printf_USART, "%c",RxBuffer[flag++]);//ËÙ¶È
		
     WaitX(2);
//		OLED_ShowStr(0,0,(unsigned char*)"%c,i",1);				//²âÊÔ6*8×Ö·û
//		OLED_ShowStr(0,1,(unsigned char*)"Wildfire Tech",1);				//²âÊÔ6*8×Ö·û
//		OLED_ShowStr(0,2,(unsigned char*)"Wildfire Tech",1);				//²âÊÔ6*8×Ö·û		 
//		OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//²âÊÔ6*8×Ö·û
//		OLED_ShowStr(0,4,(unsigned char*)"Hello wildfire",1);				//²âÊÔ8*16×Ö·û
//		OLED_ShowStr(0,5,(unsigned char*)"Hello wildfireWie",2);				//²âÊÔ8*16×Ö·û
//		OLED_ShowStr(0,6,(unsigned char*)"Hello wildfdfire",2);				//²âÊÔ8*16×Ö·û
//		OLED_ShowStr(0,7,(unsigned char*)"Hello eWildfire",2);				//²âÊÔ8*16×Ö·û
   }


    _EE
}
#ifdef __cplusplus
}
#endif
