/**
 * @file Can_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "Can_IF.h"

static CanInfor_t CanInfor;

static uint8_t Can_Rx(void);
static uint8_t Can_Tx(void);
static uint8_t Can_Tx_Cycle(uint32_t index);
static uint8_t Can_Tx_Event(uint32_t index);
static uint8_t Can_Tx_CycleEvent(uint32_t index);

static uint8_t Can_Tx_Cycle(uint32_t index)
{
    uint8_t Tx_Status = CAN_TX_SUCCESS;

    if ((CanInfor.TxList[index].TxFlag == 1U) && (CanInfor.Timer % CanInfor.TxList[index].Peroid == 0))
    {
        // uint8_t Tx_Status = CanMsg_Tx(TxList[index].ID, TxList[index].Buffer, TxList[index].Len);
        CanInfor.TxList[index].CanTx_CallBack(Tx_Status);
    }

    if (CanInfor.TxList[index].Peroid == 0)
    {
        /* code */
    }
    return 0;
}

static uint8_t Can_Tx_CycleEvent(uint32_t index)
{
    uint8_t Tx_Status = CAN_TX_SUCCESS;

    if (CanInfor.TxList[index].TxFlag == 1U)
    {
        if ((CanInfor.TxList[index].TxTimes != 0U))
        {
            if (CanInfor.Timer % CanInfor.TxList[index].FastPeroid == 0U)
            {
                // Tx_Status = CanMsg_Tx(CanInfor.TxList[index].ID, CanInfor.TxList[index].Buffer, CanInfor.TxList[index].Len);
                if (Tx_Status == CAN_TX_SUCCESS)
                {
                    CanInfor.TxList[index].TxTimes--;
                }
                CanInfor.TxList[index].CanTx_CallBack(Tx_Status);
            }
        }
        else
        {
            if (CanInfor.Timer % CanInfor.TxList[index].Peroid == 0)
            {
                // Tx_Status = CanMsg_Tx(CanInfor.TxList[index].ID, CanInfor.TxList[index].Buffer, CanInfor.TxList[index].Len);
                CanInfor.TxList[index].CanTx_CallBack(Tx_Status);
            }
        }
    }
    return 0;
}

static uint8_t Can_Rx(void)
{
    uint8_t CanMsgIsLost = 0;

    for (uint32_t i = 0; i < CAN_RX_NUMBER; i++)
    {
        CanMsgIsLost = 0;
        for (uint32_t j = 0; j < RX_BUFFER_SIZE; j++)
        {
            if (CanInfor.RxList[i].ID == CanInfor.RxBuffer[j].ID)
            {
                MemoryCopy(CanInfor.RxList[i].Buffer, CanInfor.RxBuffer[j].Data, CanInfor.RxBuffer[j].Len);
                if (CanInfor.RxList[i].IsLost == CAN_RX_LOST)
                {
                    CanInfor.RxList[i].CanRx_CallBack(CAN_RX_RECOVER);
                }
                else
                {
                    CanInfor.RxList[i].CanRx_CallBack(CAN_RX_NORMAL);
                }

                CanInfor.RxList[i].LostCunt = 0;
                CanInfor.RxList[i].IsLost = CAN_RX_NORMAL;
                CanMsgIsLost = 0;
                break;
            }
        }

        if (CanMsgIsLost == 1 & CanInfor.RxList[i].Peroid != 0)
        {
            if (CanInfor.RxList[i].LostCunt < CanInfor.RxList[i].Peroid)
            {
                CanInfor.RxList[i].LostCunt++;
            }
            else
            {
                CanInfor.RxList[i].IsLost = CAN_RX_LOST;
                CanInfor.RxList[i].CanRx_CallBack(CAN_RX_LOST);
            }
        }
    }

    MemorySet((uint8_t *)CanInfor.RxBuffer, 0, sizeof(CanInfor.RxBuffer)); // 清空接收Buffer
    CanInfor.RxBufferIndex = 0;
    return 0;
}

static uint8_t Can_Tx(void)
{
    for (uint16_t i = 0; i < CAN_TX_NUMBER; i++)
    {
        switch (CanInfor.TxList[i].TxType)
        {
        case CAN_TX_CYCLE:
            Can_Tx_Cycle(i);
            break;
        case CAN_TX_CYCEV:
            Can_Tx_CycleEvent(i);
            break;

        default:
            break;
        }
    }
    return 0;
}

void CAN_Task_Init(TxList_t *TxList, RxList_t *RxList)
{
    MemorySet((uint8_t *)&CanInfor, 0, sizeof(CanInfor));
    CanInfor.TxList = TxList;
    CanInfor.RxList = RxList;
    CanInfor.IsReady = STD_SUCCESS;
}

void Can_Task(void)
{
    // Can_Tx();
    // Can_Rx();
    // CanInfor.Timer += CanInfor.Task_Peroid;
}

uint8_t GetIsLost(uint32_t ID)
{
    for (uint32_t i = 0; i < CAN_RX_NUMBER; i++)
    {
        if (CanInfor.RxList[i].ID == ID)
        {
            return CanInfor.RxList[i].IsLost;
        }
    }
    return 0;
}

uint8_t SetTxFlag(uint32_t ID, uint8_t TxFlag)
{
    for (uint32_t i = 0; i < CAN_TX_NUMBER; i++)
    {
        if (CanInfor.TxList[i].ID == ID)
        {
            return CanInfor.TxList[i].TxFlag;
        }
    }
    return 1;
}