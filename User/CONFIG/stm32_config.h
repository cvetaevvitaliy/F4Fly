#ifndef __STM32_CONFIG_
#define __STM32_CONFIG_
#ifdef __cplusplus
       extern "C" {
#endif
#define VERSIONS 2
/* 
1 黑板
2 yyb绿板
*/
#define ROTATE90 0
/*
1 
Rothmarketing 90 CCW
方向四轴旋转90度
0
不旋转
*/
#define THROTTLE_INVERTED 1 //throttleinverted 1
/*
1 反转
0 不反转
油门反相 学长遥控需要反相
*/

/*
#include "stm32_config.h"
*/

/***USART***/
#include "Usart_Config.h"
/***USART***/
/****************************************/
/*** LCD ***/
/******************************/

/******************************/
/*** LCD ***/
/****************************************/
/***TOUCH***/
/***TOUCH***/
/****************************************/

/***PWM***/
#include "Pwm_Config.h"
/***PWM***/
/****************************************/
/***MPU6050***/
//#define MPU6050_USE_DMP
/***MPU6050***/
/****************************************/
/***SD***/
/***SD***/

#ifdef __cplusplus
        }
#endif
#endif

