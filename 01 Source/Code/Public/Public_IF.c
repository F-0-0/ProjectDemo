/**
 * @file Public_IF.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.sizeof(uint8_t)
 * @date 2024-03-06
 *
 *
 */

#include "Public_IF.h"

/******************************  Memory Operation ************************************/

static uint8_t GetSizeTyes(uint32_t Length)
{
    if (Length % sizeof(uint64_t) == 0)
    {
        return sizeof(uint64_t);
    }
    else if (Length % sizeof(uint32_t) == 0)
    {
        return sizeof(uint32_t);
    }
    else if (Length % sizeof(uint16_t) == 0)
    {
        return sizeof(uint16_t);
    }
    return sizeof(uint8_t);
}

void MemoryCopy(uint8_t *Dst, uint8_t *Src, uint32_t Length)
{
    if (Dst != NULL && Src != NULL)
    {
        uint64_t *Dst64 = (uint64_t *)Dst;
        uint64_t *Src64 = (uint64_t *)Src;
        uint32_t *Dst32 = (uint32_t *)Dst;
        uint32_t *Src32 = (uint32_t *)Src;
        uint16_t *Dst16 = (uint16_t *)Dst;
        uint16_t *Src16 = (uint16_t *)Src;
        switch (GetSizeTyes(Length))
        {
        case sizeof(uint64_t): // uint64_t
            while (Length != 0)
            {
                *(Dst64++) = *(Src64++);
                Length -= sizeof(uint64_t);
            }
            break;

        case sizeof(uint32_t): // uint32_t
            while (Length != 0)
            {
                *(Dst32++) = *(Src32++);
                Length -= sizeof(uint32_t);
            }
            break;

        case sizeof(uint16_t): // uint16_t
            while (Length != 0)
            {
                *(Dst16++) = *(Src16++);
                Length -= sizeof(uint16_t);
            }
            break;

        default: // uint8_t
            while (Length--)
            {
                *(Dst++) = *(Src++);
            }
            break;
        }
    }
}

void MemorySet(uint8_t *Dst, uint8_t Data, uint32_t Length)
{
    if (Dst != NULL)
    {
        uint64_t *Dst64 = (uint64_t *)Dst;
        uint32_t *Dst32 = (uint32_t *)Dst;
        uint16_t *Dst16 = (uint16_t *)Dst;
        switch (GetSizeTyes(Length))
        {
        case sizeof(uint64_t): // uint64_t
            while (Length != 0)
            {
                *(Dst64++) = Data;
                Length -= sizeof(uint64_t);
            }
            break;

        case sizeof(uint32_t): // uint32_t
            while (Length != 0)
            {
                *(Dst32++) = Data;
                Length -= sizeof(uint32_t);
            }
            break;

        case sizeof(uint16_t): // uint16_t
            while (Length != 0)
            {
                *(Dst16++) = Data;
                Length -= sizeof(uint16_t);
            }
            break;

        default: // uint8_t
            while (Length--)
            {
                *(Dst++) = Data;
            }
            break;
        }
    }
}

uint8_t MemoryCompare(uint8_t *Src1, uint8_t *Src2, uint32_t Length)
{
    if (Src1 != NULL && Src2 != NULL)
    {
        uint64_t *Src1_64 = (uint64_t *)Src1;
        uint64_t *Src2_64 = (uint64_t *)Src2;
        uint32_t *Src1_32 = (uint32_t *)Src1;
        uint32_t *Src2_32 = (uint32_t *)Src2;
        uint16_t *Src1_16 = (uint16_t *)Src1;
        uint16_t *Src2_16 = (uint16_t *)Src2;
        switch (GetSizeTyes(Length))
        {
        case sizeof(uint64_t): // uint64_t
            while (Length != 0)
            {
                if (*(Src1_64++) != *(Src2_64++))
                {
                    return STD_FALSE;
                }
                Length -= sizeof(uint64_t);
            }
            break;

        case sizeof(uint32_t): // uint32_t
            while (Length != 0)
            {
                if (*(Src1_32++) != *(Src2_32++))
                {
                    return STD_FALSE;
                }
                Length -= sizeof(uint32_t);
            }
            break;

        case sizeof(uint16_t): // uint16_t
            while (Length != 0)
            {

                if (*(Src1_16++) != *(Src2_16++))
                {
                    return STD_FALSE;
                }
                Length -= sizeof(uint16_t);
            }
            break;

        default: // uint8_t
            while (Length--)
            {
                if (*(Src1++) != *(Src2++))
                {
                    return STD_FALSE;
                }
            }
            break;
        }
    }
    return STD_TRUE;
}

/******************************  Memory Operation ************************************/

/******************************     Data Check    ************************************/

uint32_t CRC32(uint8_t *Buffer, uint32_t Length)
{
    uint32_t CRC = 0xFFFFFFFF;

    for (uint32_t i = 0; i < Length; ++i)
    {
        CRC ^= Buffer[i];
        for (uint32_t Bit = 0; Bit < 8; ++Bit)
        {
            if ((CRC & 1) != 0)
            {
                CRC = (CRC >> 1) ^ 0xEDB88320;
            }
            else
            {
                CRC >>= 1;
            }
        }
    }

    return CRC ^ 0xFFFFFFFF;
}

/******************************     Data Check    ************************************/

/******************************       SysTick     ************************************/
int64_t GetSysTick_us(void)
{
    return perfc_convert_ticks_to_us(get_system_ticks());
}

int64_t GetSysTick_ms(void)
{
    return perfc_convert_ticks_to_ms(get_system_ticks());
}

void PrintSysTick(void)
{
    int64_t data = GetSysTick_us();
    uint16_t microsecond = data % 1000;
    uint16_t millisecond = data / 1000 % 1000;
    uint16_t second = data / 1000 / 1000 % 60;
    uint8_t minute = data / 1000 / 1000 / 60 % 60;
    uint8_t hour = data / 1000 / 1000 / 60 / 60 % 24;
    uint64_t day = data / 1000 / 1000 / 60 / 60 / 24;
    logInfo("%lldd%dh%dm%ds%dms%dus ", day, hour, minute, second, millisecond, microsecond);
}
/******************************       SysTick     ************************************/

/******************************      DebugMode    ************************************/
static uint8_t DebugMode = STD_FALSE;

extern uint8_t GetDebugMode(void)
{
    return DebugMode;
}

extern void OpenDebugMode(void)
{
    DebugMode = STD_TRUE;
}

extern void CloseDebugMode(void)
{
    DebugMode = STD_FALSE;
}
/******************************      DebugMode    ************************************/
const char *ResetReason[] = {
    [0] = "Power On Reset",    // 上电复位
    [1] = "Brown Out Reset",   // 掉电复位
    [2] = "External Reset",    // 外部复位
    [3] = "Programming Reset", // 编程复位
    [4] = "Watchdog Reset",    // 看门狗复位
    [5] = "PC Overflow Reset", // PC 地址溢出复位
    [6] = "Software Reset",    // CPU 软件复位
    [7] = "Lockup Reset",      // CPU 锁死复位
};