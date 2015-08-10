#include "bak.h"
#include "stmflash.h"
#include "control.h"
#include "Balance.h"
#include "stm32f10x_bkp.h"


#define SIZE 21
#define FLASH_SAVE_ADDR  0X08035000      //设置FLASH 保存地址(必须为偶数)

void StmFlash_Save(void)
{
    u16 StmFlash_Buffer[SIZE];
    int i = 0;
    StmFlash_Buffer[i++] = (u16)(PID_ROL.P   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_ROL.I   * 1000);
    StmFlash_Buffer[i++] = (u16)(PID_ROL.D   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_PIT.P   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_PIT.I   * 1000);
    StmFlash_Buffer[i++] = (u16)(PID_PIT.D   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_YAW.P   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_YAW.I   * 1000);
    StmFlash_Buffer[i++] = (u16)(PID_YAW.D   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_ALT.P   * 1000);
    StmFlash_Buffer[i++] = (u16)(PID_ALT.I   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_ALT.D   * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_PID_1.P * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_PID_1.I * 100 );
    StmFlash_Buffer[i++] = (u16)(PID_PID_1.D * 100 );
    StmFlash_Buffer[i++] = (u16)(ACC_OFFSET.x      );
    StmFlash_Buffer[i++] = (u16)(ACC_OFFSET.y      );
    StmFlash_Buffer[i++] = (u16)(ACC_OFFSET.z      );
    StmFlash_Buffer[i++] = (u16)(GYRO_OFFSET.x     );
    StmFlash_Buffer[i++] = (u16)(GYRO_OFFSET.y     );
    StmFlash_Buffer[i++] = (u16)(GYRO_OFFSET.z     );

    STMFLASH_Write(FLASH_SAVE_ADDR, (u16 *)StmFlash_Buffer, SIZE);
}
void StmFlash_Read(void)
{
    u16 StmFlash_Buffer[SIZE];
    int i = 0;
    STMFLASH_Read(FLASH_SAVE_ADDR, (u16 *)StmFlash_Buffer, SIZE);
    PID_ROL.P  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_ROL.I  =  ((float)(StmFlash_Buffer[i++]) / 1000 );
    PID_ROL.D  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_PIT.P  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_PIT.I  =  ((float)(StmFlash_Buffer[i++]) / 1000 );
    PID_PIT.D  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_YAW.P  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_YAW.I  =  ((float)(StmFlash_Buffer[i++]) / 1000 );
    PID_YAW.D  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_ALT.P  =  ((float)(StmFlash_Buffer[i++]) / 1000 );
    PID_ALT.I  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_ALT.D  =  ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_PID_1.P = ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_PID_1.I = ((float)(StmFlash_Buffer[i++]) / 100  );
    PID_PID_1.D = ((float)(StmFlash_Buffer[i++]) / 100  );
    ACC_OFFSET.x  =  ((u8)(StmFlash_Buffer[i++])        );
    ACC_OFFSET.y  =  ((u8)(StmFlash_Buffer[i++])        );
    ACC_OFFSET.z  =  ((u8)(StmFlash_Buffer[i++])        );
    GYRO_OFFSET.x =  ((u8)(StmFlash_Buffer[i++])        );
    GYRO_OFFSET.y =  ((u8)(StmFlash_Buffer[i++])        );
    GYRO_OFFSET.z =  ((u8)(StmFlash_Buffer[i++])        );
}

