/**
 * @file main.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "./ASW/AppService_IF.h"

typedef enum
{
    MCU_NORMAL,
    MCU_SLEEP,
    MCU_IDLE,
} MCU_STATUS_t;

MCU_STATUS_t MCU_STATUS = MCU_NORMAL;

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, Reset, Dev_SystemReset, Reset MCU);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, SysTick, PrintSysTick, Printf SysTick);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinToggle, Dev_GPIO_Toggle, Pin Toggle);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinSetLow, Dev_GPIO_SetLow, Pin Set Low);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinSetHigh, Dev_GPIO_SetHigh, Pin Set High);

int main(void)
{
    // 如果从bootloader启动，需要关闭看门狗等中断，
    Disable_All_IRQ();
    MCU_Init();
    Service_Init();
    Enable_All_IRQ();
    PrintSysTick();
    while (1)
    {
        switch (MCU_STATUS)
        {
        case MCU_NORMAL:
            Service_Task();
            Dev_Feed_WDT();
            break;

        case MCU_SLEEP:
            Sleep_Task();
            break;

        default:
            MCU_STATUS = MCU_NORMAL;
            break;
        }
    }
    return 0;
}
