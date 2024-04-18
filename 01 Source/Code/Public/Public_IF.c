/**
 * @file Public_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "Public_IF.h"

/******************************  Memory Operation ************************************/

void MemoryCopy(uint8_t *Dst, uint8_t *Src, uint32_t Length)
{
    if (Dst != NULL && Src != NULL)
    {
        while (Length--)
        {
            *(Dst++) = *(Src++);
        }
    }
}

void MemorySet(uint8_t *Dst, uint8_t Data, uint32_t Length)
{
    if (Dst != NULL)
    {
        while (Length--)
        {
            *(Dst++) = Data;
        }
    }
}

uint8_t MemoryCompare(uint8_t *Src1, uint8_t *Src2, uint32_t Length)
{
    if (Src1 != NULL && Src2 != NULL)
    {
        while (Length--)
        {
            if (*(Src1++) != *(Src2++))
            {
                return STD_FALSE;
            }
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
uint64_t GetSysTick_us(void)
{
    return (uint64_t)perfc_convert_ticks_to_us(get_system_ticks());
}

uint64_t GetSysTick_ms(void)
{
    return (uint64_t)perfc_convert_ticks_to_ms(get_system_ticks());
}

void PrintSysTick(void)
{
    uint64_t data = (uint64_t)GetSysTick_us();
    uint16_t microsecond = data % 1000;
    uint16_t millisecond = data / 1000 % 1000;
    uint16_t second = data / 1000 / 1000 % 60;
    uint8_t minute = data / 1000 / 1000 / 60 % 60;
    uint8_t hour = data / 1000 / 1000 / 60 / 60 % 24;
    uint64_t day = data / 1000 / 1000 / 60 / 60 / 24;
    logInfo("%lldd%dh%dm%ds%dms%dus ", day, hour, minute, second, millisecond, microsecond);
}
/******************************       SysTick     ************************************/