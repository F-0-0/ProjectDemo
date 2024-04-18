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

#define Page0FlagCalculate(VersionID, PageAddress) (VersionID ^ PAGE_FLAG0 + PageAddress ^ PAGE_SIZE + PAGE_FLAG0)
#define Page1FlagCalculate(VersionID, PageAddress) (VersionID ^ PAGE_FLAG1 + PageAddress ^ PAGE_SIZE + PAGE_FLAG1)

static Flash_t Flash;

static uint8_t PageSearch(void);
static uint8_t PageSearchLast(void);
static uint8_t PageVerify(void);
static void PageMapSort(void);

static void FlashUpDate_FrameInfor(void);
static void FlashUpDate_NextAddress(void);

/**
 * @brief 寻找所有的有效页
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t PageSearch(void)
{
    uint32_t PageAddress = BASE_ADDRESS;
    uint32_t VersionID = 0;
    uint32_t Page_Flag0 = 0;
    uint32_t Page_Flag1 = 0;

    for (uint16_t i = 0; i < PAGE_NUMBER; i++)
    {
        PageAddress += i * PAGE_SIZE;

        MemoryCopy((uint8_t *)&VersionID, (uint8_t *)(PageAddress + VERSIONID_BASE), VERSIONID_SIZE);
        MemoryCopy((uint8_t *)&Page_Flag0, (uint8_t *)(PageAddress + PAGE_FLAG0_BASE), PAGE_FLAG_SIZE);
        MemoryCopy((uint8_t *)&Page_Flag1, (uint8_t *)(PageAddress + PAGE_FLAG1_BASE), PAGE_FLAG_SIZE);

        if (Page_Flag0 == Page0FlagCalculate(VersionID, PageAddress) &&
            Page_Flag1 == Page1FlagCalculate(VersionID, PageAddress))
        {
            Flash.PageMap[i].Address = PageAddress;
            Flash.PageMap[i].VersionID = VersionID;
            Flash.PageMap[i].IsValid = STD_VALID;
            Flash.PageMap[i].IsErase = STD_DISABLE;
        }
        else
        {
            Flash.PageMap[i].Address = PageAddress;
            Flash.PageMap[i].VersionID = UINT16_MAX;
            Flash.PageMap[i].IsValid = STD_INVALID;
            Flash.PageMap[i].IsErase = STD_ENABLE;
            Flash.InvalidPageCnt++;
        }
    }
    return STD_SUCCESS;
}

/**
 * @brief 对PageMap进行排序：冒泡排序 这里有优化空间
 *
 */
static void PageMapSort(void)
{
    uint16_t PageSize = PAGE_SIZE;
    for (uint16_t i = 0; i < PageSize - 1; i++)
    {
        for (uint16_t j = 0; j < PageSize - i - 1; j++)
        {
            if (Flash.PageMap[j].VersionID > Flash.PageMap[j + 1].VersionID)
            {
                uint16_t VersionID = Flash.PageMap[j].VersionID;
                uint16_t Address = Flash.PageMap[j].Address;
                uint8_t IsValid = Flash.PageMap[j].IsValid;

                Flash.PageMap[j].VersionID = Flash.PageMap[j + 1].VersionID;
                Flash.PageMap[j].Address = Flash.PageMap[j + 1].Address;
                Flash.PageMap[j].IsValid = Flash.PageMap[j + 1].IsValid;

                Flash.PageMap[j + 1].VersionID = VersionID;
                Flash.PageMap[j + 1].Address = Address;
                Flash.PageMap[j + 1].IsValid = IsValid;
            }
        }
    }
}

