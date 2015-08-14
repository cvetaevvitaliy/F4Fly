#include "ultrasonic.h"
#include "delay.h"
#include "stm32f4xx.h"
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

	/*使能PA、PB、PC、PD口时钟，使能IO口复用时钟*/
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE);
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);                                              //置低
	 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1); 
}
void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 
 TIM_DeInit(TIM1);
 TIM_TimeBaseStructure.TIM_Period=0XFFFF-1;		 //ARR的值, 一次计时6秒
 TIM_TimeBaseStructure.TIM_Prescaler=71;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
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
