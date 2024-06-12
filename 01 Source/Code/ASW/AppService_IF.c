/**
 * @file AppService_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "AppService_IF.h"

// clang-format off
static TaskInfor_t TaskList[] = {
    //      ID  IsRun       Period  NextTime    RunCnt  Task0_Function                  Parameter
    [0] = { 0,  STD_TRUE,   100,    0,          0,      .Function1 = shellTask,         &shell  },
    [1] = { 1,  STD_TRUE,   20,     0,          0,      .Function0 = Functional_Task,   STD_NULL},
    [2] = { 2,  STD_TRUE,   20,     0,          0,      .Function0 = Idle_Task,         STD_NULL},
};
// clang-format on

void Project_Infor_UpDate(void)
{
    Project_Infor.Voltage = 0;               // 电源电压（0.1V）
    Project_Infor.Current = 0;               // 电源电流（0.1A）
    Project_Infor.Temperature = 0;           // 温度（0.1℃）
    Project_Infor.RunTime = GetSysTick_ms(); // 运行时间（ms）
}

void Service_Init(void)
{
    User_Shell_Init();
    cm_backtrace_init((const char *)Project_Infor.FlashData.SerialNumber,
                      (const char *)Project_Infor.FlashData.APP_Version,
                      (const char *)Project_Infor.FlashData.HardwareVersion);

    Flash_Init();
    FlashBufferRead((uint8_t *)&Project_Infor.FlashData, sizeof(Project_Infor.FlashData));
    // FlashData.ResetReason = SRC_LL_GetResetStateFlag();
}

void Service_Task(void)
{

    for (uint8_t i = 0; i < sizeof(TaskList) / sizeof(TaskList[0]); i++)
    {
        ProjectInfor_UpDate();
        if (Project_Infor.RunTime >= TaskList[i].NextStartTime)
        {
            TaskList[i].NextStartTime = Project_Infor.RunTime + TaskList[i].Period;
            TaskList[i].RunCounter++;
            if (TaskList[i].Parameter != STD_NULL)
            {
                TaskList[i].Function1(TaskList[i].Parameter);
            }
            else
            {
                TaskList[i].Function0();
            }
        }
    }
}

void Idle_Task()
{
    // 空闲任务
    // ADC_Task();
    // DMA_Task();
    // ADC Filter
    // GPIO Filter
}

void Sleep_Task()
{
    while (1)
    {
        // 休眠任务
    }
}