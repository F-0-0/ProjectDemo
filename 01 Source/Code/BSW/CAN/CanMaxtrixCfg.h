/**
 * @file CanMaxtrixCfg.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef CANMAXTRIXCFG_H
#define CANMAXTRIXCFG_H

#include "../../Public/Project_Type.h"
/******************************** User Configuration ********************************/
// clang-format off
#define CAN_TX_CYCLE        (0x0U) // 周期型报文
#define CAN_TX_CYCEV        (0x1U) // 周期/事件型报文

#define CAN_APP             (0x0U) // 应用报文
#define CAN_UDS             (0x1U) // 诊断报文
#define CAN_NM              (0x2U) // 网络管理报文

#define CAN_TX_NUMBER       (0x10U) //! CAN发送数量
#define CAN_RX_NUMBER       (0x10U) //! CAN接收数量
#define RX_BUFFER_SIZE      (0x20U) //! CAN接收Buffer大小
#define RX_BUFFER_DATA_SIZE (0x40U) //! CAN接收Buffer Data大小

#define CAN_TIMER_MAX       (2999U) //! CAN任务计时器最大值（32位,单位ms）
// clang-format on
/******************************** User Configuration ********************************/

typedef void (*CAN_TXRX_CallBack)(uint8_t state);

typedef struct
{
    const uint32_t ID;                // 报文ID
    const uint32_t Peroid;            // 报文周期(ms)
    const uint32_t LostTime;          // 超时时间(ms)
    const uint8_t CanType;            // 应用报文 网络管理报文 诊断报文
    uint8_t Len;                      // 报文数据长度(字节)
    uint32_t LostCunt;                // 报文丢失时间(ms)
    uint8_t IsLost;                   // 报文是否超时 0:未超时 1:超时
    uint8_t *Buffer;                  // 报文Buffer
    CAN_TXRX_CallBack CanRx_CallBack; // CAN报文超时/接收完成后调用
} RxList_t;

typedef struct
{
    const uint32_t ID;                // 报文ID
    const uint32_t Peroid;            // 周期型报文发送周期(ms)
    const uint32_t FastPeroid;        // 事件型报文发送周期(ms)
    const uint8_t Len;                // 报文数据长度(字节)
    const uint8_t TxType;             // CAN_TX_CYCLE:周期型报文； CAN_TX_CYCEV:周期事件型报文
    const uint8_t CanType;            // 应用报文 网络管理报文 诊断报文
    uint8_t TxFlag;                   // 1:可以发送; 0:正在装填数据,不可以发送;
    uint32_t TxDelay;                 // 报文间延迟时间;
    uint32_t TxTimes;                 // 事件型报文发送数据帧数; 需要发送时设置需要发送的数据帧数;
    uint8_t *Buffer;                  // 报文Buffer
    CAN_TXRX_CallBack CanTx_CallBack; // CAN报文发送完成后调用
} TxList_t;

typedef union
{
    struct
    {
    };
    uint8_t data[8];
} CAN_TX_0x050_t;

typedef union
{
    struct
    {
    };
    uint8_t data[8];
} CAN_TX_0x400_t;

typedef union
{
    struct
    {
    };
    uint8_t data[8];
} CAN_RX_0x601_t;

typedef union
{
    struct
    {
    };
    uint8_t data[8];
} CAN_RX_0x700_t;

typedef union
{
    struct
    {
    };
    uint8_t data[8];
} CAN_TX_0x708_t;

extern CAN_TX_0x050_t CAN_TX_0x050;
extern CAN_TX_0x400_t CAN_TX_0x400;
extern CAN_RX_0x601_t CAN_RX_0x601;
extern CAN_RX_0x700_t CAN_RX_0x700;
extern CAN_TX_0x708_t CAN_TX_0x708;

extern TxList_t TxList[CAN_TX_NUMBER]; // Tx列表
extern RxList_t RxList[CAN_RX_NUMBER]; // Rx列表
#endif