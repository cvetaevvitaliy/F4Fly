#ifndef __IMU_H_
#define __IMU_H_
#ifdef __cplusplus
       extern "C" {
#endif
#include "stm32f4xx.h"
#include "MPU6050.h"


typedef struct{
    float rol;
    float pit;
    float yaw;}T_float_angle;

extern void IMUupdate(S_INT16_XYZ *acc, S_INT16_XYZ *gyr, T_float_angle *angle);
extern T_float_angle    Att_Angle;  //ATT?????????
extern void Prepare_Data(S_INT16_XYZ *acc_in, S_INT16_XYZ *acc_out);
extern void Prepare_Data2(T_float_angle *angle_in);
extern float   AngleOffset_Rol, AngleOffset_Pit;
extern float position_x,position_y,position_z;
#ifdef __cplusplus
        }
#endif
#endif
