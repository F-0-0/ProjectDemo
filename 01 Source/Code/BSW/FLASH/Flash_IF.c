/**
 * @file Flash_IF.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "Flash_IF.h"

static Flash_t Flash;

uint8_t FlashInit(void);
uint8_t FlashBufferRead(uint8_t *Buffer, uint16_t Size);
uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size);

static uint8_t FlashRead(void);
static uint8_t FlashWrite(void);
static uint8_t FlashErase(void);

static uint8_t SearchVailPage(void);
static uint8_t SearchLastPage(void);
static uint8_t PreviousPage(void);
static uint8_t VerifyPage(void);
static void PageMapSort(void);

/**
 * @brief Flash擦除操作
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t FlashErase(void)
{
    uint32_t BaseAddress = Flash.BaseAddress;
    uint32_t MaxAddress = Flash.BaseAddress + PAGE_NUMBER * PAGE_SIZE - PAGE_SIZE;
    BSW_ASSERT(Flash.NowAddress >= BaseAddress && Flash.NowAddress <= MaxAddress);

    /*对接驱动*/

    return STD_SUCCESS;
}

/**
 * @brief Flash写操作
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t FlashWrite(void)
{
    uint32_t BaseAddress = Flash.BaseAddress;
    uint32_t MaxAddress = Flash.BaseAddress + PAGE_NUMBER * PAGE_SIZE - PAGE_SIZE;
    BSW_ASSERT(Flash.NowAddress >= BaseAddress && Flash.NowAddress <= MaxAddress);

    Flash.NowAddress += PAGE_SIZE; // 更新当前地址
    if (Flash.NowAddress >= Flash.BaseAddress + PAGE_NUMBER * PAGE_SIZE)
    {
        Flash.NowAddress = Flash.BaseAddress;
    }

    uint32_t VersionID = Flash.Buffer[VERSIONID_SEGMENT];                            // 获取版本号
    uint32_t Page_Flag0 = (VersionID | (Flash.NowAddress % PAGE_SIZE) & PAGE_FLAG0); // 计算Page_Flag0
    uint32_t Page_Flag1 = (VersionID | (Flash.NowAddress % PAGE_SIZE) & PAGE_FLAG1); // 计算Page_Flag1

    VersionID++; // 版本更新，32位数据记录版本号，不用管溢出，溢出翻转
    Flash.Buffer[VERSIONID_SEGMENT] = VersionID;
    Flash.Buffer[PAGE_FLAG0_SEGMENT] = Page_Flag0;
    Flash.Buffer[PAGE_FLAG1_SEGMENT] = Page_Flag1;
    uint32_t CRC = CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE); // 计算CRC
    Flash.Buffer[PAGE_CRC_SEGMENT] = CRC;

    FlashErase();

    /*对接驱动*/

    return STD_SUCCESS;
}

/**
 * @brief Flash读操作
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t FlashRead(void)
{
    /*对接驱动*/

    MemoryCopy((uint8_t *)Flash.Buffer, (uint8_t *)Flash.NowAddress, PAGE_SIZE);

    return STD_SUCCESS;
}

