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

SHELL_EXPORT_USER(SHELL_CMD_PERMISSION(1), Admin, Admin, 管理员);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, open, OpenDebugMode, Open Debug Mode);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, close, CloseDebugMode, Close Debug Mode);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, sysTick, PrintSysTick, Printf SysTick);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, reset, Dev_SystemReset, Reset MCU);
// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, Product, BCM_Infor, BCM Product Information);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), FlashBufferCommit, FlashBufferCommit, Write Information To Flash);

// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(1) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, Wirte_DownloadTime, Wirte_DownloadTime, Product Download Time Wirte, .data.cmd.signature = "s");
// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(1) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, Wirte_SN, Wirte_SN, Product SN Wirte, .data.cmd.signature = "s");
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(1) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinToggle, Dev_GPIO_Toggle, Pin Toggle);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(1) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinSetLow, Dev_GPIO_SetLow, Pin Set Low);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(1) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC) | SHELL_CMD_DISABLE_RETURN, PinSetHigh, Dev_GPIO_SetHigh, Pin Set High);

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
            Dev_SystemReset();
            break;
        }
    }
    return 0;
}
