#include "Control.h"
#include "Balance.h"
#include "data_transfer.h"
#include "pwm.h"
#include "usr_usart.h"
#include "math.h"
#include "motor.h"
#include "filter.h"
#include "pfilter.h"
#include "delay.h"
#include "math.h"
#include "IMU.h"
//#include "ADNS3080.h"

#define RC_Yaw_P 20
#define RC_ROL_P 50
#define RC_PIT_P 50


//#define RC_FUN_MIN  1200
//#define RC_FUN_MAX  1800
//#define RC_FUN_CNT  20
//#define RC_FUN_ZERO 1000
u32 ALT_Set = 500 ; // (rc_in->AUX2 - 1100);//定高
static float rol_i = 0, pit_i = 0, yaw_i = 0, alt_i = 0,
             rol_ni = 0,  pit_ni = 0;
PID PID_ROL, PID_PIT, PID_YAW, PID_ALT, PID_POS,
    PID_PID_1, PID_PID_2, PID_PID_3, PID_PID_4,
    PID_PID_5, PID_PID_6, PID_PID_7, PID_PID_8,
    PID_PID_9, PID_PID_10, PID_PID_11, PID_PID_12;
extern int sum_x, sum_y;
static T_float_angle *att_in;
static S_INT16_XYZ   *gyr_in;
//static S_INT16_XYZ   *acc_in;
static T_RC_Data     *rc_in ;
static T_Control     *ctl;
float     IN_ROP = 0.04,    IN_POP = 0.04,  IN_YOP = 0.045, //0.04
          IN_ROI = 0,   IN_POI = 0, IN_YOI = 0,
          IN_ROD = 0.80,    IN_POD = 0.80, IN_YOD = 0.012, //1.33
          Last_Angle_gx = 0,  LAST_Angle_gy = 0, LAST_Angle_gz = 0,
          Out_PID_X = 0,      Out_PID_Y = 0,    Out_PID_Z;
int     Angle_gy = 0,       Angle_gx = 0,     Angle_gz = 0 , qifei = 0, down = 800, time = 15;

//位置偏移////////////////////////////////////////////////////////
float Last_balance_x = 0, Last_balance_y = 0, Last_v_x = 0, Last_v_y = 0;
float balance_x = 0, v_x, X_PID = 0;
float balance_y = 0, v_y, Y_PID = 0;
extern int sum_x, sum_y;
extern unsigned char OV7670[5];
int p = 50;
void Balance_Data(T_float_angle *Att_in, S_INT16_XYZ *Gyr_in, S_INT16_XYZ *Acc_in, T_RC_Data *Rc_in, T_Control *Ctl)
{
    att_in = Att_in;
    gyr_in = Gyr_in;
    //    acc_in = Acc_in;
    rc_in  = Rc_in ;
    ctl    = Ctl   ;
}
/*****************************************************
/CONTROL
/armed 1 解锁
       0 上锁
*****************************************************/
#define DEAD_BAND 15
#define INTEGRAL_WINDUP_R 300
#define INTEGRAL_WINDUP_P 300
#define INTEGRAL_WINDUP_Y 3000
#define INTEGRAL_WINDUP_A 700
//float Throttle2;// HROTTLE;
u32 Throttle_OUT;
u32 Throttle_IN;

float PID_ALT_OUT_last = 0, PID_ALT_OUT_BK;
int ALT_Control_Out;
extern u8 flag_ALT;
s32 Alt_Error, Alt_Error_Last;

