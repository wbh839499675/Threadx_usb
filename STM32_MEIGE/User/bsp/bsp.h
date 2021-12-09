/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32H7)
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明

*
*	Copyright (C)
*
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__

#define STM32_V7


/* 检查是否定义了开发板型号 */
#if !defined (STM32_V7)
	#error "Please define the board model : STM32_V7"
#endif

/* 定义 BSP 版本号 */
#define __STM32H7_BSP_VERSION		"1.1"

#define  USE_THREADX    1

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h 中定义此行，表示本函数移到 stam32f4xx_it.c。 避免重复定义 */

#define ERROR_HANDLER()		Error_Handler(__FILE__, __LINE__);

/* 默认是关闭状态 */
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

/* 定义优先级分组 */
#define NVIC_PREEMPT_PRIORITY	4

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
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
