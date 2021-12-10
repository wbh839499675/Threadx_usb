/*
********************************************************************************
*
*                                 KEY DRIVER
*
* Filename      : bsp_key.c
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
#include "bsp_key.h"


extern TX_EVENT_FLAGS_GROUP                        key_event_flag;

EXTI_HandleTypeDef                          key_exti;

static void key_exti_callback(void);

void BSP_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : KEY_Pin */
    GPIO_InitStruct.Pin     = KEY_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

    (void)HAL_EXTI_GetHandle(&key_exti, KEY_EXTI_LINE);
    (void)HAL_EXTI_RegisterCallback(&key_exti, 0x00, key_exti_callback);

    /* EXTI3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(KEY_EXTI_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(KEY_EXTI_IRQn);
}

void BSP_KEY_Deinit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin     = KEY_PIN;
    HAL_GPIO_DeInit(KEY_GPIO_PORT, GPIO_InitStruct.Pin);

    HAL_NVIC_DisableIRQ(KEY_EXTI_IRQn);
}

static void key_exti_callback(void)
{
    tx_event_flags_set(&key_event_flag, KEY_STATUS_PRESS, TX_OR);
}

void EXTI15_10_IRQHandler(void)
{
    key_exti_callback();
    HAL_EXTI_ClearPending(&key_exti, GPIO_MODE_IT_RISING);
}

