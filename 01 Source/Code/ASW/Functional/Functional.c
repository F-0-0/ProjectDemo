/**
 * @file Functional.c
 * @brief
 * @author F00(1565438246@qq.com)
 * @date 2024-04-13
 */

#include "../AppService_IF.h"

extern void PowerManage_Task(void);
extern void SleepManage_Task(void);

void Functional_Task(void)
{
    static uint32_t cnt = 0;
    logInfo("Functional Task Runing:%d\t", cnt++);

    PowerManage_Task();
    SleepManage_Task();
}