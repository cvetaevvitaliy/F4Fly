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
#define RC_Yaw_P 20
#define RC_ROL_P 50
#define RC_PIT_P 50


//#define RC_FUN_MIN  1200
//#define RC_FUN_MAX  1800
//#define RC_FUN_CNT  20
//#define RC_FUN_ZERO 1000

static float rol_i = 0, pit_i = 0, yaw_i = 0, alt_i = 0,
	           rol_ni=0,  pit_ni = 0;
PID PID_ROL, PID_PIT, PID_YAW, PID_ALT, PID_POS,
    PID_PID_1, PID_PID_2, PID_PID_3, PID_PID_4,
    PID_PID_5, PID_PID_6, PID_PID_7, PID_PID_8,
    PID_PID_9, PID_PID_10, PID_PID_11, PID_PID_12;

static T_float_angle *att_in;
static S_INT16_XYZ   *gyr_in;
//static S_INT16_XYZ   *acc_in;
static T_RC_Data     *rc_in ;
static T_Control     *ctl;
float     IN_ROP =0.040,    IN_POP =0.040,  IN_YOP =0.045,
          IN_ROI = 0,   IN_POI = 0, IN_YOI =0,
          IN_ROD =1.33,    IN_POD =1.33, IN_YOD =0.012,
          Last_Angle_gx=0,  LAST_Angle_gy=0,LAST_Angle_gz=0,
          Out_PID_X=0,      Out_PID_Y=0,    Out_PID_Z;
int     Angle_gy=0,       Angle_gx=0,     Angle_gz=0; 

//位置偏移////////////////////////////////////////////////////////
float Last_balance_x=0,Last_balance_y=0,Last_v_x=0,Last_v_y=0;
extern float sum_x,sum_y;
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
#define INTEGRAL_WINDUP_Y 300
#define INTEGRAL_WINDUP_A 400
//float Throttle2;// HROTTLE;
u32 Throttle_OUT;
u32 Throttle_IN;

