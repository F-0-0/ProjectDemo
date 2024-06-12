/**
 * @file AppService_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef APPSERVICE_IF_H
#define APPSERVICE_IF_H

#include "../Public/Project_Type.h"
#include "../Drivers/Drivers.h"

#include "../BSW/FLASH/Flash_IF.h"
#include "../BSW/CAN/CAN_IF.h"

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

extern void Service_Init(void);
extern void Service_Task(void);
extern void Idle_Task(void);
extern void Sleep_Task(void);

extern void Functional_Task(void);
extern void ProjectInfor_UpDate(void);

#endif