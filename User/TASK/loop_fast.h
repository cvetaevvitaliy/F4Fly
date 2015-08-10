#ifdef __cplusplus
       extern "C" {
#endif
T_float_angle   Att_Angle_Avg;
#include "Rc.h"
#include "balance.h"
int task_fast(void)//500hz
{
    extern S_INT16_XYZ Acc, Average_Acc, Gyr, Mag;
    _SS
    while (1)
    {
        WaitX(2);
        if (flag_ACC)
        {
            IMUupdate(&Gyr, &Average_Acc, &Att_Angle);//222us转换为欧拉角
            Prepare_Data2(&Att_Angle);//24us欧拉角滤波
            //        //Control(&Att_Angle, &Gyr, &Rc_D, &RC_Control);//17us
            Balance(&Att_Angle, &Gyr, &Acc, &Rc_D, &RC_Control); //17usPID算法

        }
    }
    _EE
}

#ifdef __cplusplus
        }
#endif