void ALT_Control(u32 ALT_Set)
{
    static u16 Alt_time = 0, flag_time = 0;
    Alt_time++; if (Alt_time > 50)
    {
        Alt_time = 0;
        flag_time = 1;
    }
    if (1 == flag_ALT || (1 == flag_time && 0 == flag_ALT))
    {
        extern u16 Alt_ultrasonic;
        extern s32 Alt_bmp;
        s32 Alt;

        flag_ALT = 0;
        //time = 0;
        Alt = Alt_ultrasonic;// ? Alt_ultrasonic : Alt_bmp;
        Alt_Error = ALT_Set - Alt;
        PID_ALT.pout = PID_ALT.P * Alt_Error;
        if (PID_ALT.pout < 0)PID_ALT.pout = 0;
        alt_i += Alt_Error;
        PID_ALT.iout = (PID_ALT.I / 100) * alt_i;
        if (PID_ALT.iout > INTEGRAL_WINDUP_A)
            PID_ALT.iout = INTEGRAL_WINDUP_A;
        else if (alt_i < -INTEGRAL_WINDUP_A)
            PID_ALT.iout = -INTEGRAL_WINDUP_A;
        PID_ALT.dout = -PID_ALT.D * (Alt_Error_Last - Alt_Error) * (1000000 / 10000); // (currenttime - lasttime));
        if ( PID_ALT.dout < 0)PID_ALT.dout = 0;
        //if(Alt_ultrasonic<300)PID_ALT.dout=0;
        Alt_Error_Last = Alt_Error;
        //PID_ALT.dout = PID_ALT.D * (acc_in->z-8192);
        //if (ctl->ALT_ON_OFF && Alt_ultrasonic != 0)
        if (Alt_ultrasonic != 0)
        {
            PID_ALT.OUT = PID_ALT.pout + PID_ALT.iout + PID_ALT.dout;
            PID_ALT_OUT_last = PID_ALT.OUT;
        }
        else
        {
            PID_ALT.OUT = PID_ALT_OUT_last;
        }
    }

}
static T_float_angle angle;
void Yaw_Control(void)
{
//if(rc_in->YAW>1420||rc_in->YAW<1400){att_in->yaw=0;}
////////////////////////    外环(PID)      ///////////////////////////////////////
    angle.yaw = att_in->yaw; //+ (-(*yaw_just - 10));//+ (rc_in->YAW   - 1411) / RC_Yaw_P
    //目标角度  = 输入角度    +  输入角度-1411        /比例值    +  前角度-10
    PID_YAW.pout = PID_YAW.P * angle.yaw;
    /*****************************************************
    *****************************************************/
    yaw_i += angle.yaw; //积分
    if (yaw_i > INTEGRAL_WINDUP_Y)
        yaw_i = INTEGRAL_WINDUP_Y;
    else if (yaw_i < -INTEGRAL_WINDUP_Y)
        yaw_i = -INTEGRAL_WINDUP_Y;
    PID_YAW.iout = PID_YAW.I * yaw_i;
    /****************************************************/
    PID_YAW.dout = PID_YAW.D * gyr_in->z;
    /*****************************************************
    /PID
    *****************************************************/
    PID_YAW.OUT = PID_YAW.pout + PID_YAW.iout + PID_YAW.dout;
}
void Rol_Control(void)
{

    extern u16 Alt_ultrasonic;

    balance_x = (float)((int)OV7670[2] - 30) * ((float)Alt_ultrasonic / 200.0); //-tan(att_in->rol/180.0*3.1416)*(float)Alt_ultrasonic*0.1*0.8;
    if (time == 0) {
        v_x = balance_x - Last_balance_x;
        X_PID = 0.2 * balance_x + 2.0 * v_x; //x方向PID+3*v_x
        Mag.x = X_PID;
        Last_balance_x = balance_x;
        time = 15;
    }
    if (X_PID > 3)X_PID = 3; if (X_PID < -3)X_PID = -3;
    if (Alt_ultrasonic < 100)X_PID = 0;
//-------------------外环----------------------------------
    Angle_gx = -(gyr_in->x);
    angle.rol = att_in->rol + (*rol_just - 10) + (rc_in->ROLL  - 1650) / RC_ROL_P;
    //TIM_SetCompare1(TIM4, (162+(int)(att_in->rol*10.0/9.0)));
    //Sys_Printf(Printf_USART, "%lf  ",X_PID);
    Out_PID_X = PID_ROL.P * angle.rol + PID_ROL.I * rol_i;
//-------------------内环-----------------------------------

    PID_ROL.OUT = (Angle_gx * 3.5 + Out_PID_X) * IN_ROP + (Angle_gx - Last_Angle_gx) * IN_ROD;
    Last_Angle_gx = Angle_gx;
    if (PID_ROL.OUT > 400) {PID_ROL.OUT = 400;}
    if (PID_ROL.OUT < -400) {PID_ROL.OUT = -400;}
    //Sys_Printf(Printf_USART, "%lf\n  ",X_PID);
//    angle.rol = att_in->rol;// + (-(*rol_just - 10)) ;//- (rc_in->ROLL  - 1422) / RC_ROL_P
//    /*****************************************************
//    /P
//    *****************************************************/

//    rol_i += angle.rol; //积分
//    if (rol_i > INTEGRAL_WINDUP_R)
//        rol_i = INTEGRAL_WINDUP_R;
//    else if (rol_i < -INTEGRAL_WINDUP_R)
//        rol_i = -INTEGRAL_WINDUP_R;
//
//PID_ROL.pout = PID_ROL.P * angle.rol;
//PID_ROL.iout = PID_ROL.I * rol_i;
//PID_ROL.dout = PID_ROL.D * gyr_in->x;                                                                                         aa

//    PID_ROL.OUT = PID_ROL.pout + PID_ROL.iout - PID_ROL.dout;
}
void Pit_Control(void)
{   extern u16 Alt_ultrasonic;
//
//  balance_y=(float)sum_y;//-tan(att_in->pit/180.0*3.1416)*(float)Alt_ultrasonic*0.1*0.8;//
//  v_y=balance_y-Last_balance_y;
//  Y_PID=balance_y+2*v_y;//
//  Last_balance_y=balance_y;
//  Last_v_y=v_y;
//  if(Y_PID>8)Y_PID=8;if(Y_PID<-8)Y_PID=-8;

//------------------外环----------------------------------
    Angle_gy = (gyr_in->y);
    angle.pit = att_in->pit + (*pit_just - 10) + (rc_in->PITCH - 1500) / RC_PIT_P;
    //TIM_SetCompare2(TIM4, (162-(int)(att_in->pit/9.0*10.0)));
    Out_PID_Y = PID_PIT.P * angle.pit;
//-------------------内环-----------------------------------

    PID_PIT.OUT = (Angle_gy * 3.5 + Out_PID_Y) * IN_POP + (Angle_gy - LAST_Angle_gy) * IN_POD;
    LAST_Angle_gy = Angle_gy;
    if (PID_PIT.OUT > 400) {PID_PIT.OUT = 400;}
    if (PID_PIT.OUT < -400) {PID_PIT.OUT = -400;}
//--------------------------------------------------------------
//    angle.pit = att_in->pit; //+(*pit_just - 10);//+ (rc_in->PITCH - 1413) / RC_PIT_P
//    pit_i += angle.pit; //积分
//    if (pit_i > INTEGRAL_WINDUP_P)
//        pit_i = INTEGRAL_WINDUP_P;
//    else if (pit_i < -INTEGRAL_WINDUP_P)
//        pit_i = -INTEGRAL_WINDUP_P;
//      PID_PIT.pout = PID_PIT.P * angle.pit;
//    PID_PIT.iout = PID_PIT.I * pit_i;
//    PID_PIT.dout = PID_PIT.D * gyr_in->y;
//    PID_PIT.OUT = PID_PIT.pout + PID_PIT.iout + PID_PIT.dout;
}



