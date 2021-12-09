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

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#define GPIO_LED1               GPIOB
#define GPIO_PIN_LED1           GPIO_PIN_0

#define GPIO_LED2               GPIOE
#define GPIO_PIN_LED2           GPIO_PIN_1

#define GPIO_LED3               GPIOB
#define GPIO_PIN_LED3           GPIO_PIN_14


#define LED1_ON()               HAL_GPIO_WritePin(GPIO_LED1, GPIO_PIN_LED1, GPIO_PIN_SET)
#define LED1_OFF()              HAL_GPIO_WritePin(GPIO_LED1, GPIO_PIN_LED1, GPIO_PIN_RESET)

#define LED2_ON()               HAL_GPIO_WritePin(GPIO_LED2, GPIO_PIN_LED2, GPIO_PIN_SET)
#define LED2_OFF()              HAL_GPIO_WritePin(GPIO_LED2, GPIO_PIN_LED2, GPIO_PIN_RESET)

#define LED3_ON()               HAL_GPIO_WritePin(GPIO_LED3, GPIO_PIN_LED3, GPIO_PIN_SET)
#define LED3_OFF()              HAL_GPIO_WritePin(GPIO_LED3, GPIO_PIN_LED3, GPIO_PIN_RESET)


void LED_Init(void);

#endif

/***************************** MEIGE SMART www.meigsmart.com (END OF FILE) *********************************/
