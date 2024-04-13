/**
 * @file Flash_IF.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#ifndef FLASH_IF_H
#define FLASH_IF_H

#include "../../Public/Public_IF.h"

/******************************** User Configuration ********************************/
// clang-format off
#define PAGE_FLAG0          0x1234U
#define PAGE_FLAG1          0x5678U

#define BASE_ADDRESS        0x60000000U // 基准地址
#define PAGE_SIZE           512U        // 页的大小(字节)(最小擦出单元)
#define PAGE_NUMBER         128U        // 页的数量

#define VERSIONID_SIZE      0x4U // 版本号段  数据大小（字节）
#define PAGEFLAG0_SIZE      0x4U // 标志1段   数据大小（字节）
#define PAGEFLAG1_SIZE      0x4U // 标志1段   数据大小（字节）
#define CRC_SIZE            0x4U // CRC校验段 数据大小（字节）
#define FLASH_DATA_SIZE     (PAGE_SIZE - VERSIONID_SIZE - PAGEFLAG0_SIZE - PAGEFLAG1_SIZE - CRC_SIZE) // 数据段    数据大小（字节）


#define VERSIONID_SEGMENT   0x0U            // 版本号段  页的第1、第2、第3、第4个字节
#define PAGE_FLAG0_SEGMENT  0x1U            // 标志1段   页的第5、第6、第7、第8个字节
#define PAGE_FLAG1_SEGMENT  PAGE_SIZE - 2   // 标志1段   页的倒数第5、第6、第7、第8个字节
#define PAGE_CRC_SEGMENT    PAGE_SIZE - 1   // CRC校验段 页的倒数第1、第2、第3、第4个字节

// clang-format on
/******************************** User Configuration ********************************/

/**
 * 将Flash分成5部分
 * 第1部分存 版本号     根据版本号找最新的数据
 * 第2部分存 标志0      用于在不计算CRC的情况下判断快速数据是否有效
 * 第3部分存 数据内容   用于储存数据
 * 第4部分存 标志1      用于在不计算CRC的情况下判断快速数据是否有效
 * 第5部分存 CRC校验    在数据第一次通过简单判断后进行计算CRC，判断数据是否有效
 *
 * uint32_t VersionID;  // 版本号
 * uint32_t Page_Flag0; // 标志0
 * uint8_t *Data;       // 数据内容
 * uint32_t Page_Flag1; // 标志1
 * uint32_t Page_CRC;   // CRC校验
 *
 * 标志0与标志1的计算方法：(VersionID | (NowAddress % PageSize) & PAGE_FLAG);
 */

typedef struct
{
    uint32_t Address;   // 页的地址
    uint16_t VersionID; // 页的版本号
    uint8_t IsValid;    // 页是否有效
    uint8_t IsErase;    // 页是否需要擦除
} PageMap_t;

typedef struct
{
    uint32_t BaseAddress;           // 基准地址
    uint32_t NowAddress;            // 当前地址
    uint32_t PageMapIndex;          // 映射表索引
    uint8_t IsReady;                // 是否准备好
    uint8_t Buffer[PAGE_SIZE];      // 缓存区 外部谨慎操作
    PageMap_t PageMap[PAGE_NUMBER]; // 映射表 外部谨慎操作
} Flash_t;

void Flash_Task(void);
void Flash_Init(void);
uint8_t FlashBufferRead(uint8_t *Buffer, uint16_t Size);
uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size);

#endif