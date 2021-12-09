/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Host MEIGE class                                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_meige.h"
#include "ux_host_stack.h"
#include "includes.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_host_class_meige_configure                       PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calls the USBX stack to do a SET_CONFIGURATION to the */
/*    meige. Once the meige is configured, its interface will be          */
/*    activated. The bulk endpoints enumerated(1 IN, 1 OUT ).             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    meige                                   Pointer to meige class      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_host_stack_configuration_interface_get  Get interface           */
/*    _ux_host_stack_device_configuration_get     Get configuration       */
/*    _ux_host_stack_device_configuration_select  Select configuration    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _ux_host_class_meige_activate                meige class activate   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            optimized based on compile  */
/*                                            definitions,                */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _ux_host_class_meige_configure(UX_HOST_CLASS_MEIGE *meige)
{
    UINT                status;
    UX_CONFIGURATION    *configuration;
#if UX_MAX_DEVICES > 1
    UX_DEVICE           *parent_device;
#endif

    /* If the device has been configured already, we don't need to do it again. */
    if (meige->ux_host_class_meige_device->ux_device_state == UX_DEVICE_CONFIGURED)
        return(UX_SUCCESS);

    /* A meige normally has one configuration. So retrieve the 1st configuration only. */
    status = _ux_host_stack_device_configuration_get(meige->ux_host_class_meige_device, 0, &configuration);
    if (status != UX_SUCCESS)
    {
        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_CONFIGURATION_HANDLE_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_CONFIGURATION_HANDLE_UNKNOWN, meige->ux_host_class_meige_device, 0, 0, UX_TRACE_ERRORS, 0, 0)

        log_e("failed...status = 0x%02x\r\n",status);
        return(UX_CONFIGURATION_HANDLE_UNKNOWN);
    }

#if UX_MAX_DEVICES > 1
    /* Check the meige power source and check the parent power source for incompatible connections. */
    if (meige->ux_host_class_meige_device->ux_device_power_source == UX_DEVICE_BUS_POWERED)
    {
        /* Get parent device pointer.  */
        parent_device = meige->ux_host_class_meige_device->ux_device_parent;
        
        /* If the device is NULL, the parent is the root meige and we don't have to worry 
           if the parent is not the root meige, check for its power source. */
        if ((parent_device != UX_NULL) && (parent_device->ux_device_power_source == UX_DEVICE_BUS_POWERED))
        {

            /* Error trap. */
            _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_CONNECTION_INCOMPATIBLE);

            /* If trace is enabled, insert this event into the trace buffer.  */
            UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_CONNECTION_INCOMPATIBLE, meige, 0, 0, UX_TRACE_ERRORS, 0, 0)
            log_e("failed...status = 0x%02x\r\n",status);
            return(UX_CONNECTION_INCOMPATIBLE);
        }
    }
#endif

    /* We have the valid configuration. Ask the USBX stack to set this configuration. */
    status = _ux_host_stack_device_configuration_select(configuration);
    if (status != UX_SUCCESS)
    {
        log_e("failed...status = 0x%02x\r\n",status);
        return(status);
    }

    log_i("meige->ux_host_class_meige_device->ux_device_descriptor->bDeviceClass:%02X\r\n",
            (UINT)meige->ux_host_class_meige_device->ux_device_descriptor.bDeviceClass);

    /* Choose data interface according to class ID */
    if (meige->ux_host_class_meige_device->ux_device_descriptor.bDeviceClass == 0xEF)       //ASR
    {
        meige->ux_host_class_meige_data_interface = 4;
        log_i("ASR modem, data interface is 4\r\n");
    }
#if 0
    else if (meige->ux_host_class_meige_device->ux_device_descriptor.bDeviceClass == 0x00)  //Qualcomm
    {
        meige->ux_host_class_meige_data_interface = 3;
        log_i("Qualcomm modem,data interface is 3\r\n");
    }
#endif
    else if (meige->ux_host_class_meige_device->ux_device_descriptor.bDeviceClass == 0x00)  //MEIGE
    {
        meige->ux_host_class_meige_data_interface = 5;
        log_i("MEIGE modem, data interface is 5\r\n");
    }

    /* If the operation went well, the meige default alternate setting for the meige interface is
       active. We have to memorize the data interface since the bulk in/out endpoints are hooked to it.*/
    status = _ux_host_stack_configuration_interface_get(configuration, meige->ux_host_class_meige_data_interface, 0, &meige->ux_host_class_meige_interface);
    if (status == UX_SUCCESS)
    {
        /* Store the instance in the interface container, this is for the USB stack
           when it needs to invoke the class.  */        
        meige->ux_host_class_meige_interface->ux_interface_class_instance =  (VOID *)meige;

        /* Store the class container in the interface.  The device has the correct class, duplicate it to the interface. */
        meige->ux_host_class_meige_interface->ux_interface_class =  meige->ux_host_class_meige_device->ux_device_class;
    }

    /* Return completion status. */
    return(status);
}

