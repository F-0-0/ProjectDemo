/**
 * @file System.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-18
 */

#include "../../Public/Project_Type.h"

__attribute__((used)) void SysTick_Handler(void)
{
}

void Dev_SystemReset(void)
{
}

void Dev_SystemConfig(void)
{
}

ResetReason_t Dev_GetResetReasn(void)
{
    ResetReason_t ResetReasonIndex = RESET_POWER_ON;
    return ResetReasonIndex;
}

void Dev_GPIO_Config(void)
{
}
