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

void BSP_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : KEY_Pin */
    GPIO_InitStruct.Pin     = KEY_Pin;
    GPIO_InitStruct.Mode    = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

    /* EXTI2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    /* EXTI3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

UINT BSP_KEY_Scan(void)
{
#if 0
    uint8_t status = 0;
    if (KEY_Read_Value()== 0)
    {
        status = 1;
    }
    else
    {
        status = 0;
    }
    return status;
#else
    return (KEY_Read_Value());
#endif
}
