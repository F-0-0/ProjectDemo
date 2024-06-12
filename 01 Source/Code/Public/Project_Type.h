/**
 * @file Project_Type.h
 * @brief
 * @author FanLinLin
 * @date 2024-04-28
 */

#ifndef PROJECTTYPE_H
#define PROJECTTYPE_H

#include "Public_IF.h"
// #include "./Flash/Flash_IF.h"

typedef struct // CAN报文
{
    uint16_t id;
    uint8_t len;
    uint8_t buff[8];
} Net_Can_TYPE;

typedef enum IGN_t // 车辆电源状态
{
    IGN_OFF,
    IGN_ACC,
    IGN_ON,
    IGN_START,
    IGN_OFF_TO_ON,
    IGN_ON_TO_OFF,
} IGN_t;

typedef enum Level_t // 电平状态
{
    LOW,
    HIGH,
    LOW_TO_HIGH,
    HIGH_TO_LOW,
} Level_t;

typedef enum Switch_t // 开关状态
{
    SW_IDLE,
    SW_ACTIVE,
    SW_IDLE_TO_ACTIVE,
    SW_ACTIVE_TO_IDLE,
} Switch_t;

typedef enum Door_t // 门状态
{
    DOOR_CLOSE,
    DOOR_OPEN,
    DOOR_CLOSE_TO_OPEN,
    DOOR_OPEN_TO_CLOSE,
} Door_t;

typedef enum LockState_t // 锁状态
{
    LOCK_STATE_LOCK,
    LOCK_STATE_LOCK_TO_UNLOCK,
    LOCK_STATE_UNLOCK_TO_LOCK,
    LOCK_STATE_UNLOCK,
} LockState_t;

typedef enum KeyCMD_t // 按键命令
{
    KEY_IDEL,       // 空闲
    KEY_UNLOCK,     // 解锁
    KEY_LOCK,       // 闭锁
    KEY_TRUNK,      // 开后背箱
    KEY_SEARCH_CAR, // 远程寻车
} KeyCMD_t;

typedef enum LockCMD_t // 锁命令
{
    LOCK_CMD_IDEL,           // 空闲
    LOCK_CMD_UNLOCK,         // 解锁
    LOCK_CMD_LOCK,           // 闭锁
    LOCK_CMD_LOCK_TO_UNLOCK, // 闭锁后解锁
} LockCMD_t;

typedef enum TL_CMD_t // 转向灯命令
{
    TL_CMD_IDEL,
    TL_CMD_LEFT,
    TL_CMD_LEFT_FLASH,
    TL_CMD_RIGHT,
    TL_CMD_RIGHT_FLASH,
    TL_CMD_CRASH,
    TL_CMD_EMERGENCY,
    TL_CMD_LOCK,
    TL_CMD_UNLOCK,
    TL_CMD_SEARCH_CAR,
    TL_CMD_MAX,
} TL_CMD_t;

// clang-format off
typedef enum BuzzerAlarm_t //蜂鸣器报警项
{
    Buzzer_ALARM_OFF                        = 0x0,
    Buzzer_ALARM_FollwoMeLight              = 0x1,
    Buzzer_ALARM_LockSpeedSetOFF            = 0x2,
    Buzzer_ALARM_LockSpeedSetON             = 0x3,
    Buzzer_ALARM_KeyLearnSuccess            = 0x4,
    Buzzer_ALARM_DriverDoorUnlockModeSetOFF = 0x5,
    Buzzer_ALARM_DriverDoorUnlockModeSetON  = 0x6,
    Buzzer_ALARM_LightON                    = 0x10,
    Buzzer_ALARM_RoofWinOpen                = 0x11,
    Buzzer_ALARM_KeyInCarAlarm              = 0x12,
    Buzzer_ALARM_KeyUndetectedAlarm         = 0x13,
    Buzzer_ALARM_ExitActiveArm              = 0x14,
    Buzzer_ALARM_DoorOpenLock               = 0x15,
    Buzzer_ALARM_KeyLowBat                  = 0x16,
    Buzzer_ALARM_PassengerSeatBeltWarning   = 0x17,
    Buzzer_ALARM_DriverSeatBeltWarning      = 0x19,
    Buzzer_ALARM_DoorOpen                   = 0x1B,
    Buzzer_ALARM_PowerManagementNotice      = 0x1C,
} BuzzerAlarm_t;
// clang-format on

