/**
  ******************************************************************************
  * @file         stm32h7xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/*
********************************************************************************
*                             INCLUDES
********************************************************************************
*/
#include "main.h"

/*
********************************************************************************
*                                HAL_MspInit()
*
* Description : Initializes the Global MSP.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/*
********************************************************************************
*                              HAL_UART_MspInit()
*
* Description : UART MSP Initialization.
*
* Argument(s) : huart:          UART handle pointer
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (huart->Instance == USART3)
    {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            //Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PD8     ------> USART3_RX
        PD9     ------> USART3_TX
        */
        GPIO_InitStruct.Pin         = STLINK_RX_Pin | STLINK_TX_Pin;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_PULLUP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }

//    /* USART3 interrupt Init */
//    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/*
********************************************************************************
*                              HAL_UART_MspDeInit()
*
* Description : UART MSP De-Initialization.
*
* Argument(s) : huart:          UART handle pointer
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PD8     ------> USART3_TX
        PD9     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOD, STLINK_RX_Pin | STLINK_TX_Pin);
    }
}

void HAL_HCD_MspInit(HCD_HandleTypeDef* hcdHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (hcdHandle->Instance == USB_OTG_FS)
    {
        /** Initializes the peripherals clock */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
#if 0
        PeriphClkInitStruct.PLL3.PLL3M = 8;
        PeriphClkInitStruct.PLL3.PLL3N = 336;
        PeriphClkInitStruct.PLL3.PLL3P = 2;
        PeriphClkInitStruct.PLL3.PLL3Q = 7;
        PeriphClkInitStruct.PLL3.PLL3R = 2;
        PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
        PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
        PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
        PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
#else
        PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
#endif
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }
        /** Enable USB Voltage detector */
        HAL_PWREx_EnableUSBVoltageDetector();
#if 1
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USB_OTG_FS GPIO Configuration
        PA8     ------> USB_OTG_FS_SOF
        PA9     ------> USB_OTG_FS_VBUS
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
#if 1
        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(OTG_FS_EP1_OUT_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_EP1_OUT_IRQn);

        HAL_NVIC_SetPriority(OTG_FS_EP1_IN_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_EP1_IN_IRQn);

        HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
#else
        HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
#endif
    }
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hcdHandle)
{
    if (hcdHandle->Instance == USB_OTG_FS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
#if 1
        /**USB_OTG_FS GPIO Configuration
        PA8     ------> USB_OTG_FS_SOF
        PA9     ------> USB_OTG_FS_VBUS
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9| GPIO_PIN_11 | GPIO_PIN_12);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_FS_EP1_OUT_IRQn);
        HAL_NVIC_DisableIRQ(OTG_FS_EP1_IN_IRQn);
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
#else
        HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
#endif
  }
}
