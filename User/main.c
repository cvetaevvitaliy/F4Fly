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
void SYS_INIT(void)
{

    delay_init(168);
    NVIC_Configuration();
    uart3_init(115200);

    // Sys_Printf(USART1, "\r\nUSART1 ok");
    // Sys_Printf(USART2, "\r\nUSART2 ok");
    // Sys_Printf(USART3, "\r\nUSART3 ok");

    Sys_Printf(Printf_USART, (char *)"\r\nPrintf_USART ok");

    delay_ms(100);
}

int main(void)
{	
	  SYS_INIT();
		 while (1)
 {  
		 RunTaskA(task_6050, 0);
		 RunTaskA(task_fast, 1);
		 RunTaskA(task_pwm_ex, 2);
		 RunTaskA(task_PX4FLOW,3);
		 RunTaskA(task_cap_rc, 4);
		 RunTaskA(task_Oled, 5);
	 }

}