typedef enum LockReason_t // 锁状态原因
{
    LOCK_IDLE,                 // 空闲
    LOCK_CENTRAL_UNLOCK,       // 中控解锁
    LOCK_MACHANICAL_UNLOCK,    // 机械钥匙解锁
    LOCK_REMOTE_UNLOCK,        // 远程解锁
    LOCK_PARKING_AUTO_UNLOCK,  // 停车自动解锁
    LOCK_CRASH_UNLOCK,         // 碰撞解锁
    LOCK_BATTERY_FAULT_UNLOCK, // 电池错误解锁

    LOCK_LOCK_FAULT_AFTER_UNLOCK, // 闭锁后解锁

    LOCK_CNETRAL_LOCK,    // 中控闭锁
    LOCK_MACHANICAL_LOCK, // 机械钥匙闭锁
    LOCK_REMOTE_LOCK,     // 远程闭锁
    LOCK_SPEED_LOCK,      // 速度落锁
    LOCK_SECONDARY_LOCK,  // 二次落锁

    LOCK_CNETRAL_LOCK_FAULT,    // 中控闭锁错误
    LOCK_MACHANICAL_LOCK_FAULT, // 机械闭锁错误
    LOCK_REMOTE_LOCK_FAULT,     // 远程闭锁错误

    LOCK_MISSTAKE_LOCK_MAKE_UNLOCK1, // 关闭左前门时（检车到门关信号后0.5S内检测机械闭锁信号）执行一次解锁
    LOCK_MISSTAKE_LOCK_MAKE_UNLOCK2, // 电机驱动闭锁500ms内如有车门（左门或右门）由门关到门开的动作，执行一次解锁。
    LOCK_MISSTAKE_LOCK_MAKE_UNLOCK3, // 如果锁状态为闭锁，检测到车门（左门或右门）由门开变为门全关，执行一次解锁
} LockReason_t;

typedef enum HornAlarm_t // 喇叭报警状态
{
    HORN_ALARM_IDEL,
    HORN_ALARM_NORMAL,
    HORN_ALARM_SEARCH_CAR,
} HornAlarm_t;

#define FLASH_DATA_SIZE_0 496
typedef struct FlashData_t
{
    uint8_t APP_Version[7];                 // 00 size:7   APP版本号
    uint8_t BOOT_Version[7];                // 01 size:7   BOOT版本号
    uint8_t HardwareVersion[7];             // 02 size:7   硬件版本号
    uint8_t BuildTime[21];                  // 03 size:21  编译时间
    uint8_t DownloadTime[21];               // 04 size:21  下载时间
    uint8_t SerialNumber[32];               // 05 size:32  序列号
    uint8_t FlashEraseTimes;                // 06 size:4   Flash擦写次数
    uint8_t ResetReason;                    // 07 size:4   复位原因
    uint8_t Dummy[FLASH_DATA_SIZE_0 - 104]; // 08 size:4   未使用的空间 以上一共213字节
    uint32_t Crc32;                         // 09 size:4   CRC校验
    //! 注意字节对齐，结构体大小为 FLASH_DATA_SIZE
} FlashData_t;

typedef struct Project_InPut_t // 输入信号
{
    Level_t A01_LF_TurnLightFault; // 低有效  左前转向灯故障
    Level_t A02_RF_TurnLightFault; // 低有效  右前转向灯故障
    Level_t A03_Truck_SW;          // 低有效  后背箱开关
    Level_t A04_RR_DoorStatus;     // 低有效  右后门状态
    Level_t A05_Horn_SW;           // 低有效  喇叭开关
    uint8_t A06_CrashSignal;       //! PWM   碰撞信号
    Level_t A07_LF_HandleOpen_SW;  //? 低有效 左前把手伸出到位开关
    Level_t A08_LR_DoorStatus;     // 低有效 左后门状态
    Level_t A09_RightTurnLight_SW; // 低有效 右转向灯开关
    Level_t A10_LF_HandleClose_SW; //? 低有效 左前把手缩回到位开关
    Level_t A11_IG1;               //! 高有效 IG1信号
    Level_t A13_ACC;               //! 高有效 ACC信号
    Level_t A18_CentralUnlock_SW;  // 低有效 中控解锁信号
    Level_t A19_RF_DoorStatus;     // 低有效 右前门状态
    Level_t A20_CentralLock_SW;    // 低有效 中控闭锁信号
    Level_t A21_LF_DoorStatus;     // 低有效 左前门状态
    Level_t A22_PositionLight_SW;  //! 高有效 位置灯开关
    Level_t A23_KeyIn;             //! 高有效 钥匙插入信号
    Level_t A24_Emergency_SW;      // 低有效 危险报警灯开关
    Level_t A25_TruckDoor;         // 低有效 后备箱门状态
    Level_t A26_LeftTurnLight_SW;  // 低有效 左转向灯
    Level_t A27_RF_HandleOpen_SW;  //? 低有效 右前把手伸出到位开关
    Level_t A28_RF_HandleClose_SW; //? 低有效 右前把手缩回到位开关
    uint8_t A29_DriverDoor_SW;     //! AD 驾驶员门解锁状态
    // uint8_t A12_GND; A14_GND; A15_GND; A16_CAN_H; A17_GND; A30_GND; A31_LIN; A32_CAN_L;
} BCM_InPut_t;

