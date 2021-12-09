
#ifndef __BSP_USB_H__
#define __BSP_USB_H__

#include "ux_system.h"
#include "ux_utility.h"
#include "ux_hcd_stm32.h"

///////////////////////////defined for USB//////////////////////////////////////
typedef enum
{
    USB_VBUS_FALSE = 0,
    USB_VBUS_TRUE,
} USB_VBUS_State;

typedef enum
{
    Device_disconnect = 0,
    Device_connected,
    No_Device,
} Device_State;

typedef struct
{
    Device_State    Dev_state;
} ux_app_devInfotypeDef;

typedef enum
{
    App_Ready = 1,
    App_Start,
    App_Idle,
} ux_app_stateTypeDef;

#define USB_OTG_FS_PWR_EN_Pin           GPIO_PIN_10
#define USB_OTG_FS_PWR_EN_GPIO_Port     GPIOD
////////////////////////////////////////////////////////////////////////////////

extern UINT MX_USB_Host_Init(void);

#endif
