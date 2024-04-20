/**
 * @file Can_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

/*
1、发送节点只允许发送规定长度的报文。
2、接收节点对报文数据长度的处理如下：
    1) 如果节点接收到的报文 DLC 等于预期 DLC，则应按常规方式处理该报文数据；
    2) 如果节点接收到的报文 DLC 大于预期的 DLC，则应按照预期的 DLC 处理该报文的信号；
    3) 如果节点接收到的报文的 DLC 小于预期的 DLC，但该报文的数据场已包含了接收节点预期的所有信号，则接收节点仍需处理所有信号；
    4) 如果节点接收到的报文的 DLC 小于预期的 DLC，但该报文的数据场未包含接收节点预期的所有信号，则接收节点忽略整帧报文，不处理其包含的信号。
3、报文间延迟时间
        延迟时间 tDelay 定义了报文传输后的一段时间，报文发送之后的 tDelay 时间内，不能重复发送有相同标识符的报文，延迟时间和如下条件相关：
    周期和事件型：一个周期发送报文之后的 tDelay 时间内有触发事件发生，或者一个触发事件发生之后的 tDelay 时间内发生了触发事件。
4、信号格式
        信号的位和字节顺序须按照 Motorola LSB 在前格式。即报文的起始为 LSB（最低有效字节）的 lsb（最低有效位），
    以信号的开始位置是报文的最低有效字节的最低有效位。报文中未使用的位须设为 0。相应的，未使用的字节须设为 0x00。
5、报文接收超时处理
        超时监测只在应用报文使用，超时监测在检测到电源由非 ON 档切换为 ON 档的 1000ms或总线睡眠唤醒的 1000ms 后激活，
    且在再次切换到非 ON 档后的 50ms 内或网络重新睡眠后关闭。如果局部监测的报文全部超时，则 ECU 应记录故障码“ECU 接收超时”。
    对报文超时的定义如下：
    1）报文的 5 倍周期时间小于或等于 1s 的，以 1s 作为报文丢失的确认时间。
    2）报文的 5 倍周期时间大于 1s 的，以报文的 5 倍周期时间作为报文丢失的确认时间。
6、报文发送超时处理
        发送方可以选择监测某些重要的周期报文是否被成功发送，发送超时时间为 5 倍报文周期。
    如果连续 5 次周期时间都不能发送成功，且无任何总线错误产生，ECU 可以记录超时的故障码“ECU 发送超时”。
7、Bus-Off 状态处理
        ISO 11898-1 定义了进入、退出 Bus-Off 状态的条件。当“发送错误计数器”累计超过255 时，节点进入 Bus-Off 状态。
    当收到 128 个连续 11 个隐性位，节点由 Bus-Off 状态变为“错误主动状态”，“发送错误寄存器”和“接收错误寄存器”清零。
        当总线进入 Bus-Off 后，不允许 CAN 控制器自动复位。在 Bus-Off 时间后 50ms 时刻，ECU 尝试开始发送报文，
    如果发送成功并且 50ms 内不再进入 Bus-Off，则节点完成 Bus-Off 复位的确认，节点恢复正常通讯。
    备注：
    1. 当“发送错误计数器”累计超过 255 时，节点进入 Bus-Off 状态。
    2. 节点不允许 CAN 控制器自动复位，要求节点进入 Bus-Off 后，每隔 50ms 尝试复位一次，每复位一次复位计数器（Reset Counter）加 1，复位计数器的初始值为 0。
    3. 如果复位计数器等于 10，则节点每 1s 尝试复位一次。
    如果节点成功发送常规报文，则 Bus-Off 复位成功，节点即可正常通讯，如果常规报文尝试发送失败，则节点继续每 1s 尝试一次 Bus-Off 复位。
    4. Busoff Counter 和 Reset Counter 是在 Busoff 后立即同时加 1，但相应的 CAN ControlerReset 发生在 Busoff 的 50ms 后。（前 10 次）
*/

#ifndef CAN_IF_H
#define CAN_IF_H

#include "../../Public/Public_IF.h"
#include "../../Drivers/Drivers.h"
#include "CanMaxtrixCfg.h"

/******************************** User Configuration ********************************/
// clang-format off
#define CAN_RX_RECOVER      (0x0U) // 报文恢复
#define CAN_RX_NORMAL       (0x1U) // 报文接收
#define CAN_RX_LOST         (0x2U) // 报文丢失

#define CAN_TX_ERROR        (0x0U) // 发送失败
#define CAN_TX_SUCCESS      (0x1U) // 发送成功
// clang-format on
/******************************** User Configuration ********************************/

typedef struct
{
    uint32_t ID;                       // 报文ID
    uint8_t Len;                       // 报文数据长度(字节)
    uint8_t Data[RX_BUFFER_DATA_SIZE]; // 报文Buffer
} CAN_Message_t;

typedef struct
{
    TxList_t *TxList;                       // Tx列表
    RxList_t *RxList;                       // Rx列表
    CAN_Message_t RxBuffer[RX_BUFFER_SIZE]; // Rx列表 在接收中断里面更新数据
    uint32_t RxBufferIndex;                 // 接收Buffer的索引
    uint32_t Timer;                         // 时间戳
    uint32_t Task_Peroid;                   // 任务周期
    uint16_t BusOffCnt;                     // Busoff Counter
    uint16_t BusResetCnt;                   // Reset Counter
    uint16_t TxErrorCnt;                    // 发送错误次数
    uint16_t RxErrorCnt;                    // 接收错误次数
    uint8_t Bus_Status;                     // 总线状态
    uint8_t IsReady;                        //
} CanInfor_t;

extern void Can_Task(void);
extern void CAN_Task_Init(TxList_t *TxList, RxList_t *RxList);
extern uint8_t GetIsLost(uint32_t CAN_ID);
extern uint8_t SetTxFlag(uint32_t CAN_ID, uint8_t TxFlag);
#endif