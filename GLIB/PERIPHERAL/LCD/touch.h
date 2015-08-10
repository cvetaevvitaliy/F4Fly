#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
#ifdef __cplusplus
       extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////////////
//������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046�ȣ� ����
//////////////////////////////////////////////////////////////////////////////////
// //�봥����оƬ��������
// #define PEN     PBin (4)//PBin(6)        //PF10 INT
// #define DOUT    PBin (6)//PAin(6)      //PF8  MISO
// #define TDIN    PBout(7)//PAout(7)    //PF9  MOSI
// #define TCLK    PBout(5)//PAout(5)    //PB1  SCLK
// #define TCS     PBout(4)//PBout(7)    //PB2  CS
//#define TOUCH_x 12
//#define TOUCH_y 20

//#define TOUCH(XX,YY) (tp_dev.x>(lcddev.width/TOUCH_x*XX)\
//                      &&tp_dev.x<(lcddev.width/TOUCH_x*(XX+1))\
//                      &&tp_dev.y>(lcddev.height/TOUCH_y*(YY))\
//                      &&tp_dev.y<(lcddev.height/TOUCH_y*(YY+1))\
//                     )

//#define Touch(poix,poiy,width,high) ((320-tp_dev.y)>poix\
//                                     &&(320-tp_dev.y)<(poix+width)\
//                                     &&tp_dev.x>(poiy)\
//                                     &&tp_dev.x<(poiy+high)\
//                                    )


#define TP_PRES_DOWN 0x80  //����������      
#define TP_CATH_PRES 0x40  //�а���������      
//������������
typedef struct
{
    u8 (*init)(void);           //��ʼ��������������
    u8 (*scan)(u8);             //ɨ�败����.0,��Ļɨ��;1,��������;
    void (*adjust)(void);       //������У׼
    u16 x0;                     //ԭʼ����(��һ�ΰ���ʱ������)
    u16 y0;
    u16 x;                      //��ǰ����(�˴�ɨ��ʱ,����������)
    u16 y;
    u8  sta;                    //�ʵ�״̬
    //b7:����1/�ɿ�0;
    //b6:0,û�а�������;1,�а�������.
    ////////////////////////������У׼����/////////////////////////
    float xfac;
    float yfac;
    short xoff;
    short yoff;
    //�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
    //touchtype=0��ʱ��,�ʺ�����ΪX����,����ΪY�����TP.
    //touchtype=1��ʱ��,�ʺ�����ΪY����,����ΪX�����TP.
    u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;        //������������touch.c���涨��

u16 TP_Read_XOY(u8 xy);                         //���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x, u16 *y);                  //˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x, u16 *y);                 //����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color); //��һ������У׼��
void TP_Draw_Big_Point(u16 x, u16 y, u16 color); //��һ�����
u8 TP_Scan(u8 tp);                              //ɨ��
//void TP_Save_Adjdata(void);                     //����У׼����
//u8 TP_Get_Adjdata(void);                        //��ȡУ׼����
void TP_Adjust(void);                           //������У׼
void TP_TestAdjust(void);
u8 TP_Init(void);                               //��ʼ��
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac); //��ʾУ׼��Ϣ



#ifdef __cplusplus
}
#endif

#endif












