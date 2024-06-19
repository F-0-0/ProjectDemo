/**
 * @file Drivers.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-18
 */

#include "Drivers.h"

/**
 * @brief printf 函数重定向到串口
 */
int fputc(int ch, FILE *f)
{
    static int64_t TimeOut;
    if (GetDebugMode() == STD_TRUE)
    {
        if ((GetSysTick_ms() - TimeOut) < (TIME_1M * 10))
        {
            // while ((SCI3->S1 & 0x40U) == 0U)
            //     ; /* Judge whether the serial port is sent */
            // SCI3->D = (uint8_t)ch;
        }
        else
        {
            CloseDebugMode();
        }
    }
    else
    {
        TimeOut = GetSysTick_ms();
    }
    return ch;
}

void MCU_Init(void)
{
    Disable_All_IRQ();
    Dev_SystemConfig();
    init_cycle_counter(STD_FALSE); // 开始计时
    Dev_WDT_Init();
    Dev_UART_Init();
    // Dev_ADC_Init();
    // Dev_EPWM_Init();
    // Dev_FLASH_Init();
    Enable_All_IRQ();
}