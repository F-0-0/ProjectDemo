/**
 * @file test.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#if 1
#include "./ASW/AppService_IF.h"
#include "stdio.h"

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

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int For_Test(int argc, char const *argv[])
{
    int index = 5;
    int min = 0;
    int max = 10;

    int a, b, c, d;
    For_U(a, b, 2, 0, 5)
    {
        printf("%d: %d-->", b, a);

        For_D(c, d, -2, -5, 0)
        {
            printf("%d ", c);
        }
        printf("\n");
    }

    printf("\n");

    return 0;
}

#endif