/**
 * @file Flash_IF.c
 * @author your name (you@domain.com)
 * @brief
 * @VersionID 0.1
 * @date 2024-03-17
 *
 *
 */

#include "Flash_IF.h"

#define Page0FlagCalculate(VersionID, PageAddress) (VersionID ^ PAGE_FLAG0 + PageAddress ^ PAGE_SIZE + PAGE_FLAG0 + PAGE_FLAG0)
#define Page1FlagCalculate(VersionID, PageAddress) (VersionID ^ PAGE_FLAG1 + PageAddress ^ PAGE_SIZE + PAGE_FLAG1 + PAGE_FLAG1)

static Flash_t Flash;

static uint16_t PageSearch(void);
static uint8_t PageVerify(void);
static void FlashUpDate_FrameInfor(void);

/**
 * @brief 寻找所有的有效页
 *
 * @return uint16_t  无效页数量
 */
static uint16_t PageSearch(void)
{
    uint32_t PageAddress = 0;
    uint32_t VersionID = 0;
    uint32_t Page_Flag0 = 0;
    uint32_t Page_Flag1 = 0;
    uint8_t FindFlag = STD_FALSE;
    for (uint16_t i = 0; i < PAGE_NUMBER; i++)
    {
        PageAddress = BASE_ADDRESS + i * PAGE_SIZE;

        MemoryCopy((uint8_t *)&VersionID, (uint8_t *)(PageAddress + VERSIONID_BASE), VERSIONID_SIZE);
        MemoryCopy((uint8_t *)&Page_Flag0, (uint8_t *)(PageAddress + PAGE_FLAG0_BASE), PAGE_FLAG_SIZE);
        MemoryCopy((uint8_t *)&Page_Flag1, (uint8_t *)(PageAddress + PAGE_FLAG1_BASE), PAGE_FLAG_SIZE);

        if (Page_Flag0 == Page0FlagCalculate(VersionID, PageAddress) &&
            Page_Flag1 == Page1FlagCalculate(VersionID, PageAddress))
        {
            Flash.PageMap[i].Address = PageAddress;
            Flash.PageMap[i].VersionID = VersionID;
            Flash.PageMap[i].IsValid = STD_VALID;
            if (FindFlag == STD_FALSE)
            {
                if (Flash.PageMapIndex == 0)
                {
                    Flash.PageMapIndex = i;
                }
                else if (Flash.PageMap[i].VersionID - Flash.PageMap[Flash.PageMapIndex].VersionID > PAGE_NUMBER)
                {
                    // 这里两个页的VersionID差值大于了PageNumber，说明VersionID已经翻转，小的那个是最新的页
                    // uint8_t的数据 255 + 1= 0 --> 0 - 255 = 1
                    FindFlag = STD_TRUE;
                }
                Flash.PageMapIndex = i;
                Flash.NowAddress = Flash.PageMap[i].Address;
            }
        }
        else
        {
            Flash.PageMap[i].Address = PageAddress;
            Flash.PageMap[i].VersionID = UINT16_MAX;
            Flash.PageMap[i].IsValid = STD_INVALID;
            Flash.InvalidPageCnt++;
        }
    }
    return Flash.InvalidPageCnt;
}

/**
 * @brief 对找到的页进行CRC校验
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t PageVerify(void)
{
    uint32_t Page_CRC32 = 0;
    if (Dev_DFLASH_Read(Flash.NowAddress, Flash.Buffer, 1) == STD_SUCCESS)
    {
        MemoryCopy((uint8_t *)&Page_CRC32, Flash.Buffer + PAGE_CRC_BASE, CRC_SIZE);
        if (Page_CRC32 == CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE))
        {
            return STD_SUCCESS;
        }
    }
    {
        // 片内的Flash，读取失败说明Flash损坏
    }

    for (uint16_t i = 1; i < PAGE_NUMBER; i++)
    {
        if (FlashPreviousPage() == STD_SUCCESS)
        {
            return STD_SUCCESS;
        }
    }

    return STD_ERROR;
}

/**
 * @brief 在写入Flash前更新版本号、Page_Flag0、Page_Flag1、CRC
 *
 */
