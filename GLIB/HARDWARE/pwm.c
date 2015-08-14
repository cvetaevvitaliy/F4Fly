#include "pwm.h"
#include "Pwm_Config.h"
/**
 * stm32f103vct6 100pin
 * data
 */

/*
��
TIM2_PWM_Init(u16 arr, u16 psc)//arr���Զ���װ psc��ʱ��Ԥ��Ƶ��
TIM2_PWM_Init(900,1);//900 ����Ƶ,PWMƵ��=72000/900=80Khz
TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)��
TIM_SetCompare1(TIM2,100);
TIM_SetCompare2(TIM2,200);
TIM_SetCompare3(TIM2,300);
TIM_SetCompare4(TIM2,400);
*/
/*****************************************************
REMAP        |   00    |   01    |   10    |   11    |
*****************************************************|
TIM2_CH1_ETR |   PA0   |PA15(JTDI)| (PA0)  |(PA15)   |
TIM2_CH2     |   PA1   |PB3(JTDO) | (PA1)  |(PB3 )   |
TIM2_CH3     |        PA2        |    (PB10)(!36)    |
TIM2_CH4     |        PA3        |    (PB11)(!36)    |
*****************************************************|
TIM3_CH1     |   PA6   |PB4(JNTRST)|       |(PC6)(64 100 144)|
TIM3_CH2     |   PA7   |   PB5   |         |(PC7)    |
TIM3_CH3     |        PB0        |         |(PC8)    |
TIM3_CH4     |        PB1        |         |(PC9)    |
*****************************************************|
TIM4_CH1     |   PB6   |(PD12)(100 144)|   |         |
TIM4_CH2     |   PB7   |(PD13)(100 144)|   |         |
TIM4_CH3     |   PB8   |(PD14)(100 144)|   |         |
TIM4_CH4     |   PB9   |(PD15)(100 144)|   |         |
*****************************************************|
TIM5_CH1     |   PA0   |                             |
TIM5_CH2     |   PA1   |                             |
TIM5_CH3     |   PA2   |                             |
TIM5_CH4     |   PA3   |                             |
*****************************************************|
TIM8_ETR     |   PA0   |                             |
TIM8_CH1     |   PC6   |                             |
TIM8_CH2     |   PC7   |                             |
TIM8_CH3     |   PC8   |                             |
TIM8_CH4     |   PC9   |                             |
TIM8_BKIN    |   PA6   |                             |
TIM8_CH1N    |   PA7   |                             |
TIM8_CH2N    |   PB0   |                             |
TIM8_CH3N    |   PB1   |                             |
*****************************************************|
TIM1_ETR     |        PA12       |   PE7 (100 144)   |
TIM1_CH1     |        PA8        |   PE9 (100 144)   |
TIM1_CH2     |        PA9 (Tx)   |   PE11(100 144)   |
TIM1_CH3     |        PA10(Rx)   |   PE13(100 144)   |
TIM1_CH4     |        PA11(CANRx)|   PE14(100 144)   |
TIM1_BKIN    |PB12(!36)|   PA6   |   PE15(100 144)   |
TIM1_CH1N    |PB13(!36)|   PA7   |   PE8 (100 144)   |
TIM1_CH2N    |PB14(!36)|   PB0   |   PE10(100 144)   |
TIM1_CH3N    |PB15(!36)|   PB1   |   PE12(100 144)   |
*****************************************************/

/**
 * PWM�����ʼ��
 * @param arr [�Զ���װ��ֵ]
 * @param psc [ʼ��Ԥ��Ƶֵ]
 */
void TIM2_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��Xʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);//ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); //GPIOA0����Ϊ��ʱ��2
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2); //GPIOA0����Ϊ��ʱ��2


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA, &GPIO_InitStructure);               //��ʼ��GPIO


    //��ʼ��TIM2
    TIM_TimeBaseStructure.TIM_Period = arr - 1;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;          //ѭ����������ֵ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܶ�ʱ���������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;       //�������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //�򿪿���״̬ѡ��ر�

    TIM_CtrlPWMOutputs(TIM2, ENABLE); //ʹ�ܶ�ʱ��8��PWM���

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);         //����Tָ���Ĳ�����ʼ������TIM2
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM2��Ԥװ�ؼĴ���

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);         //����Tָ���Ĳ�����ʼ������TIM2
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIM2��Ԥװ�ؼĴ���

    TIM_Cmd(TIM2, ENABLE);//ʹ��TIM2
		
}


void TIM4_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    static TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //ʹ�ܶ�ʱ��3ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
 
   	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOA0����Ϊ��ʱ��2
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOA0����Ϊ��ʱ��2
   	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOA0����Ϊ��ʱ��2
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOA0����Ϊ��ʱ��2
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    //��ʼ��TIM4
    TIM_TimeBaseStructure.TIM_Period = arr - 1;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;          //ѭ����������ֵ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM4  PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܶ�ʱ���������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;       //�������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //�򿪿���״̬ѡ��ر�

    TIM_CtrlPWMOutputs(TIM4, ENABLE); //ʹ�ܶ�ʱ��8��PWM���
		
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4

}
//TIM8 PWM���ֳ�ʼ��
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM8_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    static TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   //ʹ�ܶ�ʱ��3ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
 
   	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOA0����Ϊ��ʱ��2
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOA0����Ϊ��ʱ��2

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOC, &GPIO_InitStructure);

  
    //��ʼ��TIM8
    TIM_TimeBaseStructure.TIM_Period = arr - 1;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;          //ѭ����������ֵ
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM8  PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܶ�ʱ���������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;       //�������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //�򿪿���״̬ѡ��ر�

    TIM_CtrlPWMOutputs(TIM8, ENABLE); //ʹ�ܶ�ʱ��8��PWM���

    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
		
		TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

    TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8

}



