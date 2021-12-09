/*
*********************************************************************************************************
* Copyright     :
* Filename      : KEY_app.c
* Description   :
* Author        :
* Version       : V1.0
* Date          : 2021-12-01
* History       :
*********************************************************************************************************
* Note(s)       :
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           INCLUDE FILES
*********************************************************************************************************
*/
#define APP_KEY_MODULE

#include "includes.h"
#include "bsp_key.h"

/*
*********************************************************************************************************
*                                           LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern void BSP_KEY_Init(void);
extern void USB_VbusFS(uint8_t state);
/*
*********************************************************************************************************
*                                             PROTOTYPES
*********************************************************************************************************
*/ 

/*
*********************************************************************************************************
*                                             Task_KEY()
*
* Description : The KEY Task.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
#if USE_THREADX
void Task_KEY(ULONG thread_input)
{
    static ULONG    status = 1;
    BSP_KEY_Init();

    while (1) 
    {
        if (BSP_KEY_Scan() == 1)
        {
            status++;
        }

        USB_VbusFS(status % 2);
        tx_thread_sleep(MS_TO_TICK(10000));
    }
}
#endif