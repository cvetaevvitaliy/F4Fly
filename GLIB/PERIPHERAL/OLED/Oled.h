#ifndef __Oled_H_
#define __Oled_H_
#ifdef __cplusplus
       extern "C" {
#endif

void OLED_Fill(unsigned char fill_Data);
void Oled_Init(void);
void OLED_Fill(unsigned char fill_Data);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
#ifdef __cplusplus
        }
#endif
#endif
