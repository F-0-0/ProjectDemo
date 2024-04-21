/**
 * @file AppService_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "AppService_IF.h"

// clang-format off
const uint8_t SoftwareVersion[] = {SOFTWARE_VERSION   }; // 软件版本号
const uint8_t HardwareVersion[] = {HARDWARE_VERSION   }; // 硬件版本号
const uint8_t BuildTime[]       = {__DATE__" "__TIME__}; // 编译时间
// clang-format on

ProjectInfor_t ProjectInfor;

// clang-format off
static TaskInfor_t TaskList[] = {
    //  ID  IsRun       Period  Task0_Function                  Parameter
    {   0,  STD_TRUE,   100,   .Function1 = shellTask,          &shell  }, // 00
    {   1,  STD_TRUE,   2000,  .Function0 = Functional_Task,    STD_NULL}, // 01
};
// clang-format on

void FlashData_Defualt(void)
{
    // 加载默认值
    MemorySet((uint8_t *)&ProjectInfor.FlashData, 0, sizeof(ProjectInfor.FlashData));
    // clang-format off
    MemoryCopy((uint8_t *)&ProjectInfor.FlashData.APP_Version[0],          (uint8_t *)&SoftwareVersion,      sizeof(ProjectInfor.FlashData.APP_Version));
    MemoryCopy((uint8_t *)&ProjectInfor.FlashData.HardwareVersion[0],      (uint8_t *)&HardwareVersion,      sizeof(ProjectInfor.FlashData.HardwareVersion));
    MemoryCopy((uint8_t *)&ProjectInfor.FlashData.BuildTime[0],            (uint8_t *)&BuildTime,            sizeof(ProjectInfor.FlashData.BuildTime));
    // clang-format on
    ProjectInfor.FlashData.Crc32 = CRC32((uint8_t *)&ProjectInfor.FlashData, sizeof(ProjectInfor.FlashData) - 4);
}

void FlashData_Init(void)
{
    // 初始化
    MemorySet((uint8_t *)&ProjectInfor.FlashData, 0, sizeof(ProjectInfor.FlashData));
    FlashBufferRead((uint8_t *)&ProjectInfor.FlashData, sizeof(ProjectInfor.FlashData));
}

void ProjectInfor_UpDate(void)
{
    ProjectInfor.Voltage = 0;               // 电源电压（0.1V）
    ProjectInfor.Current = 0;               // 电源电流（0.1A）
    ProjectInfor.Temperature = 0;           // 温度（0.1℃）
    ProjectInfor.RunTime = GetSysTick_ms(); // 运行时间（ms）
}

void Service_Init(void)
{
    User_Shell_Init();
    cm_backtrace_init((const char *)ProjectInfor.FlashData.SerialNumber, (const char *)ProjectInfor.FlashData.APP_Version, (const char *)ProjectInfor.FlashData.HardwareVersion);
}

void Service_Task(void)
{
    ProjectInfor_UpDate();
    uint8_t TaskRunCunt = 0;
    for (uint8_t i = 0; i < sizeof(TaskList) / sizeof(TaskList[0]); i++)
    {
        if (TaskList[i].IsRun == STD_TRUE && ProjectInfor.RunTime % TaskList[i].Period == 0)
        {
            TaskRunCunt++;
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

    if (TaskRunCunt == 0)
    {
        Idle_Task();
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