static void FlashUpDate_FrameInfor(void)
{
    uint32_t VersionID = 0;
    uint32_t Page_Flag0;
    uint32_t Page_Flag1;
    uint32_t CRC;

    Flash.NowAddress = Flash.NowAddress + PAGE_SIZE; // 更新当前地址
    if (Flash.NowAddress >= BASE_ADDRESS + PAGE_NUMBER * PAGE_SIZE)
    {
        Flash.NowAddress = BASE_ADDRESS;
    }

    MemoryCopy((uint8_t *)&VersionID, Flash.Buffer + VERSIONID_BASE, VERSIONID_SIZE); // 获取版本号
    VersionID++;                                                                      // 版本更新
    Page_Flag0 = Page0FlagCalculate(VersionID, Flash.NowAddress);                     // 计算Page_Flag0
    Page_Flag1 = Page1FlagCalculate(VersionID, Flash.NowAddress);                     // 计算Page_Flag1

    MemoryCopy(Flash.Buffer + VERSIONID_BASE, (uint8_t *)&VersionID, VERSIONID_SIZE);
    MemoryCopy(Flash.Buffer + PAGE_FLAG0_BASE, (uint8_t *)&Page_Flag0, PAGE_FLAG_SIZE);
    MemoryCopy(Flash.Buffer + PAGE_FLAG1_BASE, (uint8_t *)&Page_Flag1, PAGE_FLAG_SIZE);
    CRC = CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE); // 计算CRC
    MemoryCopy(Flash.Buffer + PAGE_CRC_BASE, (uint8_t *)&CRC, CRC_SIZE);
}

uint8_t FlashPreviousPage(void)
{
    uint32_t Index = Flash.PageMapIndex;
    uint32_t Page_CRC32 = 0;

    Flash.PageMap[Index].IsValid = STD_INVALID;
    Dev_DFLASH_Erase(Flash.NowAddress, 5);

    do
    {
        if (Index == 0)
        {
            Index = PAGE_NUMBER - 1;
        }
        else
        {
            Index--;
        }

        if (Flash.PageMap[Index].IsValid == STD_VALID)
        {
            if (Dev_DFLASH_Read(Flash.NowAddress, Flash.Buffer, 1) == STD_SUCCESS)
            {
                MemoryCopy((uint8_t *)&Page_CRC32, Flash.Buffer + PAGE_CRC_BASE, CRC_SIZE);
                if (Page_CRC32 == CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE))
                {
                    Flash.NowAddress = Flash.PageMap[Index].Address;
                    Flash.PageMapIndex = Index;
                    return STD_SUCCESS;
                }

                Flash.PageMap[Index].IsValid = STD_INVALID;
                Dev_DFLASH_Erase(Flash.NowAddress, 5);
            }
            else
            {
                // 片内的Flash，读取失败说明Flash损坏
            }
        }

        if (Index == Flash.PageMapIndex)
        {
            return STD_ERROR;
        }

    } while (1);

    return STD_ERROR;
}

uint8_t FlashBufferRead(uint8_t *Buffer, uint16_t Size)
{
    // 只对Data段的数据进行操作
    if (Size <= FLASH_DATA_SIZE)
    {
        MemoryCopy(Buffer, Flash.Buffer + FLASH_DATA_BASE, Size);
        return STD_SUCCESS;
    }

    return STD_ERROR;
}

uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size)
{
    // 只对Data段的数据进行操作
    if (Size <= FLASH_DATA_SIZE)
    {
        MemoryCopy(Flash.Buffer + FLASH_DATA_BASE, Buffer, Size);
        return STD_SUCCESS;
    }
    return STD_ERROR;
}

uint8_t FlashBufferCommit(void)
{
    FlashUpDate_FrameInfor();
    if (Dev_DFLASH_Erase(Flash.NowAddress, 5) == STD_SUCCESS &&
        Dev_DFLASH_Write(Flash.NowAddress, Flash.Buffer, 5) == STD_SUCCESS)
    {
        return STD_SUCCESS;
    }
    else
    {
        return STD_ERROR;
    }
}

uint32_t FlashNowAddress(void)
{
    return Flash.NowAddress;
}

void Flash_Init(void)
{
    Flash.NowAddress = BASE_ADDRESS;
    Flash.PageMapIndex = 0;
    Flash.InvalidPageCnt = 0;
    Flash.IsReady = STD_ERROR;

    MemorySet((uint8_t *)Flash.PageMap, 0, sizeof(Flash.PageMap));
    MemorySet(Flash.Buffer, 0, sizeof(Flash.Buffer));

    if (PageSearch() != PAGE_NUMBER)
    {
        Flash.IsReady = PageVerify();
    }
    else
    {
        for (uint16_t i = 0; i < PAGE_NUMBER; i++)
        {
            Dev_DFLASH_Erase(BASE_ADDRESS + i * PAGE_SIZE, 5);
        }
        MemorySet(Flash.Buffer, 0x00, PAGE_SIZE); // 数据段置零
        Flash.IsReady = FlashBufferCommit();
    }
}