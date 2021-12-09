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

#define KEY_Pin                 GPIO_PIN_13
#define KEY_GPIO_Port           GPIOC

#define KEY_Read_Value()        HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

void BSP_KEY_Init(void);
UINT BSP_KEY_Scan(void);

#endif