int Balance_Throttle = 0;//平衡时的 油门大小
int Balance_Throttle1 = 0;//平衡时的 油门大小

void Balance(T_float_angle *att_in, S_INT16_XYZ *gyr_in, S_INT16_XYZ *acc_in, T_RC_Data *Rc_in, T_Control *Ctl)
{
    extern u16 Alt_ultrasonic;
    Balance_Data(att_in, gyr_in, acc_in, Rc_in, Ctl);//赋值
//  if(rc_in->THROTTLE>1700){Throttle_IN=1700-RC_FUN_ZERO;}
//  else {Throttle_IN = rc_in->THROTTLE - RC_FUN_ZERO;}//油门-1200
    time--;
    Yaw_Control();
    Rol_Control();
    Pit_Control();
    //ALT_Control(ALT_Set);
    if (qifei == 1) {
        Throttle_IN = rc_in->THROTTLE - RC_FUN_ZERO;
        Balance_Throttle = Throttle_OUT = Throttle_IN;
        //if(Throttle_OUT>700)Throttle_OUT=700;
        Throttle_OUT = Alt_ultrasonic / 5 + 690; //730
        if (Throttle_OUT > 800)Throttle_OUT = 800;
    }//800

    /*****************************************************
    /CONTROL
    *****************************************************/
    //              ROL
    //               +
    // MOTO1_PWM    /|\    MOTO1_PWM
    //               |
    //        + ----------- - PITCH
    //               |
    // MOTO1_PWM     |     MOTO1_PWM
    //               -
    //


    if (rc_in->THROTTLE > 1300 && ctl->ARMED && att_in->pit < 40 && att_in->rol < 40 && att_in->pit > -40 && att_in->rol > -40)
    {

        MOTO1_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT + PID_ALT.OUT); //
        MOTO2_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT + PID_ALT.OUT);
        MOTO3_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT + PID_ALT.OUT);
        MOTO4_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT + PID_ALT.OUT);
        //qifei = 1;
    }
    else
    {
        if (qifei == 0) {
            att_in->yaw = 0;
            pit_i = 0;
            rol_i = 0;
            yaw_i = 0;
            alt_i = 0;
            qifei = 0;
            MOTO1_PWM = MOTO2_PWM = MOTO3_PWM = MOTO4_PWM = 0;
        }
        if (qifei == 1 && Alt_ultrasonic > 100 && ctl->ARMED && att_in->pit < 40 && att_in->rol < 40 && att_in->pit > -40 && att_in->rol > -40) {
            p--;
            if (p == 0)
            {down--; p = 5;}
            MOTO1_PWM = (int32_t)(down + PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT); //
            MOTO2_PWM = (int32_t)(down - PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT);
            MOTO3_PWM = (int32_t)(down - PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT);
            MOTO4_PWM = (int32_t)(down + PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT);
        }
        if (qifei == 1 && Alt_ultrasonic < 100)
        {
            qifei = 0;
            MOTO1_PWM = MOTO2_PWM = MOTO3_PWM = MOTO4_PWM = 0; down = 800; ctl->ARMED = 0;
        }
    }
    Moto_PwmRflash(MOTO1_PWM, MOTO2_PWM, MOTO3_PWM, MOTO4_PWM);
}














