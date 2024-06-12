/**
 * @file Project_Type.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-05-06
 */

#include "Project_Type.h"

// clang-format off
// clang-format off
const uint8_t SoftwareVersion[] = {SOFTWARE_VERSION   }; // 软件版本号
const uint8_t HardwareVersion[] = {HARDWARE_VERSION   }; // 硬件版本号
const uint8_t BuildTime[]       = {__DATE__" "__TIME__}; // 编译时间
// clang-format on
// clang-format on

FlashData_t FlashData;
SystemCfg_t SystemCfg;
Project_Infor_t Project_Infor;

void FlashData_Defualt(void)
{
    // 加载默认值
    MemorySet((uint8_t *)&Project_Infor.FlashData, 0, sizeof(Project_Infor.FlashData));
    // clang-format off
    MemoryCopy((uint8_t *)&Project_Infor.FlashData.APP_Version[0],          (uint8_t *)&SoftwareVersion,      sizeof(Project_Infor.FlashData.APP_Version));
    MemoryCopy((uint8_t *)&Project_Infor.FlashData.HardwareVersion[0],      (uint8_t *)&HardwareVersion,      sizeof(Project_Infor.FlashData.HardwareVersion));
    MemoryCopy((uint8_t *)&Project_Infor.FlashData.BuildTime[0],            (uint8_t *)&BuildTime,            sizeof(Project_Infor.FlashData.BuildTime));
    // clang-format on
    Project_Infor.FlashData.Crc32 = CRC32((uint8_t *)&Project_Infor.FlashData, sizeof(Project_Infor.FlashData) - 4);
}

void FlashData_Init(void)
{
    // 初始化
    MemorySet((uint8_t *)&FlashData, 0, sizeof(FlashData));
    // FlashBufferRead((uint8_t *)&FlashData, sizeof(FlashData));
}

/********************************************** Factory Mode  **********************************************/

extern uint32_t FlashNowAddress(void);
extern uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size);
extern uint8_t FlashBufferCommit(void);
void Project_InforPrint()
{
    Printf("\r\n");
    Printf("BCM Product Information\r\n");
    PrintSysTick();
    Printf("\r\n");
    Printf("Reset Reason:       %s\r\n", ResetReason[FlashData.ResetReason]);
    Printf("Battery Voltage:    %0.3fV\r\n", Project_Infor.Voltage * 0.001f);
    Printf("APP Version:        %s\r\n", FlashData.APP_Version);
    Printf("Boot Version:       %s\r\n", FlashData.BOOT_Version);
    Printf("Hard Version:       %s\r\n", FlashData.HardwareVersion);
    Printf("Build Time:         %s\r\n", FlashData.BuildTime);
    Printf("Download Time:      %s\r\n", FlashData.DownloadTime);
    Printf("Product SN:         %s\r\n", FlashData.SerialNumber);
    Printf("Flash Adress:       0x%X\r\n", FlashNowAddress());
    Printf("Flash Erase Time:   %d\r\n", *(uint32_t *)(&FlashData.FlashEraseTimes));
}

void Wirte_SN(uint8_t *SN)
{
    uint16_t Len = 0;
    MemoryCopy(FlashData.SerialNumber, SN, sizeof(FlashData.SerialNumber));
    (*(uint32_t *)(&FlashData.FlashEraseTimes))++;
    FlashBufferWrite((uint8_t *)&FlashData, sizeof(FlashData));
    FlashBufferCommit();
}

void Wirte_DownloadTime(uint8_t *Time)
{
    uint16_t Len = 0;
    MemoryCopy(FlashData.DownloadTime, Time, sizeof(FlashData.DownloadTime));
    (*(uint32_t *)(&FlashData.FlashEraseTimes))++;
    FlashBufferWrite((uint8_t *)&FlashData, sizeof(FlashData));
    FlashBufferCommit();
}

/********************************************** Factory Mode  **********************************************/