/**
 * @brief 寻找最新的有效页
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t PageSearchLast(void)
{
    Flash.PageMapIndex = 0;
    for (uint16_t i = 0; i < PAGE_NUMBER - 1; i++)
    {
        // 因为之前对PageMap进行了排序，无效的页在最后；有效页VersionID排序是由小到大
        if (Flash.PageMap[i + 1].IsValid == STD_VALID)
        {
            // 这里两个页的VersionID差值大于了PageNumber，说明VersionID已经翻转，小的那个是最新的页
            if (Flash.PageMap[i + 1].VersionID - Flash.PageMap[i].VersionID >= PAGE_NUMBER)
            {
                Flash.PageMapIndex = i;
                break;
            }
            else // 取大的VersionID
            {
                Flash.PageMapIndex = i + 1;
            }
        }
        else // PageMap后面已经没有 有效的页
        {
            break;
        }
    }
    Flash.NowAddress = Flash.PageMap[Flash.PageMapIndex].Address;
    return STD_SUCCESS;
}

/**
 * @brief 对找到的页进行CRC校验
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t PageVerify(void)
{
    uint32_t Page_CRC32 = 0;
    for (uint16_t i = 0; i < PAGE_NUMBER; i++)
    {
        if (Dev_DFLASH_Read(Flash.NowAddress, Flash.Buffer) == STD_SUCCESS)
        {
            MemoryCopy((uint8_t *)&Page_CRC32, Flash.Buffer + PAGE_CRC_BASE, CRC_SIZE);
            if (Page_CRC32 == CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE))
            {
                Flash.IsReady = STD_SUCCESS;
                return STD_SUCCESS;
            }
            else
            {
                PreviousPage();
            }
        }
        else
        {
            PreviousPage();
        }
    }
    return STD_ERROR;
}

static void FlashUpDate_NextAddress(void)
{
    Flash.NextAddress = Flash.NowAddress + PAGE_SIZE; // 更新当前地址
    if (Flash.NextAddress >= BASE_ADDRESS + PAGE_NUMBER * PAGE_SIZE)
    {
        Flash.NextAddress = BASE_ADDRESS;
    }
}

static void FlashUpDate_FrameInfor(void)
{
    uint32_t VersionID = 0;
    uint32_t Page_Flag0;
    uint32_t Page_Flag1;
    uint32_t CRC;

    FlashUpDate_NextAddress();
    Flash.NowAddress = Flash.NextAddress;
    MemoryCopy((uint8_t *)&VersionID, Flash.Buffer + VERSIONID_BASE, VERSIONID_SIZE); // 获取版本号

    Page_Flag0 = Page0FlagCalculate(VersionID, Flash.NowAddress); // 计算Page_Flag0
    Page_Flag1 = Page0FlagCalculate(VersionID, Flash.NowAddress); // 计算Page_Flag1

    VersionID++; // 版本更新，32位数据记录版本号，不用管溢出，溢出翻转
    MemoryCopy(Flash.Buffer + VERSIONID_BASE, (uint8_t *)&VersionID, VERSIONID_SIZE);
    MemoryCopy(Flash.Buffer + PAGE_FLAG0_BASE, (uint8_t *)&Page_Flag0, PAGE_FLAG_SIZE);
    MemoryCopy(Flash.Buffer + PAGE_FLAG1_BASE, (uint8_t *)&Page_Flag1, PAGE_FLAG_SIZE);
    CRC = CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE); // 计算CRC
    MemoryCopy(Flash.Buffer + PAGE_CRC_BASE, (uint8_t *)&CRC, CRC_SIZE);
}

uint8_t PreviousPage(void)
{
    if (Flash.PageMapIndex == 0)
    {
    }
    return STD_SUCCESS;
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

        if (Flash.IsEraseNextPage == STD_FALSE)
        {
            FlashUpDate_NextAddress();
            Dev_DFLASH_Erase(Flash.NextAddress);
        }

        if (Flash.IsEraseNextPage == STD_TRUE && Flash.WriteCnt-- <= 0)
        {
            Dev_DFLASH_Write(Flash.NowAddress, Flash.Buffer);
        }

        return STD_SUCCESS;
    }
    return STD_ERROR;
}

void Flash_Init(uint8_t WriteCntToCommit)
{
    Flash.IsEraseNextPage = STD_FALSE;
    Flash.NowAddress = BASE_ADDRESS;
    Flash.IsReady = STD_ERROR;
    Flash.WriteCnt = WriteCntToCommit;
    Flash.InvalidPageCnt = 0;

    MemorySet((uint8_t *)Flash.PageMap, 0, sizeof(Flash.PageMap));
    MemorySet(Flash.Buffer, 0, sizeof(Flash.Buffer));

    PageSearch();
    if (Flash.InvalidPageCnt != PAGE_SIZE)
    {
        PageMapSort();
        PageSearchLast();
        PageVerify();
    }
    else
    {
        MemorySet(Flash.Buffer, 0x00, PAGE_SIZE); // 数据段置零
        FlashUpDate_FrameInfor();
        if (Dev_DFLASH_Erase(Flash.NowAddress) == STD_SUCCESS &&
            Dev_DFLASH_Write(Flash.NowAddress, Flash.Buffer) == STD_SUCCESS)
        {
            Flash.IsReady = STD_SUCCESS;
        }
        else
        {
            Flash.IsReady = STD_ERROR;
        }
    }
}