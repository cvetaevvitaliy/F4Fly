#ifdef __cplusplus
       extern "C" {
#endif

#include "G32_Timer_Cap.h"
 #include "Rc.h"
extern int RC_flag_ok;
extern u8 RC_data_cnt;
extern u8 *p_RxBuffer;
extern void Data_Receive_Anl(u8 *data_buf, u8 num);
//extern T_RC_Data Rc_Data;//1000~2000
//extern T_RC_Data Rc_D;
//extern T_Control RC_Control;

int task_cap_rc(void)
{
    static u16 Rc_Pwm_In[6];
    u32 temp = 0;
    static u8 capok;
    _SS
    TIM3_Cap_Init(0XFFFF, 72);
    TIM4_Cap_Init(0XFFFF, 72);
    while (1)
    {
        static u8 utime;
        WaitX(25 - utime * 2);
        TIM3_CAPTURE_STA_CH1 = 0;
        TIM3_CAPTURE_STA_CH2 = 0;
        TIM3_CAPTURE_STA_CH3 = 0;
        TIM3_CAPTURE_STA_CH4 = 0;
        capok = 0;
        utime = 0;
        while (1)
        {
            WaitX(2);
            if (TIM4_CAPTURE_STA_CH2 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH2 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH2;
                Rc_Pwm_In[0] = temp;
                capok |= (1 << 0);
            }
            if (TIM3_CAPTURE_STA_CH1 & 0X80)
            {
                temp  = TIM3_CAPTURE_STA_CH1 & 0X3F;
                temp *= 65536;
                temp += TIM3_CAPTURE_VAL_CH1;
                Rc_Pwm_In[0] = temp;
                capok |= (1 << 1);
            }
            if (TIM3_CAPTURE_STA_CH2 & 0X80)
            {
                temp  = TIM3_CAPTURE_STA_CH2 & 0X3F;
                temp *= 65536;
                temp += TIM3_CAPTURE_VAL_CH2;
                Rc_Pwm_In[5] = temp;
                capok |= (1 << 2);
            }
            if (TIM3_CAPTURE_STA_CH3 & 0X80)
            {
                temp  = TIM3_CAPTURE_STA_CH3 & 0X3F;
                temp *= 65536;
                temp += TIM3_CAPTURE_VAL_CH3;
                Rc_Pwm_In[3] = temp;
                capok |= (1 << 3);
            }
            if (TIM3_CAPTURE_STA_CH4 & 0X80)
            {
                temp  = TIM3_CAPTURE_STA_CH4 & 0X3F;
                temp *= 65536;
                temp += TIM3_CAPTURE_VAL_CH4;
                Rc_Pwm_In[4] = temp;
                capok |= (1 << 4);
            }
            if (TIM4_CAPTURE_STA_CH1 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH1 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH1;
                Rc_Pwm_In[5] = temp;
                capok |= (1 << 5);
            }
            utime++;
            if (utime > 2 || capok == 0x1f)
            {
                break;
            }
        }
        Rc_DataAnl(Rc_Pwm_In);
        Rc_GetValue(&Rc_D);
        RC_Analyse(&Rc_D, &RC_Control);
        Send.RCData = 1;
    }
    _EE
}
#ifdef __cplusplus
        }
#endif
