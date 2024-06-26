/**
 * @file Public_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef PUBLIC_IF_H
#define PUBLIC_IF_H

#include "Public_Type.h"
#include <stdio.h>
#include "./perf_counter/perf_counter.h"
#include "./cm_backtrace/cm_backtrace.h"
#include "./LetterShell/log.h"
#include "./LetterShell/shell.h"
#include "./LetterShell/shell_port.h"

/************************* APP  Config *************************/
#define SOFTWARE_VERSION "V1.0.0" // software_version
#define HARDWARE_VERSION "V1.0.0" // hardware_version

/************************* APP Config *************************/

/************************* APP  Funcation *************************/
extern void MemoryCopy(uint8_t *Dst, uint8_t *Src, uint32_t Size);
extern void MemorySet(uint8_t *Dst, uint8_t Data, uint32_t Size);
extern uint8_t MemoryCompare(uint8_t *Src1, uint8_t *Src2, uint32_t Size);
extern uint32_t CRC32(uint8_t *Buffer, uint32_t Length);
extern int64_t GetSysTick_ms(void);
extern int64_t GetSysTick_us(void);
extern void delay_ms(int32_t ms);
extern void delay_us(int32_t us);
extern void PrintSysTick(void);

extern uint8_t GetDebugMode(void);
extern void OpenDebugMode(void);
extern void CloseDebugMode(void);

typedef enum ResetReason_t
{
    RESET_POWER_ON,    // 上电复位
    RESET_BROWN_OUT,   // 掉电复位
    RESET_EXTERNAL,    // 外部复位
    RESET_PROGRAMMING, // 编程复位
    RESET_WATCHDOG,    // 看门狗复位
    RESET_PC_OVERFLOW, // PC 地址溢出复位
    RESET_SOFTWARE,    // CPU 软件复位
    RESET_LOCKUP,      // CPU 锁死复位
} ResetReason_t;

extern const char *ResetReason[];

#define Printf(format, ...)            \
    if (GetDebugMode() == STD_TRUE)    \
    {                                  \
        printf(format, ##__VA_ARGS__); \
    }                                  \
    else                               \
    {                                  \
    }
/************************* APP  Funcation *************************/

#endif
