/*
********************************************************************************
*                             INCLUDES
********************************************************************************
*/
#include "includes.h"
#include "bsp_usb.h"
#include "ux_host_class_meige.h"
#include "ux_host_class_cdc_acm.h"

extern TX_QUEUE                        ux_app_message_queue;
extern ux_app_devInfotypeDef           ux_dev_info;

extern UX_HOST_CLASS_MEIGE             *app_cdc_meig;
extern UX_HOST_CLASS_CDC_ACM           *app_cdc_acm;
/*
********************************************************************************
*                             LOCAL VARIABLE
********************************************************************************
*/
HCD_HandleTypeDef hhcd_USB_OTG_FS;

/*
********************************************************************************
*                              PROTOTYPES
********************************************************************************
*/
extern UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *Current_class, VOID *Current_instance);

/*
********************************************************************************
*                                USB_Vbus_Init()
*
* Description : VBUS port Initialization Function.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
void USB_Vbus_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin : USB_OTG_FS_PWR_EN_Pin */
    GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USB_OTG_FS_PWR_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_OTG_FS_OVCR_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/*
********************************************************************************
*                                USB_VbusFS()
*
* Description : VBUS port control Function.
*
* Argument(s) : state:          The VBUS status
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
void USB_VbusFS(uint8_t state)
{
    uint8_t data = state;

    if(state == 0)
        data = GPIO_PIN_SET;
    else
        data = GPIO_PIN_RESET;

    HAL_GPIO_WritePin(USB_OTG_FS_PWR_EN_GPIO_Port, USB_OTG_FS_PWR_EN_Pin, (GPIO_PinState)data);
}

/*
*********************************************************************************************************
*                                             MX_USB_OTG_FS_HCD_Init()
*
* Description : USB_OTG_FS Initialization Function.
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

void MX_USB_OTG_FS_HCD_Init(void)
{
    hhcd_USB_OTG_FS.Instance = USB_OTG_FS;
    hhcd_USB_OTG_FS.Init.Host_channels = 16;
    hhcd_USB_OTG_FS.Init.speed = HCD_SPEED_FULL;
    hhcd_USB_OTG_FS.Init.dma_enable = DISABLE;
    hhcd_USB_OTG_FS.Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
    hhcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
    hhcd_USB_OTG_FS.Init.use_external_vbus = DISABLE;

    if (HAL_HCD_Init(&hhcd_USB_OTG_FS) != HAL_OK)
    {
        log_e("HAL_HCD_Init failed.\r\n");
    }
}


UINT MX_USB_Host_Init(void)
{
    UINT ret = UX_SUCCESS;

    /* The code below is required for installing the host portion of USBX */
    if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
    {
        log_e("ux_host_stack_initialize failed.\r\n");
        return UX_ERROR;
    }

    /* Register MEIGE class */
#if 1
    if (ux_host_stack_class_register(_ux_system_host_class_meige_name,
                                    ux_host_class_meige_entry) != UX_SUCCESS)
#else
    if (ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name,
                                        _ux_host_class_cdc_acm_entry) != UX_SUCCESS)
#endif
    {
        log_e("ux_host_stack_class_register failed.\r\n");
        return UX_ERROR;
    }

    /* Initialize the LL driver */
    MX_USB_OTG_FS_HCD_Init();

    /* Register all the USB host controllers available in this system */
#if 0
    if (ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                    ux_hcd_stm32_initialize,
                                    USB_OTG_HS_PERIPH_BASE,
                                    (ULONG)&hhcd_USB_OTG_FS) != UX_SUCCESS)
#else
    if (ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                    ux_hcd_stm32_initialize,
                                    USB_OTG_FS_PERIPH_BASE,
                                    (ULONG)&hhcd_USB_OTG_FS) != UX_SUCCESS)
#endif
    {
        log_e("ux_host_stack_hcd_register failed.\r\n");
        return UX_ERROR;
    }

    /* Drive vbus to be addedhere */
    USB_VbusFS(USB_VBUS_TRUE);

    /* Enable USB Gloable Interrupt */
    HAL_HCD_Start(&hhcd_USB_OTG_FS);
    
    return ret;
}
