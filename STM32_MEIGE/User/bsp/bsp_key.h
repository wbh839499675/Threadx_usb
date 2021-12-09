/*
*********************************************************************************************************
*
*	ģ������ : BSP LEDģ��
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��
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
