/**
 * @file UART.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "../../Public/Project_Type.h"

static char Dev_UART_RxBuffer[4][512]; // UART0接收数据存储数组
static uint16_t Dev_UART_Index[4];     // 下一位入队index

uint16_t Dev_UART_RxBuf_Size(uint8_t instance)
{
    return Dev_UART_Index[instance];
}

void Dev_UART_RxBuf_Push(uint8_t instance, char data)
{
    if (Dev_UART_Index[instance] <= sizeof(Dev_UART_RxBuffer[instance]))
    {
        Dev_UART_RxBuffer[instance][Dev_UART_Index[instance]] = data;
        Dev_UART_Index[instance]++;
    }
    else
    {
        // buffer is full
    }
}

char Dev_UART_RxBuf_Pop(uint8_t instance)
{
    char Result = 0;
    if (Dev_UART_Index[instance] >= 1)
    {
        Result = Dev_UART_RxBuffer[instance][0];
        Dev_UART_Index[instance]--;
        for (uint8_t i = 0; i < Dev_UART_Index[instance]; i++)
        {
            Dev_UART_RxBuffer[instance][i] = Dev_UART_RxBuffer[instance][i + 1];
        }
    }
    return Result;
}

void Dev_UART_RxBuf_Print(uint8_t instance)
{
    if (Dev_UART_RxBuf_Size(instance) != 0)
    {
        Printf("\r\n");
    }

    for (uint16_t i = 0; i < Dev_UART_RxBuf_Size(instance); i++)
    {
        Printf("%x", Dev_UART_RxBuffer[instance][i]);
    }

    if (Dev_UART_RxBuf_Size(instance) != 0)
    {
        Printf("\r\n");
    }
}

void Dev_UART0_CallBack(void *parameter, uint32_t event)
{
    char data = 0;
    if (1 == event)
    {
        // data = (char)SCI_LL_GetReceivedData(SCI0);
        Dev_UART_RxBuf_Push(0, data);
    }
}

void Dev_UART_Init(void)
{
}