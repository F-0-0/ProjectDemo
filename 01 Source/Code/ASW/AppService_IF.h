/**
 * @file AppService_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef APPSERVICE_IF_H
#define APPSERVICE_IF_H

#include "../Public/Public_IF.h"
#include "../Drivers/Drivers.h"

#include "../BSW/FLASH/Flash_IF.h"
#include "../BSW/CAN/CAN_IF.h"

typedef struct
{
    uint8_t APP_Version[7];               // 00 size:7   APP版本号
    uint8_t BOOT_Version[7];              // 01 size:7   BOOT版本号
    uint8_t HardwareVersion[7];           // 02 size:7   硬件版本号
    uint8_t BuildTime[21];                // 04 size:21  编译时间
    uint8_t DownloadTime[21];             // 05 size:21  下载时间
    uint8_t SerialNumber[32];             // 03 size:32  序列号
    uint8_t FlashEraseTimes;              // 06 size:4   Flash擦写次数
    uint8_t Dummy[FLASH_DATA_SIZE - 103]; // 07 size:4   未使用的空间 以上一共213字节
    uint32_t Crc32;                       // 08 size:4   CRC校验
    //! 注意字节对齐，结构体大小为 FLASH_DATA_SIZE
} FlashData_t;

typedef struct
{
    uint16_t Voltage;     // 电源电压（0.1V）
    uint16_t Current;     // 电源电流（0.1A）
    uint16_t Temperature; // 温度（0.1℃）
    uint64_t RunTime;     // 运行时间（ms）
    FlashData_t FlashData;
} ProjectInfor_t;

typedef void (*Task0_Function)(void);
typedef void (*Task1_Function)(void *);

typedef struct
{
    uint8_t ID;
    uint8_t IsRun;
    uint16_t Period; // ms
    uint64_t NextStartTime;
    uint64_t RunCounter;

    union
    {
        Task0_Function Function0;
        Task1_Function Function1;
    };
    void *Parameter;
} TaskInfor_t;

extern ProjectInfor_t ProjectInfor;

extern void Service_Init(void);
extern void Service_Task(void);
extern void Idle_Task(void);
extern void Sleep_Task(void);

extern void Functional_Task(void);
extern void ProjectInfor_UpDate(void);

extern void FlashData_Defualt(void);
extern void FlashData_Init(void);

#endif