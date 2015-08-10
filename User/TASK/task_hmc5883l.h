#ifdef __cplusplus
       extern "C" {
#endif
#include "HMC5883L.h"
#define PI 3.14159265
#define N 5
u16 i=0,k,j,t=0;
float mid[5];
float emp;
int task_hmc5883l(void)
{
    extern S_INT16_XYZ rollt;
		extern float compass_yaw;
    _SS
    HMC5883L_Init();
    HMC5883L_Calibrate();
    WaitX(20);
    while (1)
    {
        HMC5883L_Start();//7us
        WaitX(10);
        /**********430us*************/
        HMC5883L_MultRead(&hmc5883l);//8us
        //HMC5883L_Printf(&hmc5883l);
        rollt.x = hmc5883l.hx;
        rollt.y = hmc5883l.hy;
        rollt.z = hmc5883l.hz;
        {
            //From APM
            float angle1, angle2,
                  sin_angle1, sin_angle2,
                  cos_angle1, cos_angle2,
                  Xr, Yr;

            angle1 = (Att_Angle.rol + 180) / 180 * PI;
            angle2 = (-Att_Angle.pit + 180) / 180 * PI;
            sin_angle1 = sin(angle1);
            cos_angle1 = cos(angle1);
            sin_angle2 = sin(angle2);
            cos_angle2 = cos(angle2);
						
            Xr = rollt.x * cos_angle2 * cos_angle2 + rollt.y * sin_angle2 * sin_angle1 - rollt.z * cos_angle1 * sin_angle2;
            Yr = rollt.y * cos_angle1 + rollt.z * sin_angle1;
            mid[i] = atan2(Yr, Xr) * (180 / PI); //160us
//////////////////////////////////////////////////////////////////						
				if(mid[i]!=0)i++;
    if(i==N)
	{i=0;
  for (j=0;j<N;j++)
{
 for (k=0;k<N;k++)
{
if(mid[k]>mid[k+1])
{
emp= mid[k];
mid[k] = mid[k+1];
mid[k+1] =emp;
}}}
compass_yaw=mid[2];}
///////////////////////////////////////////////////
        }
        WaitX(10);
    }
    _EE
}
#ifdef __cplusplus
        }
#endif
