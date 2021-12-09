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

extern short userShellRead(char *data, unsigned short len);

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

    UART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    UART3_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    //UART3_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    UART3_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_DMADISABLEONERROR_INIT;
    UART3_Handler.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;

    HAL_UART_Init(&UART3_Handler);

    if (HAL_UARTEx_SetTxFifoThreshold(&UART3_Handler, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        //Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&UART3_Handler, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        //Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&UART3_Handler) != HAL_OK)
    {
        //Error_Handler();
    }

    //HAL_UART_Receive_DMA(&UART3_Handler, (uint8_t *)aRxBuffer, 1);
    

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    
    HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)aRxBuffer, 1);
}

void USART3_IRQHandler(void)
{ 
    uint32_t timeout=0;
    uint32_t maxDelay = 0x1FFFF;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif

    HAL_UART_IRQHandler(&UART3_Handler);

#if 1
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
#else
    if (__HAL_UART_GET_FLAG(&UART3_Handler, UART_FLAG_IDLE))
    {
        static uint16_t count;

        __HAL_UART_CLEAR_IDLEFLAG(&UART3_Handler);
        //if (userShellRead)
        if (1)
        {
            //count = RXBUFFERSIZE - UART3_Handler.RxXferCount;
            //UART3_Handler.RxState = HAL_UART_STATE_READY;
            //HAL_UART_Receive_IT(&UART3_Handler, (uint8_t *)aRxBuffer, 1);
        }
    }

#endif
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
} 

extern char recv_buf;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
#if 0
        if ((USART_RX_STA & 0x8000) == 0)
        {
            if (USART_RX_STA & 0x4000)
            {
                if (aRxBuffer[0] != 0x0a)
                    USART_RX_STA = 0;
                else
                    USART_RX_STA |= 0x8000;
            }
            else
            {
                if (aRxBuffer[0] == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0;
                }
            }
        }
#else
    //shellHandler(&shell, recv_buf);
    

#endif
    }
}
