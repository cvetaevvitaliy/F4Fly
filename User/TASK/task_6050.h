#ifdef __cplusplus
       extern "C" {
#endif
/*
    Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
    R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
*/

int flag_ACC = 0;
int task_6050(void)
{
    extern S_INT16_XYZ Acc, Gyr, Mag, Average_Acc,MPU6050_GYRO_Gr;
    extern S_INT16_XYZ Average2_Acc, Average3_Acc, Average1_Gyr, Average2_Gyr;

    _SS
    MPU6050_Init();

    WaitX(200);
    while (1)
    {
        WaitX(1);
        MPU6050_Read();
        MPU6050_Dataanl();//5us

        Acc.x = MPU6050_ACC_LAST.x;
        Acc.y = MPU6050_ACC_LAST.y;
        Acc.z = MPU6050_ACC_LAST.z;
        Gyr.x = MPU6050_GYRO_LAST.x;
        Gyr.y = MPU6050_GYRO_LAST.y;
        Gyr.z = MPU6050_GYRO_LAST.z;//低通防止静态误差

        Prepare_Data(&Acc, &Average_Acc);//滤波4us
        flag_ACC = 1;

    }
    _EE
}
#ifdef __cplusplus
        }
#endif
