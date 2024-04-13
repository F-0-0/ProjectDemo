/**
 * @file GPIO_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "GPIO_IF.h"

static GPIO_Infor_t GPIO_Infor;

uint8_t SetPin(uint32_t Pin, uint8_t Value)
{
    for (uint32_t i = 0; i < OUTPUT_MAX; i++)
    {
        if (GPIO_Infor.OutPut[i].Pin == Pin)
        {
            // 对接驱动
            return STD_SUCCESS;
        }
    }

    return STD_ERROR;
}

uint32_t GetPin(uint32_t Pin, uint8_t IsFilter)
{
    uint32_t GetPiRresult = STD_ERROR;
    uint32_t ResultValue = 0;

    for (uint16_t i = 0; i < INPUT_MAX; i++)
    {
        if (GPIO_Infor.InPut[i].Pin == Pin)
        {
            if (IsFilter == FILTER)
            {
                ResultValue = GPIO_Infor.InPut[i].FIFO[0];
            }
            else
            {
                ResultValue = GPIO_Infor.InPut[i].Filter_Vlue;
            }
            GetPiRresult = STD_SUCCESS;
        }
    }

    if (GetPiRresult == STD_ERROR)
    {
        for (uint16_t i = 0; i < OUTPUT_MAX; i++)
        {
            if (GPIO_Infor.OutPut[i].Pin == Pin)
            {
                ResultValue = GPIO_Infor.OutPut[i].Value;
                GetPiRresult = STD_SUCCESS;
            }
        }
    }

    return ResultValue;
}

uint8_t TogglePin(uint32_t Pin)
{
    for (uint32_t i = 0; i < OUTPUT_MAX; i++)
    {
        if (GPIO_Infor.OutPut[i].Pin == Pin)
        {
            if (GPIO_Infor.OutPut[i].Value == STD_HIGH)
            {
                // 对接驱动
            }
            else
            {
                // 对接驱动
            }
            return STD_SUCCESS;
        }
    }

    return STD_ERROR;
}

static void Filter(void)
{
    for (uint16_t i = 0; i < INPUT_MAX; i++)
    {
        uint16_t Level_H_Count = 0;
        for (uint16_t j = 0; j < FIFO_SZIE; j++)
        {
            Level_H_Count += GPIO_Infor.InPut[i].FIFO[j];
            GPIO_Infor.InPut[i].FIFO[j + 1] = GPIO_Infor.InPut[i].FIFO[j];
        }
        GPIO_Infor.InPut[i].FIFO[0] = 0; // 对接驱动;
        Level_H_Count += GPIO_Infor.InPut[i].FIFO[0];

        if (Level_H_Count >= FIFO_COUNT)
        {
            GPIO_Infor.InPut[i].Filter_Vlue = STD_HIGH;
        }
        else if (Level_H_Count <= FIFO_SZIE - FIFO_COUNT)
        {
            GPIO_Infor.InPut[i].Filter_Vlue = STD_LOW;
        }
        else
        {
            /*keep last value*/
        }
    }
}

void GPIO_Init(void)
{
    MemorySet((uint8_t *)GPIO_Infor.OutPut, 0, sizeof(GPIO_Infor.OutPut));
    MemorySet((uint8_t *)GPIO_Infor.InPut, 0, sizeof(GPIO_Infor.InPut));
    GPIO_Infor.IsReady = STD_SUCCESS;
}

void GPIO_IDLE_Task(void)
{
}

void GPIO_Task(void)
{
}
