#ifndef _DATA_TRANSFER_H_
#define _DATA_TRANSFER_H_
#include "sys.h"

extern int RuiSaKey;
extern int RuiSaThr;
extern int RuiSaRol;
extern int RuiSaPit;


extern float *RuiSaTime1;
extern float *RuiSaTime2;
extern float *RuiSaTime3;
extern float *RuiSaTime4;
extern float *RuiSaTime5;
extern float *RuiSaTime6;
extern float *RuiSaTime7;
extern float *RuiSaTime8;
extern float *RuiSaTime9;

extern float *RuiSaTh1;
extern float *RuiSaTh2;
extern float *RuiSaTh3;
extern float *RuiSaTh4;
extern float *RuiSaTh5;
extern float *RuiSaTh6;
extern float *RuiSaTh7;
extern float *RuiSaTh8;
extern float *RuiSaTh9;

typedef struct int16_rcget
{
    int16_t ROLL;
    int16_t PITCH;
    int16_t THROTTLE;
    int16_t YAW;
    int16_t AUX1;
    int16_t AUX2;
    int16_t AUX3;
    int16_t AUX4;
    int16_t AUX5;
    int16_t AUX6;
} T_RC_Data;
struct DATA_TRANSFER_SWITCH
{
    char Status;
    char Senser;
    char RCData;
    char GpsData;
    char Offset;
    //char Offset;
    char PID1;
    char PID2;
    char PID3;
    char PID4;
    char PID5;
    char PID6;
    char MotoPwm;
    char DataF1;
    char DataF2;
    char DataF3;
    char DataF4;
};
extern struct DATA_TRANSFER_SWITCH Ex_ON_OFF, Send;
extern u8 Data_Check;
extern void Data_Exchange(void);
extern void Data_Send_Senser(void);
extern void Ex_Init (void);

extern float *yaw_just;
extern float *rol_just;
extern float *pit_just;

extern float *KALMAN_Q ;
extern float *KALMAN_R ;
extern float *KALMAN_P0 ;

#endif

