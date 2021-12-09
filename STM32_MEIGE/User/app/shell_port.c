/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "includes.h"
#include "shell.h"
#include "stm32h7xx_hal.h"
#include "log.h"
#include "tx_api.h"

extern UART_HandleTypeDef UART3_Handler;

Shell shell;
char shellBuffer[512];

//static SemaphoreHandle_t shellMutex;
static TX_MUTEX             shellMutex;

#define  APP_CFG_TASK_SHELL_PRIO                            1u

#define APP_CFG_TASK_SHELL_STK_SIZE                         1024u * 80

static TX_THREAD        tcb_task_shell;
static uint64_t         stk_task_shell[APP_CFG_TASK_SHELL_STK_SIZE / 8];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    //serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
    HAL_UART_Transmit(&UART3_Handler, (uint8_t *)data, len, 0x1FF);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    //return HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)data, len);
    //uint8_t status = HAL_UART_Receive(&UART3_Handler, (uint8_t *)data, len, 100);
    uint8_t status = HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)data, len);
    if (status == 0)
        log_d("status = %d\r\n", status);
    return status;
}


/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    //xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    tx_mutex_get(&shellMutex, TX_WAIT_FOREVER);
    
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    //xSemaphoreGiveRecursive(shellMutex);
    tx_mutex_put(&shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    //shellMutex = xSemaphoreCreateMutex();
    tx_mutex_create(&shellMutex, (CHAR *)"shell mutex", TX_NO_INHERIT);

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
#if 0
    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
    {
        logError("shell task creat failed");
    }
#else
    tx_thread_create(&tcb_task_shell,
                       "The SHELL Task",
                       shellTask,
                       &shell,
                       &stk_task_shell[0],
                       APP_CFG_TASK_SHELL_STK_SIZE,
                       APP_CFG_TASK_SHELL_PRIO,
                       APP_CFG_TASK_SHELL_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);
#endif
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

