#ifdef __cplusplus
extern "C" {
#endif
#include "stdio.h"
#include "PX4FLOW.h"
#include "pfilter.h"
#include "usr_usart.h"
#include "minos_delay.h"

int task_PX4FLOW(void)
{


    _SS

	delay_ms(2000);
     while (1)
   { 
		 WaitX(2);		

		    PX4FLOW_Read();
        PX4FLOW_Dataanl();//5us
   }


    _EE
}
#ifdef __cplusplus
}
#endif
