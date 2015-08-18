/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "sys_usart.h"
#include "usr_usart.h"
#include "G32_I2c.h"
#include "sys_os.h"
#include "./TASK/task_6050.h"
#include "./TASK/loop_fast.h"
#include "./TASK/task_pwm_ex.h"
#include "./TASK/task_ultrasonic.h"
#include "./TASK/task_cap_rc.h"
#include "./TASK/task_PX4FLOW.h"
#include "./TASK/task_Oled.h"
#include "./TASK/task_led.h"

#if 1==VERSIONSBY
#error 1
#elif 2==VERSIONSBY
//#error 2
#endif

void SYS_INIT(void)
{

    delay_init(168);
    NVIC_Configuration();
    USART3_Configuration(256000);
    USART_NVIC_Configuration(3, 2, 0, 0);
    USART1_Configuration(115200);
    USART_NVIC_Configuration(1, 2, 0, 0);

    // Sys_Printf(USART1, "\r\nUSART1 ok");
    // Sys_Printf(USART2, "\r\nUSART2 ok");
    // Sys_Printf(USART3, "\r\nUSART3 ok");

    Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok");
//Data_Read();
    delay_ms(100);
}
#include "data_transfer.h"

#define RelayOff        GPIO_SetBits(GPIOB,GPIO_Pin_13);
#define RelayOn        GPIO_ResetBits(GPIOB,GPIO_Pin_13); 


void RelayInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);//GPIOF9,F10设置高，灯灭

}
#include "Rc.h"

int task_control(void)
{
    _SS	
	    *RuiSaTh1=850;
			*RuiSaTh2=850;
			*RuiSaTime1=850;
			*RuiSaTime2=2500;
			*RuiSaTime3=850;
			
    while (1)
    {

			
			
        WaitX(100);
        if (1 == RuiSaKey)
        {
            Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok1");
            WaitX(1000);RC_Control.ARMED = 1;
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
										RuiSaKey=0;
					RC_Control.ARMED = 0;

        }
        else if (2 == RuiSaKey)
        {
            Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok2");

            WaitX(1000);RC_Control.ARMED = 1;
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
//            run();
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
										RuiSaKey=0;
					RC_Control.ARMED = 0;

        }
        else if (3 == RuiSaKey)
        {
					
            Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok3");

            WaitX(1000);
            
					RelayOn;
            WaitX(3000);
					RC_Control.ARMED = 1;
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
            RelayOff;
            RuiSaPit = -5;
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
										RuiSaKey=0;
					RC_Control.ARMED = 0;

        }
        else if (4 == RuiSaKey)
        {
					
            Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok4");

            WaitX(1000);
            RC_Control.ARMED = 1;RelayOn;
            RuiSaThr = *RuiSaTh1;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = 5;
            WaitX((int)(*RuiSaTime2));
           // FindPoint();
            RuiSaThr = 0;
            WaitX((int)(*RuiSaTime3));
            RuiSaThr = *RuiSaTh2;
            WaitX((int)(*RuiSaTime1));
            RuiSaPit = -5;
            WaitX((int)(*RuiSaTime2));
            RuiSaThr = 0;
										RuiSaKey=0;
					RC_Control.ARMED = 0;
        }
				else
				{
														RuiSaKey=0;
}
				
    }
    _EE
}

int main(void)
{
    SYS_INIT();
    while (1)
    {
        RunTaskA(task_6050, 1);
        RunTaskA(task_fast, 2);
        RunTaskA(task_pwm_ex, 3);
        RunTaskA(task_cap_rc, 4);
        RunTaskA(task_led, 5);
    }
}
