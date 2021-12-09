#include "bsp_led.h"
#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"
#include "stdint.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitStruct.Pin    = GPIO_PIN_LED1;
    GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull   = GPIO_PULLUP;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIO_LED1, &GPIO_InitStruct);

    GPIO_InitStruct.Pin    = GPIO_PIN_LED2;
    GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull   = GPIO_PULLUP;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIO_LED2, &GPIO_InitStruct);

    GPIO_InitStruct.Pin    = GPIO_PIN_LED3;
    GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull   = GPIO_PULLUP;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIO_LED3, &GPIO_InitStruct);

//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
}

