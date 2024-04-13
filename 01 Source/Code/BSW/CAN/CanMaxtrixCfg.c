/**
 * @file CanMaxtrixCfg.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "CanMaxtrixCfg.h"

void CanTx_CallBack_0x050(uint8_t state)
{
}

void CanRx_CallBack_0x601(uint8_t state)
{
}

void CanTx_CallBack_0x400(uint8_t state)
{
}

CAN_TX_0x050_t CAN_TX_0x050;
CAN_RX_0x601_t CAN_RX_0x601;

CAN_TX_0x400_t CAN_TX_0x400;

CAN_RX_0x700_t CAN_RX_0x700;
CAN_TX_0x708_t CAN_TX_0x708;

// clang-format off
TxList_t TxList[CAN_TX_NUMBER] = {
    // ID       Peroid  FastPeroid  Len     TxType          CanType     TxFlag  TxDelay TxTimes Buffer              CanTx_CallBack      
    { 0x050,    100,    0,          8,      CAN_TX_CYCLE,   CAN_UDS,    0,      0,      0,      CAN_TX_0x050.data,  CanTx_CallBack_0x050},
};

RxList_t RxList[CAN_RX_NUMBER] = {
    //  ID      Peroid  LostTime    CanType     Len LostCunt    IsLost  Buffer              CanRx_CallBack      
    { 0x050,    100,    1000,       CAN_UDS,    8,  0,          FALSE,  CAN_RX_0x601.data,  CanRx_CallBack_0x601},
};
// clang-format on