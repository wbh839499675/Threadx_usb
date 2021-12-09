/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For STM32H7)
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��

*
*	Copyright (C)
*
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__

#define STM32_V7


/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_V7)
	#error "Please define the board model : STM32_V7"
#endif

/* ���� BSP �汾�� */
#define __STM32H7_BSP_VERSION		"1.1"

#define  USE_THREADX    1

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h �ж�����У���ʾ�������Ƶ� stam32f4xx_it.c�� �����ظ����� */

#define ERROR_HANDLER()		Error_Handler(__FILE__, __LINE__);

/* Ĭ���ǹر�״̬ */
#define  Enable_EventRecorder  0

#if Enable_EventRecorder == 1
	#include "EventRecorder.h"
#endif

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* �������ȼ����� */
#define NVIC_PREEMPT_PRIORITY	4

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
#include "bsp_led.h"
#include "bsp_usb.h"

#define B1_Pin                          GPIO_PIN_13
#define B1_GPIO_Port                    GPIOC
#define LD1_Pin                         GPIO_PIN_0
#define LD1_GPIO_Port                   GPIOB
#define LD3_Pin                         GPIO_PIN_14
#define LD3_GPIO_Port                   GPIOB
#define STLINK_RX_Pin                   GPIO_PIN_8
#define STLINK_RX_GPIO_Port             GPIOD
#define STLINK_TX_Pin                   GPIO_PIN_9
#define STLINK_TX_GPIO_Port             GPIOD
#define USB_OTG_FS_PWR_EN_Pin           GPIO_PIN_10
#define USB_OTG_FS_PWR_EN_GPIO_Port     GPIOD
#define USB_OTG_FS_OVCR_Pin             GPIO_PIN_7
#define USB_OTG_FS_OVCR_GPIO_Port       GPIOG
#define LD2_Pin                         GPIO_PIN_1
#define LD2_GPIO_Port                   GPIOE






void bsp_Init(void);
void bsp_Idle(void);
void System_Init(void);

void bsp_GetCpuID(uint32_t *_id);
void Error_Handler(char *file, uint32_t line);

#endif
