#include "motor.h"

#define Moto_PwmMax 1000

#define PWM_SetCompare1(x)  TIM_SetCompare2(TIM8, (x));
#define PWM_SetCompare2(x)  TIM_SetCompare1(TIM2, (x));
#define PWM_SetCompare3(x)  TIM_SetCompare2(TIM2, (x));
#define PWM_SetCompare4(x)  TIM_SetCompare1(TIM8, (x));
int32_t MOTO1_PWM, MOTO2_PWM, MOTO3_PWM, MOTO4_PWM;

void Moto_PwmRflash(int32_t MOTO1_PWM, int32_t MOTO2_PWM, int32_t MOTO3_PWM, int32_t MOTO4_PWM)
{

    PWM_SetCompare1(MOTO1_PWM + 2300);
    PWM_SetCompare2(MOTO2_PWM + 2300);
    PWM_SetCompare3(MOTO3_PWM + 2300);
    PWM_SetCompare4(MOTO4_PWM + 2300);
}


//T22 T21
//T81 T82



//01
//23

//20
//31

//10
//23

//10
//32
