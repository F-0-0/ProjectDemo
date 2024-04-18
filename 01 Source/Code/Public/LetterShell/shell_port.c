/**
 * @file shell_port.c
 * @brief
 * @author FanLinLin
 * @date 2024-04-13
 */

#include <stdio.h>
#include "log.h"
#include "shell_port.h"

extern void Dev_UART_RxBuf_Print(unsigned char instance);
extern char Dev_UART_RxBuf_Pop(unsigned char instance);
extern unsigned short Dev_UART_RxBuf_Size(unsigned char instance);

void LogWrite(char *Buffer, short len);

Log shell_Log = {
    .write = LogWrite,
    .active = 1,
    .level = LOG_DEBUG};

void LogWrite(char *Buffer, short len)
{
    if (shell_Log.shell)
    {
        shellWriteEndLine(shell_Log.shell, Buffer, len);
    }
}
/* 1. 创建shell对象，开辟shell缓冲区 */
Shell shell;
char shell_buffer[512];

/* 2. 自己实现shell写函数 */

// shell写函数原型：signed short (*write)(char *, unsigned short);/**< shell写函数 */
signed short User_Shell_Write(char *ch, unsigned short len)
{
    // 串口打印
    for (unsigned short i = 0; i < len; i++)
    {
        printf("%c", ch[i]);
    }
    return len;
}

extern char UART0_RxBuffer[1024]; // UART0接收数据存储数组
signed short User_Shell_Read(char *data, unsigned short len)
{
    // Dev_UART_RxBuf_Print();

    unsigned short size = 0;

    if (Dev_UART_RxBuf_Size(0) != 0)
    {
        *data = Dev_UART_RxBuf_Pop(0);
        size++;
    }

    return size;
}

/* 3. 编写初始化函数 */
void User_Shell_Init(void)
{
    // 注册自己实现的写函数
    shell.write = User_Shell_Write;
    shell.read = User_Shell_Read;

    // 调用shell初始化函数
    shellInit(&shell, shell_buffer, 512);
    logRegister(&shell_Log, &shell);
}