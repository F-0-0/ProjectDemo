/**
 * @file GPIO_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef GPIO_IF_H
#define GPIO_IF_H

#include "../../Public/Public_IF.h"

/******************************** User Configuration ********************************/
// clang-format off
#define FIFO_SZIE       10U
#define FIFO_COUNT      8U

#define FILTER          0U
#define UNFILTER        1U

typedef enum
{
    INPUT_MAX,
}INPUT;

typedef enum
{
    OUTPUT_MAX,
}OUTPUT;

// clang-format on
/******************************** User Configuration ********************************/
typedef struct
{
    uint8_t Pin;
    uint8_t FIFO[FIFO_SZIE];
    uint8_t Filter_Vlue;
    uint8_t Level_H_Cnt;
    uint8_t Level_L_Cnt;
    const uint8_t Valid_Value;
} InPut_t;

typedef struct
{
    uint8_t Pin;
    uint8_t Value;
} OutPut_t;

typedef struct
{
    uint8_t IsReady;
    InPut_t InPut[INPUT_MAX];
    OutPut_t OutPut[OUTPUT_MAX];
} GPIO_Infor_t;

void GPIO_Init(void);
void GPIO_Task(void);
void GPIO_IDLE_Task(void);
uint8_t SetPin(uint32_t Pin, uint8_t Value);
uint32_t GetPin(uint32_t Pin, uint8_t IsFilter);
uint8_t TogglePin(uint32_t Pin);

#endif