/**
 * @file Drivers.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-18
 */

#include "../Public/Public_IF.h"

/******************* UART Funcation *******************/
extern void Dev_UART_Init(void);
extern void Dev_UART_RxBuf_Print(uint8_t instance);
extern char Dev_UART_RxBuf_Pop(uint8_t instance);
extern void Dev_UART_RxBuf_Push(uint8_t instance, char data);
extern uint16_t Dev_UART_RxBuf_Size(uint8_t instance);
/******************* UART Funcation *******************/

/******************* WDT Funcation *******************/
extern void Dev_WDT_Init(void);
extern void Dev_Feed_WDT(void);
/******************* WDT Funcation *******************/

/******************* ADC Funcation *******************/
extern void Dev_ADC_Init(void);
extern void Dev_ADC_Task(void);
extern void Dev_DMA_Task(void);
extern uint16_t Dev_ADC_GetValue(uint8_t instance, uint8_t channel);
/******************* ADC Funcation *******************/

/******************* EPWM Funcation *******************/
extern void Dev_EPWM_Init(void);
extern void Dev_EPWM_ChxEnable(uint32_t instance, uint8_t channelId, uint8_t enable);
extern void Dev_EPWM_SetPeriod(uint32_t instance, uint16_t periodValue);
extern void Dev_EPWM_SetDutyCycle(uint32_t instance, uint8_t channel, uint16_t DutyValue);
extern void Dev_EPWM_SetOutputPolarity(uint32_t instance, uint16_t channelId, uint8_t polarity);
/******************* EPWM Funcation *******************/

/******************* FLash Funcation *******************/
extern void Dev_FLASH_Init(void);
extern void Dev_DFLASH_Erase_All(void);
extern void Dev_DFLASH_Erase(uint32_t Address);
extern void Dev_DFLASH_Read(uint32_t Address, uint8_t *Buffer);
extern void Dev_DFLASH_Write(uint32_t Address, uint8_t *Buffer);
extern uint8_t Dev_DFLASH_IsBusy(void);
extern uint8_t Dev_DFLASH_IsEraseComplete(void);
extern uint8_t Dev_DFLASH_IsWriteComplete(void);
extern uint8_t Dev_DFLASH_IsReadComplete(void);
/******************* FLash Funcation *******************/

/******************* Driver Init Funcation *******************/
extern void Dev_CAN_Init(void);   // 01
extern void Dev_SPI_Init(void);   // 02
extern void Dev_IIC_Init(void);   // 03
extern void Dev_LIN_Init(void);   // 04
extern void Dev_CMP_Init(void);   // 08
extern void Dev_Timer_Init(void); // 09
extern void Dev_RTC_Init(void);   // 10
/******************* Driver Init Funcation *******************/

/******************* System Funcation *******************/
extern void Dev_SystemReset(void);
extern void Dev_SystemConfig(void);
extern void Dev_GPIO_Init(void);
/******************* System Funcation *******************/

/******************* Interrupt Funcation *******************/
extern void Disable_All_IRQ(void); //! 是否支持嵌套
extern void Enable_All_IRQ(void);  //! 是否支持嵌套
extern void Disable_WDT_IRQ(void);
extern void Enable_WDT_IRQ(void);
/******************* Interrupt Funcation *******************/

extern void MCU_Init(void);
