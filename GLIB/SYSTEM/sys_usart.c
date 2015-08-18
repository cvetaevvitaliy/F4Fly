#include "sys_usart.h"
#include "usr_usart.h"
#include "data_transfer.h"
#include <stdarg.h>
//static uint8_t SYS_USART_SendData(USART_TypeDef *USARTx, unsigned char DataToSend);
/**********************************************\
 *External Functions                          *
\**********************************************/
//extern void Uart_DataAnl(u8 buf_num);       //串口缓存数据分析
extern void Data_Receive_Anl(u8 *data_buf, u8 num);
/******************************************************
        整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
        radix=10 标示是10进制  非十进制，转换结果为0;
        例：d=-379;
        执行  itoa(d, buf, 10); 后
        buf="-379"
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */
/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
            Data：   发送数组
            ...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
      "\r"  回车符      USART_OUT(USARTx, "abcdefg\r")
            "\n"    换行符      USART_OUT(USARTx, "abcdefg\r\n")
            "%s"    字符串      USART_OUT(USARTx, "字符串是：%s","abcdefg")
            "%d"    十进制      USART_OUT(USARTx, "a=%d",10)
* 调用方法：无
****************************************************************************/
void Sys_Printf(USART_TypeDef *USARTx, char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);
    while (*Data != 0)                                        //判断是否到达字符串结束符
    {
        if (*Data == 0x5c)                                    //'\'
            switch (*++Data)
            {
            case 'r':                                     //回车符
                SYS_USART_SendData(USARTx, 0x0d);
                Data++;
                break;
            case 'n':                                     //换行符
                SYS_USART_SendData(USARTx, 0x0a);
                Data++;
                break;
            default:
                Data++;
                break;
            }
        else if (*Data == '%')
            switch (*++Data)
            {
            case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++)
                {
                    SYS_USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;
            case 'd':                                         //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    SYS_USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;
            default:
                Data++;
                break;
            }
        else SYS_USART_SendData(USARTx, *Data++);
    }
}
/**********************************************\
 *Static variable                             *
\**********************************************/
static u8 TxBuffer[3][0xff];//发送缓存
static u8 TxCount[3] = {0};//发送尾
static u8 TxCounter[3] = {0};//当前发送

void SYS_UART_IQR(USART_TypeDef *USARTx)
{
    if (USARTx->SR & USART_IT_ORE)
    {
        USARTx->SR;
    }
    //发送中断
    if ((USARTx->SR & (1 << 7)) && (USARTx->CR1 & USART_CR1_TXEIE)) //if(USART_GetITStatus(USARTx,USART_IT_TXE)!=RESET)
    {
        int USARTn;
        if (USARTx == USART1)
        {
            USARTn = 0;
        }
        else if (USARTx == USART2)
        {
            USARTn = 1;
        }
        else if (USARTx == USART3)
        {
            USARTn = 2;
        }
        USARTx->DR = TxBuffer[USARTn][TxCounter[USARTn]++]; //写DR清除中断标志
        if (TxCounter[USARTn] == TxCount[USARTn])
            USARTx->CR1 &= ~USART_CR1_TXEIE;        //关闭TXE中断//USART_ITConfig(USARTx,USART_IT_TXE,DISABLE);
    }
    //接收中断 (接收寄存器非空)
    if (USARTx->SR & (1 << 5)) //if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)//
    {
        u8 com_data = USARTx->DR;
        if (USARTx == USART3)
        {
            static u8 RxBuffer[50];
            static u8 RxState = 0;
            static u8 _data_len = 0, _data_cnt = 0;
            if (RxState == 0 && com_data == 0xAA)
            {
                RxState = 1;
                RxBuffer[0] = com_data;
            }
            else if (RxState == 1 && com_data == 0xAF)
            {
                RxState = 2;
                RxBuffer[1] = com_data;
            }
            else if (RxState == 2 && com_data > 0 && com_data < 0XF1)
            {
                RxState = 3;
                RxBuffer[2] = com_data;
            }
            else if (RxState == 3 && com_data < 50)
            {
                RxState = 4;
                RxBuffer[3] = com_data;
                _data_len = com_data;
                _data_cnt = 0;
            }
            else if (RxState == 4 && _data_len > 0)
            {
                _data_len--;
                RxBuffer[4 + _data_cnt++] = com_data;
                if (_data_len == 0)
                    RxState = 5;
            }
            else if (RxState == 5)
            {
                RxState = 0;
                RxBuffer[4 + _data_cnt] = com_data;
                Data_Receive_Anl(RxBuffer, _data_cnt + 5);
            }
            else
                RxState = 0;
            //Sys_Printf(USARTx,"2");
        }
        else if (USARTx == USART1)
        {
            static u8 RxState = 0;
            if (RxState == 0 && com_data == 'R')
            {
                RxState = 1;
            }
            else if (RxState == 1 && com_data > '0' && com_data < '5' )
            {
                RxState = 0;
                RuiSaKey = com_data - '0';
                //Data_Receive_Anl(RxBuffer, _data_cnt + 5);
            }
            else
                RxState = 0;
        }
    }
}
/**************************实现函数********************************************
*******************************************************************************/
uint8_t SYS_USART_SendData(USART_TypeDef *USARTx, unsigned char DataToSend)
{
    int USARTn;
    if (USARTx == USART1)
    {
        USARTn = 0;
    }
    else if (USARTx == USART2)
    {
        USARTn = 1;
    }
    else if (USARTx == USART3)
    {
        USARTn = 2;
    }
    TxBuffer[USARTn][TxCount[USARTn]++] = DataToSend;
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    return DataToSend;
}
int fputc(int ch, FILE *f)
{
    SYS_USART_SendData(Printf_USART, (u8) ch);
    while (USART_GetFlagStatus(Printf_USART, USART_FLAG_TC) == RESET);
    return ch;
}
//uint8_t *Sys_sPrintf(USART_TypeDef *USARTx, unsigned char *DataToSend, unsigned char num)
//{
//    int USARTn;
//    if (USARTx == USART1)
//    {
//        USARTn = 0;
//    }
//    else if (USARTx == USART2)
//    {
//        USARTn = 1;
//    }
//    else if (USARTx == USART3)
//    {
//        USARTn = 2;
//    }
//    for (int i = 0; i < num; i++)
//        TxBuffer[USARTn][TxCount[USARTn]++] = *(DataToSend + i);
//    //if (TxCounter[USARTn] == TxCount[USARTn])
//      USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
//    return DataToSend;
//}
uint8_t *Sys_sPrintf(USART_TypeDef *USARTx, unsigned char *DataToSend, unsigned char num)
{
    int USARTn;
    if (USARTx == USART1)
    {
        USARTn = 0;
    }
    else if (USARTx == USART2)
    {
        USARTn = 1;
    }
    else if (USARTx == USART3)
    {
        USARTn = 2;
    }
    for (int i = 0; i < num; i++)
        TxBuffer[USARTn][TxCount[USARTn]++] = *(DataToSend + i);
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    return DataToSend;
}



