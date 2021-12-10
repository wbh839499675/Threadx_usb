/*
*********************************************************************************************************
*
*	模块名称 : BSP LED模块
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期         作者       说明
*
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "tx_port.h"

#define KEY_PIN                 GPIO_PIN_13
#define KEY_GPIO_PORT           GPIOC

#define KEY_EXTI_IRQn           EXTI15_10_IRQn
#define KEY_EXTI_LINE           EXTI_LINE_13

#define KEY_Read_Value()        HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_PIN)

#define KEY_STATUS_PRESS        (0x01)


void BSP_KEY_Init(void);


#endif