float PID_ALT_OUT_last=0;
int ALT_Control_Out;
extern u8 flag_ALT;extern int v_x,v_y;
int Alt_Error, Alt_Error_Last;
void ALT_Control(u32 ALT_Set)
{
    static u16 Alt_time = 0, flag_time = 0;
    Alt_time++; if (Alt_time > 250)
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
        alt_i += Alt_Error;
        PID_ALT.iout = (PID_ALT.I / 100) * alt_i;
        if (PID_ALT.iout > INTEGRAL_WINDUP_A)
            PID_ALT.iout = INTEGRAL_WINDUP_A;
        else if (alt_i < -INTEGRAL_WINDUP_A)
            PID_ALT.iout = -INTEGRAL_WINDUP_A;
        PID_ALT.dout = -PID_ALT.D * (Alt_Error_Last - Alt_Error) * (1000000 / 10000); // (currenttime - lasttime));

        Alt_Error_Last = Alt_Error;
        //PID_ALT.dout = PID_ALT.D * (acc_in->z-8192);
        //if (ctl->ALT_ON_OFF && Alt_ultrasonic != 0)
        if (Alt_ultrasonic != 0)
        {
            PID_ALT.OUT = PID_ALT.pout + PID_ALT.iout + PID_ALT.dout;
            PID_ALT_OUT_last=PID_ALT.OUT;
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
    PID_YAW.dout = PID_YAW.D * gyr_in->z;
    /*****************************************************
    /PID
    *****************************************************/
    PID_YAW.OUT = PID_YAW.pout  + PID_YAW.dout;
}
void Rol_Control(void)
{ extern u16 Alt_ultrasonic;
	float balance_x=0,v_x,X_PID=0;
balance_x=sum_x-tan(att_in->rol/180.0*3.1416)*(float)Alt_ultrasonic*0.1*0.8;
v_x=balance_x-Last_balance_x;
X_PID=0.20*balance_x+40*v_x;//
Last_balance_x=balance_x;
Last_v_x=v_x;
if(X_PID>5)X_PID=5;if(X_PID<-5)X_PID=-5;
//Sys_Printf(Printf_USART, "m_x:%lf\n",X_PID);//ËÙ¶È
//Sys_Printf(Printf_USART, "v_x:%d\n\n",flow.gyro_x_rate);
 Mag.x=balance_x;

//-------------------外环----------------------------------
	  Angle_gx=-(gyr_in->x);
	angle.rol = att_in->rol+(rc_in->ROLL  - 1660) / RC_ROL_P-X_PID;//

Out_PID_X=PID_ROL.P * angle.rol+PID_ROL.I * rol_i;
//-------------------内环-----------------------------------


PID_ROL.OUT=(Angle_gx*3.5+Out_PID_X)*IN_ROP+(Angle_gx-Last_Angle_gx)*IN_ROD;
Last_Angle_gx=Angle_gx;                                         
if(PID_ROL.OUT>400){PID_ROL.OUT=400;}  
if(PID_ROL.OUT<-400){PID_ROL.OUT=-400;}	
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
//PID_ROL.dout = PID_ROL.D * gyr_in->x;

//    PID_ROL.OUT = PID_ROL.pout + PID_ROL.iout - PID_ROL.dout;
}
void Pit_Control(void)
{ extern u16 Alt_ultrasonic;
	float balance_y=0,v_y,Y_PID=0;
balance_y=sum_y-tan(att_in->pit/180.0*3.1416)*(float)Alt_ultrasonic*0.1*0.8;//
v_y=balance_y-Last_balance_y;
Y_PID=0.20*balance_y+40*v_y;//
Last_balance_y=balance_y;
Last_v_y=v_y;
if(Y_PID>5)Y_PID=5;if(Y_PID<-5)Y_PID=-5;
//Sys_Printf(Printf_USART, "m_y:%lf\n\n",Y_PID);
Mag.y=balance_y;
//------------------外环----------------------------------
		Angle_gy=(gyr_in->y);
	angle.pit = att_in->pit-Y_PID+(rc_in->PITCH - 1650) / RC_PIT_P;//
	
Out_PID_Y=PID_PIT.P * angle.pit;
//-------------------内环-----------------------------------

PID_PIT.OUT=(Angle_gy*3.5 + Out_PID_Y)*IN_POP+(Angle_gy-LAST_Angle_gy)*IN_POD;


LAST_Angle_gy=Angle_gy;
if(PID_PIT.OUT>400){PID_PIT.OUT=400;}  
if(PID_PIT.OUT<-400){PID_PIT.OUT=-400;}	
//--------------------------------------------------------------
//    angle.pit = att_in->pit; //+(*pit_just - 10);//+ (rc_in->PITCH - 1413) / RC_PIT_P
//    pit_i += angle.pit; //积分
//    if (pit_i > INTEGRAL_WINDUP_P)
//        pit_i = INTEGRAL_WINDUP_P;
//    else if (pit_i < -INTEGRAL_WINDUP_P)
//        pit_i = -INTEGRAL_WINDUP_P;	
//		PID_PIT.pout = PID_PIT.P * angle.pit;
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
//	if(rc_in->THROTTLE>1700){Throttle_IN=1700-RC_FUN_ZERO;}
//  else {Throttle_IN = rc_in->THROTTLE - RC_FUN_ZERO;}//油门-1200
	Throttle_IN = rc_in->THROTTLE - RC_FUN_ZERO;
    Balance_Throttle = Throttle_OUT = Throttle_IN;	
	u32 ALT_Set =750 ; // (rc_in->AUX2 - 1100);//定高
	
    Yaw_Control();
    Rol_Control();
    Pit_Control();
    ALT_Control(ALT_Set);

        if (0 == ctl->ALT_ON_OFF)
        {
				#if 0
				static s32 i = 999;
            i++;
            if (1000 == i)
            {
                Balance_Throttle1 = 140;//Balance_Throttle - 40;
            }
            else if (1250 == i)
            {
                Balance_Throttle = 150; //Balance_Throttle-10;
            }
            else if (i > 1000)
            {

                Throttle_OUT += Balance_Throttle1;

            }
            else if (i <= 0)
            {
                i = -2;
            }
            else if (i < 1000)
            {
                Throttle_OUT += PID_ALT.OUT;
            }
						#endif
        }
        else
        {
            Throttle_OUT += PID_ALT.OUT;
        }
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

				
   if (rc_in->THROTTLE > RC_FUN_MIN && ctl->ARMED && att_in->pit < 40 && att_in->rol < 40 && att_in->pit > -40 && att_in->rol > -40)
    {
//        MOTO1_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT);
//        MOTO2_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT);
//        MOTO3_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT);
//        MOTO4_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT);
//			  Sys_Printf(Printf_USART, "m_y:%d ,%d ,%d ,%d \n\n",MOTO1_PWM,MOTO2_PWM,MOTO3_PWM,MOTO4_PWM);
//			#if ROTATE90==0
        MOTO1_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT - PID_PIT.OUT- PID_YAW.OUT);//
        MOTO2_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT - PID_PIT.OUT+ PID_YAW.OUT);
        MOTO3_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT + PID_PIT.OUT- PID_YAW.OUT);
        MOTO4_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT + PID_PIT.OUT+ PID_YAW.OUT);
//#elif ROTATE90==1
//        MOTO1_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT);
//        MOTO2_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT);
//        MOTO3_PWM = (int32_t)((int)Throttle_OUT - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT);
//        MOTO4_PWM = (int32_t)((int)Throttle_OUT + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT);
//#endif
    }
    else
    { att_in->yaw = 0;
        pit_i = 0;
        rol_i = 0;
        yaw_i = 0;
        alt_i = 0;
        MOTO1_PWM = 0;
        MOTO2_PWM = 0;
        MOTO3_PWM = 0;
        MOTO4_PWM = 0;
    }
    Moto_PwmRflash(MOTO1_PWM, MOTO2_PWM, MOTO3_PWM, MOTO4_PWM);
}














