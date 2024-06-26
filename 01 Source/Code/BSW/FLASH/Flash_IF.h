/**
 * @file Flash_IF.h
 * @author your name (you@domain.com)
 * @brief
 * @VersionID 0.1
 * @date 2024-03-17
 *
 *
 */

#ifndef FLASH_IF_H
#define FLASH_IF_H

#include "../../Public/Project_Type.h"
#include "../../Drivers/Drivers.h"

/******************************** User Configuration ********************************/
// clang-format off
#define PAGE_FLAG0          0x12345678U
#define PAGE_FLAG1          0x87654321U

#define BASE_ADDRESS        0x60000000U // 基准地址
#define PAGE_SIZE           512U        // 页的大小(字节)(最小擦出单元)
#define PAGE_NUMBER         128U        // 页的数量

#define VERSIONID_SIZE      0x4U // 版本号段    数据大小（字节）
#define PAGE_FLAG_SIZE      0x4U // 标志0、1段  数据大小（字节）
#define CRC_SIZE            0x4U // CRC校验段   数据大小（字节）
#define FLASH_DATA_SIZE     (PAGE_SIZE - VERSIONID_SIZE - PAGE_FLAG_SIZE * 2 - CRC_SIZE) // 数据段    数据大小（字节）

#define VERSIONID_BASE      0x0U                                    // 版本号段  起始索引
#define PAGE_FLAG0_BASE     (VERSIONID_BASE + PAGE_FLAG_SIZE)       // 标志0段   起始索引
#define FLASH_DATA_BASE     (PAGE_FLAG0_BASE + PAGE_FLAG_SIZE)      // 数据段    起始索引
#define PAGE_FLAG1_BASE     (PAGE_SIZE - CRC_SIZE - PAGE_FLAG_SIZE) // 标志1段   起始索引
#define PAGE_CRC_BASE       (PAGE_SIZE - CRC_SIZE)                  // CRC校验段 起始索引

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
    uint32_t VersionID; // 页的版本号
    uint8_t IsValid;    // 页是否有效
} PageMap_t;

typedef struct
{
    uint32_t NowAddress;            // 当前地址
    uint32_t PageMapIndex;          // 映射表索引
    uint16_t InvalidPageCnt;        // 无效页数量
    uint8_t IsReady;                //
    uint8_t Buffer[PAGE_SIZE];      // 缓存区
    PageMap_t PageMap[PAGE_NUMBER]; // 映射表
} Flash_t;

/**
 * @brief 初始化Flash框架
 *
 */
void Flash_Init(void);

/**
 * @brief 找上一个有效页，并擦除当前页，以及在查找中遇到的CRC校验失败的页
 *
 * @return uint8_t STD_SUCCESS:成功; STD_ERROR:所有页无效，没有上一页了
 */
uint8_t FlashPreviousPage(void);

/**
 * @brief 缓存区读操作：将Flash的缓存数据拷贝到目标Buffer
 *
 * @param Buffer    目标Buffer
 * @param Size      数据拷贝的大小
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
uint8_t FlashBufferRead(uint8_t *Buffer, uint16_t Size);

/**
 * @brief 缓存区写操作：将目标Buffer数据拷贝到Flash的缓存
 *
 * @param Buffer    目标Buffer
 * @param Size      数据拷贝的大小
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size);

/**
 * @brief 将Flash的缓存数据写入Flash
 *
 * @return uint8_t
 */
uint8_t FlashBufferCommit(void);

uint32_t FlashNowAddress(void);
#endif