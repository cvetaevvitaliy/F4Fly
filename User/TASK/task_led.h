#ifdef __cplusplus
       extern "C" {
#endif
#include "gpio.h"
#include "sys_os.h"
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态

#define LED1_Toggle   digitalToggle(GPIOA,GPIO_Pin_2)
#define LED1_H        GPIO_SetBits(GPIOA,GPIO_Pin_2);
#define LED1_L        GPIO_ResetBits(GPIOA,GPIO_Pin_2); 

void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);//GPIOF9,F10设置高，灯灭

}
int task_led(void)
{
    _SS
    LED_Init();
//    GPIO_ResetBits(GPIOE,GPIO_Pin_3); 
    while (1)
    {
			WaitX(100);
//        if (RC_Control.ARMED)
//        {
//            if (RC_Control.ALT_ON_OFF)
//            {
//                WaitX(50);
//                LED1_Toggle;
//            }
//            else
//            {
//                WaitX(100);
//                LED1_H;
//            }
//        }
//        else
        {
            WaitX(200);
            LED1_Toggle;
        }
				        

    }
    _EE
}

#ifdef __cplusplus
        }
#endif
