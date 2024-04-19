/**
 * @file Public_Type.h
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */
#ifndef PUBLIC_TYPE_H
#define PUBLIC_TYPE_H

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>

// clang-format off

typedef signed      char            int8_t;
typedef signed      short   int     int16_t;
typedef signed      int             int32_t;
typedef signed      long    long    int64_t;

typedef unsigned    char            uint8_t;
typedef unsigned    short   int     uint16_t;
typedef unsigned    int             uint32_t;
typedef unsigned    long    long    uint64_t;

typedef unsigned    char            Bool;

#define STD_NULL                    ((void *)0U)

#define STD_VALID                   1U
#define STD_INVALID                 0U

#define STD_TRUE                    1U
#define STD_FALSE                   0U

#define STD_ENABLE                  1U
#define STD_DISABLE                 0U

#define STD_SUCCESS                 1U
#define STD_ERROR                   0U

#define STD_HIGH                    1U
#define STD_LOW                     0U

#define STD_OPEN                    1U
#define STD_CLOSE                   0U

#define STD_LOCK                    1U
#define STD_UNLOCK                  0U

#define STD_ACTIVE                  1U
#define STD_IDLE                    0U

// 时间宏定义 分辨率1us
#define TIME_1US                    (1)
#define TIME_1MS                    (TIME_1US * 1000)
#define TIME_1S                     (TIME_1MS * 1000)
#define TIME_1M                     (TIME_1S * 60)
#define TIME_1H                     (TIME_1S * 60)
#define TIME_1D                     (TIME_1H * 24)

#define Connect(a,b)                a##b // int   n =Connect(123,456), n=123456
#define ToSting(a)                  #a   // char* n =ToSting(123456),  n="123456"

#define Get_Bit(value,bit)          ((value) &   (1<<(bit))) // 读取指定位
#define Cpl_Bit(value,bit)          ((value) ^=  (1<<(bit))) // 取反指定位
#define Set_BitTo0(value,bit)       ((value) &=~ (1<<(bit))) // 把某个位置0
#define Set_BitTo1(value,bit)       ((value) |=  (1<<(bit))) // 把某个位置1
#define Toggle_Bit(value,bit)       ((value) ^=  (1<<(bit))) //! 把某个位反转


/**
 * @brief 向上循环，超出max则从min开始，直到max-min次循环结束
 *
 * @param index 循环变量
 * @param cycle 循环次数
 * @param start index 开始的值
 * @param min index 能达到的最小值
 * @param max index 能达到的最大值-1
 */
#define For_U(index, cycle, start, min, max) for (index = (start < max ? start : min), cycle = 0; \
                                                  cycle < max - min;                              \
                                                  index++, index = (index < max ? index : min), cycle++)

/**
 * @brief 向下循环，超出min则从max开始，直到max-min次循环结束
 *
 * @param index 循环变量
 * @param cycle 循环次数
 * @param start index 开始的值
 * @param min index 能达到的最小值
 * @param max index 能达到的最大值-1
 */
#define For_D(index, cycle, start, min, max) for (index = (start > min ? start : max), cycle = 0; \
                                                  cycle < max - min;                              \
                                                  index--, index = (index >= min ? index : max - 1), cycle++)


// #define DEBUG     //! 注释后取消断言
#define ASW_DEBUG //! 注释后取消断言
#define BSW_DEBUG //! 注释后取消断言
#ifdef DEBUG
#ifdef ASW_DEBUG
#define ASW_ASSERT(x) \
    while (!x)        \
        while (!x)    \
        {             \
            __FILE__; \
            __LINE__; \
        }
#else
#define ASW_ASSERT(x) 0
#endif

#ifdef BSW_DEBUG
#define BSW_ASSERT(x) \
    while (!x)        \
    {                 \
        __FILE__;     \
        __LINE__;     \
    }

#else
#define BSW_ASSERT(x) 0
#endif
#else
#define BSW_ASSERT(x) 0
#define ASW_ASSERT(x) 0
#endif
// clang-format on

#endif