/**
 * @brief 缓存区读操作：将Flash的缓存数据拷贝到目标Buffer
 *
 * @param Buffer    目标Buffer
 * @param Size      数据拷贝的大小
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
uint8_t FlashBufferRead(uint8_t *Buffer, uint16_t Size)
{
    uint32_t InputDataSize = VERSIONID_SIZE + PAGEFLAG1_SIZE + Size;
    uint32_t LocaldDataSize = PAGE_SIZE - CRC_SIZE - PAGEFLAG1_SIZE;
    BSW_ASSERT(InputDataSize <= LocaldDataSize);

    // 只对Data段的数据进行操作
    MemoryCopy(Buffer, Flash.Buffer + VERSIONID_SIZE + PAGEFLAG1_SIZE, Size);
    return STD_SUCCESS;
}

/**
 * @brief 缓存区写操作：将目标Buffer数据拷贝到Flash的缓存
 *
 * @param Buffer    目标Buffer
 * @param Size      数据拷贝的大小
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
uint8_t FlashBufferWrite(uint8_t *Buffer, uint16_t Size)
{
    uint32_t InputDataSize = VERSIONID_SIZE + PAGEFLAG1_SIZE + Size;
    uint32_t LocaldDataSize = PAGE_SIZE - CRC_SIZE - PAGEFLAG1_SIZE;
    BSW_ASSERT(InputDataSize <= LocaldDataSize);

    // 只对Data段的数据进行操作
    MemoryCopy(Flash.Buffer + VERSIONID_SIZE + PAGEFLAG1_SIZE, Buffer, Size);
    return STD_SUCCESS;
}

/**
 * @brief
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t Task_IDLE(void)
{

    if (Flash.Buffer[PAGE_CRC_SEGMENT] != CRC32(Flash.Buffer, PAGE_SIZE - CRC_SIZE))
    {
        FlashWrite();
    }

    return STD_SUCCESS;
}

/**
 * @brief 寻找所有的有效页
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t SearchVailPage(void)
{
    uint32_t PageAddress = Flash.BaseAddress;
    uint32_t VersionID = 0;
    uint32_t Page_Flag0 = 0;
    uint32_t Page_Flag1 = 0;

    for (uint16_t i = 0; i < PAGE_NUMBER; i++)
    {
        PageAddress += i * PAGE_SIZE;
        VersionID = Flash.Buffer[VERSIONID_SEGMENT];
        Page_Flag0 = Flash.Buffer[PAGE_FLAG0_SEGMENT];
        Page_Flag1 = Flash.Buffer[PAGE_FLAG1_SEGMENT];

        if (Page_Flag0 == (VersionID | (PageAddress % PAGE_SIZE) & PAGE_FLAG0) &&
            Page_Flag1 == (VersionID | (PageAddress % PAGE_SIZE) & PAGE_FLAG1))
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
            Flash.PageMap[i].IsValid = STD_VALID;
            Flash.PageMap[i].IsErase = STD_ENABLE;
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
    for (uint32_t i = 0; i < PageSize - 1; i++)
    {
        for (uint32_t j = 0; j < PageSize - i - 1; j++)
        {
            if (Flash.PageMap[j].VersionID > Flash.PageMap[j + 1].VersionID)
            {
                uint16_t VersionID = Flash.PageMap[j].VersionID;
                uint32_t Address = Flash.PageMap[j].Address;
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
static uint8_t SearchLastPage(void)
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
    return STD_SUCCESS;
}

/**
 * @brief 找上一个有效页
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t PreviousPage(void)
{
    if (Flash.PageMapIndex == 0)
    {
    }
    return STD_SUCCESS;
}

/**
 * @brief 对找到的页进行CRC校验
 *
 * @return uint8_t  STD_SUCCESS:成功; STD_ERROR:失败
 */
static uint8_t VerifyPage(void)
{

    uint32_t Page_CRC32 = 0;
    for (uint16_t i = 0; i < PAGE_NUMBER; i++)
    {
        FlashRead();
        Page_CRC32 = Flash.Buffer[PAGE_CRC_SEGMENT];
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
    return STD_ERROR;
}

/**
 * @brief 初始化对象 需要配置宏BASE_ADDRESS、PAGE_SIZE、PAGE_NUMBER
 *
 * @return uint8_t      STD_SUCCESS:成功; STD_ERROR:失败
 */
void Flash_Init(void)
{
    Flash.BaseAddress = BASE_ADDRESS;
    Flash.NowAddress = BASE_ADDRESS;
    Flash.IsReady = STD_ERROR;

    MemorySet((uint8_t *)Flash.PageMap, 0, sizeof(Flash.PageMap));
    MemorySet((uint8_t *)Flash.Buffer, 0, sizeof(Flash.Buffer));

    SearchVailPage();
    PageMapSort();
    SearchLastPage();
    VerifyPage();

    if (Flash.IsReady == STD_ERROR)
    {
        MemorySet(Flash.Buffer, 0x00, PAGE_SIZE); // 数据段置零
        FlashWrite();
    }

    Flash.IsReady = STD_SUCCESS;
}

void Flash_Task(void)
{
}