/**
 * @file Drivers.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef DRIVERS_H
#define DRIVERS_H

#include "../Public/Public_IF.h"

/******************* UART Funcation *******************/
extern void UART_Init(void);
extern void UART0_RxBuf_Print(void);
extern char UART0_RxBuf_Pop(void);
extern void UART0_RxBuf_Push(char data);
extern uint8_t UART0_RxBuf_Size(void);
extern void UART0_CallBack(void *parameter, uint32_t event);
/******************* UART Funcation *******************/

/******************* WDT Funcation *******************/
extern void WDT_Init(void);
extern void WDT_CallBack(void);
/******************* WDT Funcation *******************/

/******************* ADC Funcation *******************/
extern void ADC_Init(void);
extern void ADC_Task(void);
extern void DMA_Task(void);
extern void DMA0_Callback(void *parameter, uint8_t status);
extern void DMA1_Callback(void *parameter, uint8_t status);
extern void ADC0_Callback(void *parameter, uint8_t status);
extern void ADC1_Callback(void *parameter, uint8_t status);
extern uint16_t ADC0_GetValue(uint8_t channel);
extern uint16_t ADC1_GetValue(uint8_t channel);
/******************* ADC Funcation *******************/

/******************* DMA Funcation *******************/
/******************* DMA Funcation *******************/

/******************* Driver Init Funcation *******************/
extern void CAN_Init(void);   // 01
extern void SPI_Init(void);   // 02
extern void IIC_Init(void);   // 03
extern void LIN_Init(void);   // 04
extern void GPIO_Init(void);  // 06
extern void CMP_Init(void);   // 08
extern void Timer_Init(void); // 09
extern void RTC_Init(void);   // 10
extern void PWM_Init(void);   // 11
extern void Flash_Init(void); // 13
/******************* Driver Init Funcation *******************/

/******************* Driver IRQ Funcation *******************/
// clang-format off
#define Disable_All_IRQ()   //! 与普通进行对接 注意中断是否支持嵌套
#define Enable_All_IRQ()    //! 与普通进行对接 注意中断是否支持嵌套

#define Disable_WDT_IRQ()   //! 与普通进行对接 
#define Enable_WDT_IRQ()    //! 与普通进行对接 
#define Feed_WDT()          //! 与普通进行对接 是否启用窗口模式
// clang-format on
/******************* Driver IRQ Funcation *******************/
extern void SystemReset(void);
extern void SystemConfig(void);
extern void Device_Init(void);

#endif