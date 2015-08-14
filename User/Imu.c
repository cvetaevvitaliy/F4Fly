#include "Imu.h"
#include "math.h"
#include "sys_usart.h"
#include "usr_usart.h"
#define RtA 		57.324841				
#define AtR    	0.0174533				
#define Acc_G 	0.0011963				
#define Gyro_G 	0.0610351				
#define Gyro_Gr	0.0010653f			

S_INT16_XYZ ACC_AVG;					
T_float_angle angle;			

#define FILTER_NUM  10
void Prepare_Data(S_INT16_XYZ *acc_in, S_INT16_XYZ *acc_out)
{
    static uint8_t  filter_cnt = 0;
    static int16_t  ACC_X_BUF[FILTER_NUM], ACC_Y_BUF[FILTER_NUM], ACC_Z_BUF[FILTER_NUM];
    static int32_t  temp1 = 0, temp2 = 0, temp3 = 0;
    uint8_t i;

    if (ACC_Z_BUF[FILTER_NUM - 1] == 0)
    {
        temp1 = temp2 = temp3 = 0;
        ACC_X_BUF[filter_cnt] = acc_in->x;
        ACC_Y_BUF[filter_cnt] = acc_in->y;
        ACC_Z_BUF[filter_cnt] = acc_in->z;
        for (i = 0; i < FILTER_NUM; i++)
        {
            temp1 += ACC_X_BUF[i];
            temp2 += ACC_Y_BUF[i];
            temp3 += ACC_Z_BUF[i];
        }
    }
    else
    {
        temp1 = temp1 - ACC_X_BUF[filter_cnt] + acc_in->x;
        temp2 = temp2 - ACC_Y_BUF[filter_cnt] + acc_in->y;
        temp3 = temp3 - ACC_Z_BUF[filter_cnt] + acc_in->z;

        ACC_X_BUF[filter_cnt] = acc_in->x;
        ACC_Y_BUF[filter_cnt] = acc_in->y;
        ACC_Z_BUF[filter_cnt] = acc_in->z;
    }
    acc_out->x = temp1 / FILTER_NUM;
    acc_out->y = temp2 / FILTER_NUM;
    acc_out->z = temp3 / FILTER_NUM;
    filter_cnt++;
    if (filter_cnt == FILTER_NUM)  filter_cnt = 0;
}
#define FILTER_NUM2     2
#define FILTER_NUM3     10
void Prepare_Data2(T_float_angle *angle_in)
{
    static uint8_t  filter_cnt = 0;
    static uint8_t  filter_cnt2 = 0;
    static float    ACC_X_BUF[FILTER_NUM2], ACC_Y_BUF[FILTER_NUM2], ACC_Z_BUF[FILTER_NUM3];
    float temp1 = 0, temp2 = 0, temp3 = 0;
    uint8_t i;
    ACC_X_BUF[filter_cnt]  = angle_in->pit;
    ACC_Y_BUF[filter_cnt]  = angle_in->rol;
    ACC_Z_BUF[filter_cnt2] = angle_in->yaw;
    for (i = 0; i < FILTER_NUM2; i++)
    {
        temp1 += ACC_X_BUF[i];
        temp2 += ACC_Y_BUF[i];
    }
    for (i = 0; i < FILTER_NUM3; i++)
    {
        temp3 += ACC_Z_BUF[i];
    }
    angle_in->pit = temp1 / FILTER_NUM2;
    angle_in->rol = temp2 / FILTER_NUM2;
    angle_in->yaw = temp3 / FILTER_NUM3;
    filter_cnt++;
    filter_cnt2++;
    if (filter_cnt == FILTER_NUM2)  filter_cnt = 0;
    if (filter_cnt2 == FILTER_NUM3) filter_cnt2 = 0;
}





////////////////////////////////////////////////////////////////////////////////
#define Kp 0.4f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.0f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period???????

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;// scaled integral error
//====================================================================================================
// Function
//====================================================================================================
T_float_angle   Att_Angle;  //ATT?????????
//float position_x=0,position_y=0,position_z=0;
float compass_yaw;
float AngleOffset_Rol=0,AngleOffset_Pit=0;
void IMUupdate(S_INT16_XYZ *gyr, S_INT16_XYZ *acc, T_float_angle *angle)
{
    float ax = acc->x, ay = acc->y, az = acc->z;
    float gx = gyr->x, gy = gyr->y, gz = gyr->z;
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;
    gx *= Gyro_Gr;
    gy *= Gyro_Gr;
    gz *= Gyro_Gr;
    // normalise the measurements
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
//  Sys_Printf(Printf_USART, "acc:%lf\n",az);

    // ???????????????
    // estimated direction of gravity
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
    // ?????????«??????»???????????
    // ?????????????,??????????,???????,?????????
    // ?????vx\y\z,??????????(????)?????????,????????????
    // error is sum of cross product between reference direction of field and direction measured by sensor
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    // axyz?????????,????????????,??????????????
    // axyz??????????,vxyz??????????????????,??????????????????
    // ??????????,?????????????????????????
    // ??????,???????(?????????)???,exyz????????????
    // ??????????????????,???????????????,?????????????????,?????????(????????????)????????????,?????????????????????????
    // integral error scaled integral gain
    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;
    // adjusted gyroscope measurements

    gx = gx + Kp*ex + exInt;					   							//½«Îó²îPIºó²¹³¥µ½ÍÓÂÝÒÇ£¬¼´²¹³¥ÁãµãÆ¯ÒÆ
    gy = gy + Kp*ey + eyInt;				   							
    gz = gz + Kp*ez + ezInt;					   							

    // ???????PI??????
    // integrate quaternion rate and normalise
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;
    // ???????
    // normalise quaternion
    norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
    // ??????

//	if(f_angle==0&&RC_Control.ARMED==1)f_angle=compass_yaw;
//  if(RC_Control.ARMED==0)f_angle=0;
//	
//	  angle->yaw=compass_yaw-f_angle;
//if(angle->yaw<1&&angle->yaw>-1)		angle->yaw=0;
	    static float Yaw_I = 0;
		Yaw_I = MPU6050_GYRO_LAST.z*Gyro_G*0.01;
	  angle->yaw =angle->yaw + Yaw_I;// (0.99) * (angle->yaw + Yaw_I) + (0.01) * (compass_yaw);
    angle->pit = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.2957795f; // pitch
    angle->rol = -atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.2957795f; // roll
//  Sys_Printf(Printf_USART, "m_x:%lf   ,%lf   ,%lf   \n",angle->pit,angle->rol,angle->yaw);//ËÙ¶È

}
////====================================================================================================
//// END OF CODE
////====================================================================================================
////////////////////////////////////////////////////////////////////////////////

