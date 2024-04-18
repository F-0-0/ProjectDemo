/**
 * @file Drivers.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-18
 */

#include "Drivers.h"

void MCU_Init(void)
{
    Disable_All_IRQ();
    SystemConfig();
    init_cycle_counter(STD_FALSE); // 开始计时
    Dev_WDT_Init();
    Dev_UART_Init();
    // Dev_ADC_Init();
    // Dev_EPWM_Init();
    // Dev_FLASH_Init();
    Enable_All_IRQ();
}