typedef struct Project_OutPut_t // 输出信号
{
    Level_t B01_LeftTurnLight;  //! 高有效 左转向灯输出
    Level_t B02_TrunLightPower; // 转向灯电源输入
    Level_t B03_RightTrunLight; //! 高有效 右转向灯输出
    uint8_t B04_DomeLight;      //! PWM    顶灯输出
    Level_t B05_Horn;           //! 高有效 喇叭输出
    Level_t B06_TrunckUnLock;   //! 高有效 后备箱解锁输出
    Level_t B07_HornPower;      // 喇叭电源输入
    Level_t B08_DoorLock;       //? 车门闭锁输出
    Level_t B09_DoorUnLock;     //? 车门解锁输出
    Level_t B10_BCM_Power;      // BCM电源输入
    Level_t B11_ACC_Gear;       // 低有效 ACC档位信号备份输出
    Level_t B12_HandleOut;      // 低有效 门把手伸出输出
    Level_t B14_LeftDayLight;   //! 高有效 左日间行车灯输出
    Level_t B15_RightDayLight;  //! 高有效 右日间行车灯输出
    Level_t B16_HandleIn;       // 低有效 门把手缩回输入
    Level_t B17_LowBeam;        // 近光灯输出
    Level_t B18_LowSideOutput;  // 低边输出(预留)
    Level_t B19_IGNGear;        // IGN档位信号备份输出
    Level_t B20_LockPower;      // 锁电机电源输入
    // uint8_t B13_GND;            //
} BCM_OutPut_t;

typedef struct DoorInfor_t // 车门信息
{
    Door_t Door;
    LockState_t Lock;
    Switch_t Switch;

    uint16_t MotorTime;       // 电机动作时间（ms）
    uint16_t AllowUnlockTime; // 允许解锁时间（ms）
} DoorInfor_t;

typedef struct Project_Infor_t // 项目信息
{
    uint16_t Voltage;     // 电源电压（0.1V）
    uint16_t Current;     // 电源电流（0.1A）
    uint16_t Temperature; // 温度（0.1℃）
    uint64_t RunTime;     // 运行时间（ms）
    FlashData_t FlashData;
} Project_Infor_t;

// clang-format off
typedef union SystemCfg_t // BCM配置信息
{
    struct {
        // byte 0
        uint8_t FollwHome           :3; // 跟随回家功能配置 0x0:Off 0x1:30S 0x2:60S 0x3:120S(默认60s)
        uint8_t TpmsLowPressWarm    :1; // 胎压低压报警设置信号-预留
        uint8_t LockMotoActiveTime  :1; // 锁电机动作持续时间配置 （1：配置为300ms；0：默认200ms）
        uint8_t CrashUnlock         :1; // 碰撞解锁功能          （1：功能关闭；0：功能开启）
        uint8_t RemoteCtrlWindows   :1; // 遥控车窗升降          （1：功能开启；0：功能关闭）
        uint8_t SpeedLock           :1; // 速度感应落锁          （1：功能开启；0：功能关闭）
        // byte 1
        uint8_t Unuesd              :5; // 预留
        uint8_t DoorHand            :1; // 隐藏式把手配置 （0x0:无; 0x1:有）
        uint8_t DayLamp             :1; // 日间行车灯配置 （0x0:无; 0x1:有）
        uint8_t Tpms                :1; // TPMS
    };
    uint8_t byte[2];
}SystemCfg_t;
// clang-format on

extern Project_Infor_t Project_Infor;
extern FlashData_t FlashData;

/**
 * @brief 状态机专用，实现状态翻转功能,Cycle次数到了返回EndToStatus状态
 * @param Status 状态记录变量，static 修饰
 * @param Begin 开始标志位，static 修饰
 * @param StartTime 开始时间记录，static 修饰
 * @param Period 运行周期
 * @param Cycle 运行多少个周期
 * @param EndToStatus 结束到什么状态去
 */
#define Toggle_ms(Status, Begin, StartTime, Period, Cycle, EndToStatus) \
    if (Begin == TRUE)                                                  \
    {                                                                   \
        StartTime = GetSysTick_ms();                                    \
        Begin = FALSE;                                                  \
    }                                                                   \
    if ((GetSysTick_ms() - StartTime) / Period >= Cycle)                \
    {                                                                   \
        Status = EndToStatus;                                           \
        break;                                                          \
    }                                                                   \
    if ((GetSysTick_ms() - StartTime) % Period <= Period / 2)

/**
 * @brief 状态机专用，Begin等于TRUE后保持状态 KeepTime ms
 * @param Begin 开始标志位，static 修饰
 * @param StartTime 开始时间记录，static 修饰
 * @param KeepTime 保持时间
 */
#define Keep_ms(Begin, StartTime, KeepTime) \
    if (Begin == TRUE)                      \
    {                                       \
        StartTime = GetSysTick_ms();        \
        Begin = FALSE;                      \
    }                                       \
    if (GetSysTick_ms() - StartTime <= KeepTime)

/**************************** Factory Mode  ****************************/
extern void Project_InforPrint(void);
extern void Wirte_SN(uint8_t *SN);
extern void Wirte_DownloadTime(uint8_t *Time);
/**************************** Factory Mode  ****************************/

/**************************** Flash Data *******************************/
extern void FlashData_Defualt(void);
extern void FlashData_Init(void);
/**************************** Flash Data  ******************************/
#endif