/*
********************************************************************************
*
*                                 USART3 DRIVER
*
* Filename      : bsp_uart.c
* Version       : V1.0.0
* Programmer(s) : 
********************************************************************************
* Note(s)       : 
********************************************************************************
*/

/*
********************************************************************************
*                                  INCLUDE FILES
********************************************************************************
*/
#include "includes.h"

struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;

void _sys_exit(int x) 
{ 
    x = x; 
} 

int fputc(int ch, FILE *f)
{
    while((USART3->ISR & 0X40)==0);
    USART3->TDR = (uint8_t)ch;      
    return ch;
}

#define RXBUFFERSIZE            1024
#define USART_REC_LEN           200
uint16_t USART_RX_STA = 0;

uint8_t USART_RX_BUF[USART_REC_LEN];

uint8_t aRxBuffer[RXBUFFERSIZE];
extern UART_HandleTypeDef UART3_Handler;


void bsp_uart3_init(uint32_t bound)
{
    UART3_Handler.Instance          = USART3;
    UART3_Handler.Init.BaudRate     = bound;
    UART3_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART3_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART3_Handler.Init.Parity       = UART_PARITY_NONE;
    UART3_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UART3_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART3_Handler);

    HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        if ((USART_RX_STA & 0x8000) == 0)
        {
            if (USART_RX_STA&0x4000)//接收到了0x0d
            {
                if (aRxBuffer[0] != 0x0a)
                    USART_RX_STA=0;//接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000;	//接收完成了 
            }
            else //还没收到0X0D
            {
                if (aRxBuffer[0] == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0] ;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0;//接收数据错误,重新开始接收
                }
            }
        }
    }
}
 
//串口3中断服务程序
void USART3_IRQHandler(void)
{ 
    uint32_t timeout=0;
    uint32_t maxDelay = 0x1FFFF;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();    
#endif

    HAL_UART_IRQHandler(&UART3_Handler);

    timeout = 0;
    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)
    {
        timeout++;
        if (timeout > maxDelay)
            break;
    }

    timeout = 0;
    while (HAL_UART_Receive_IT(&UART3_Handler,(uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
        timeout++;
        if (timeout > maxDelay)
            break;
    }
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
} 


