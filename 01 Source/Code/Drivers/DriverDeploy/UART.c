/**
 * @file UART.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "../Drivers.h"

static char UART0_RxBuffer[1024]; // UART0接收数据存储数组
static uint16_t UART0_Index = 0;  // 下一位入队index

void UART0_RxBuf_Print(void)
{
    if (UART0_RxBuf_Size() != 0)
    {
        Printf("\r\n");
    }

    for (uint16_t i = 0; i < UART0_RxBuf_Size(); i++)
    {
        Printf("%x", UART0_RxBuffer[i]);
    }

    if (UART0_RxBuf_Size() != 0)
    {
        Printf("\r\n");
    }
}

uint8_t UART0_RxBuf_Size(void)
{
    return UART0_Index;
}

void UART0_RxBuf_Push(char data)
{
    if (UART0_Index <= sizeof(UART0_RxBuffer))
    {
        UART0_RxBuffer[UART0_Index] = data;
        UART0_Index++;
    }
    else
    {
        // buffer is full
    }
}

char UART0_RxBuf_Pop(void)
{
    char Result = 0;
    if (UART0_Index >= 1)
    {
        Result = UART0_RxBuffer[0];
        UART0_Index--;
        for (uint8_t i = 0; i < UART0_Index; i++)
        {
            UART0_RxBuffer[i] = UART0_RxBuffer[i + 1];
        }
    }
    return Result;
}

void UART0_CallBack(void *parameter, uint32_t event)
{
    char data = 0;
    if (1 == event)
    {
        // data = (char)SCI_LL_GetReceivedData(SCI0);
        UART0_RxBuf_Push(data);
    }
}

void UART_Init(void)
{
}