/**
 * @file Drivers.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "Drivers.h"

/**
 * @brief printf 函数从定向到串口
 *
 * @param ch
 * @param f
 * @return int
 */
int fputc(int ch, FILE *f)
{
    // while ((SCI0->S1 & 0x40U) == 0U)
    //     ; /* Judge whether the serial port is sent */

    // SCI0->D = (uint8_t)ch;

    return ch;
}

__attribute__((used)) void SysTick_Handler(void)
{
}

extern void SystemConfig(void)
{
    // SRC_DRV_ClockConfig(&clock_config, &clock_state);
    // SystemCoreClockUpdate();   // 更新CPU频率，用于计时
    init_cycle_counter(STD_FALSE); // 开始计时
}

void Device_Init(void)
{
    Disable_All_IRQ();
    SystemConfig();
    WDT_Init();
    UART_Init();
    ADC_Init();
    Enable_All_IRQ();
}