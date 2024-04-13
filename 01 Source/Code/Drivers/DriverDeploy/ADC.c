/**
 * @file ADC.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#define LOG_ENABLE 0

#include "../Drivers.h"

static uint8_t ADC0_ConversionFinishedFlag = 0U;
static uint8_t ADC1_ConversionFinishedFlag = 0U;
static uint8_t DMA0_ConversionFinishedFlag = 0U;
static uint8_t DMA1_ConversionFinishedFlag = 0U;

static uint32_t ADC0_Value[16] = {0};
static uint32_t ADC1_Value[16] = {0};
uint8_t ADC0_channel = 0;
uint8_t ADC1_channel = 0;

void DMA0_Callback(void *parameter, uint8_t status)
{
    if (status == 1)
    {
        DMA0_ConversionFinishedFlag = 1U;
    }
}

void DMA1_Callback(void *parameter, uint8_t status)
{
    if (status == 1)
    {
        DMA1_ConversionFinishedFlag = 1U;
    }
}

void ADC0_Callback(void *parameter, uint8_t status)
{
    if (status == 1)
    {
        ADC0_ConversionFinishedFlag = 1U;
    }
}
void ADC1_Callback(void *parameter, uint8_t status)
{
    if (status == 1)
    {
        ADC1_ConversionFinishedFlag = 1U;
    }
}

void ADC_Init(void)
{
#if 1
    // ADC mode
#else
    // DAM mode
#endif
}

uint16_t ADC0_GetValue(uint8_t channel)
{
    if (channel < 16)
    {
        return ADC0_Value[channel];
    }

    return 0U;
}

uint16_t ADC1_GetValue(uint8_t channel)
{
    if (channel < 16)
    {
        return ADC1_Value[channel];
    }

    return 0U;
}

void ADC_Task(void)
{
}

void DMA_Task(void)
{
}