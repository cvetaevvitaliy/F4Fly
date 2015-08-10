#include "ultrasonic.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stdio.h"

/* Private macro -------------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : 系统时钟初始化
* Input          : None
* Output         : None
* Return         : None
* Others         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		  /* ADCCLK = PCLK2/4 */
		RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
	/*使能PA、PB、PC、PD口时钟，使能IO口复用时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
						    RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : GPIO初始化:TRGI
* Input          : None
* Output         : None
* Return         : None
* Others         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 
 TIM_DeInit(TIM1);
 TIM_TimeBaseStructure.TIM_Period=60000;		 //ARR的值, 一次计时6秒
 TIM_TimeBaseStructure.TIM_Prescaler=72;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
}
void SysTick_Configuration(void)
{
  if (SysTick_Config(SystemCoreClock / 100000))//配置点滴时钟的频率，每10us进一次中断  
  {
    while (1);
  }  
}

/*******************************************************************************
* Function Name  : Sys_Configuration
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
* Others         : None
*******************************************************************************/
void Ultrasonic_Init(void)
{
	RCC_Configuration();
//SysTick_Configuration();
	GPIO_Configuration();
	Timer_Configuration();
}
/******** (C) COPYRIGHT 2012 Youpengshanren **********END OF FILE**********/
