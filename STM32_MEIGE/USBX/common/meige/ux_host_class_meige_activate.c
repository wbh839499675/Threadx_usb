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


/* Include necessary system files. */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_meige.h"
#include "ux_host_stack.h"
#include "includes.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_host_class_meige_activate                        PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calls the USBX stack to activate the class.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    command                               Dpump class command pointer   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_host_class_meige_configure        Configure meige class         */
/*    _ux_host_class_meige_endpoints_get    Get endpoints of meige        */
/*    _ux_host_stack_class_instance_create  Create class instance         */
/*    _ux_host_stack_class_instance_destroy Destroy the class instance    */
/*    _ux_utility_memory_allocate           Allocate memory block         */
/*    _ux_utility_memory_free               Free memory block             */
/*    _ux_utility_semaphore_create          Create meige semaphore        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _ux_host_class_meige_entry            Entry of meige class          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _ux_host_class_meige_activate(UX_HOST_CLASS_COMMAND *command)
{
    UX_DEVICE               *device;
    UX_HOST_CLASS_MEIGE     *meige;
    UINT                    status;

    /* The MEIGE class is always activated by the device descriptor. */
    device = (UX_DEVICE *)command->ux_host_class_command_container;

    /* Obtain memory for this class instance. */
    meige = _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(UX_HOST_CLASS_MEIGE));
    if (meige == UX_NULL)
        return(UX_MEMORY_INSUFFICIENT);

    /* Store the class container into this instance. */
    meige->ux_host_class_meige_class = command->ux_host_class_command_class_ptr;

    /* Store the device container into the meige class instance. */
    meige->ux_host_class_meige_device = device;

    /* Store the instance in the device container, this is for the USBX stack
       when it needs to invoke the class for deactivation. */
    device->ux_device_class_instance = (VOID *)meige;

    /* Create this class instance. */
    status = _ux_host_stack_class_instance_create(meige->ux_host_class_meige_class, (VOID *)meige);
    if (status != UX_SUCCESS)
    {
        log_e("_ux_host_stack_class_instance_create failed...ret = 0x%02x\r\n", status);
    }

    /* Configure the meige. */
    status = _ux_host_class_meige_configure(meige);
    if (status != UX_SUCCESS)
    {
        log_e("_ux_host_class_meige_configure failed...ret = 0x%02x\r\n", status);
    }
    /* Get the meige endpoint(s). We will need to search for Bulk Out and Bulk In endpoints on interface . */
    if (status == UX_SUCCESS)
        status = _ux_host_class_meige_endpoints_get(meige);

    /* Create the semaphore to protect 2 threads from accessing the same meige instance.  */
    if (status == UX_SUCCESS)
    {
        status = _ux_utility_semaphore_create(&meige->ux_host_class_meige_semaphore, "ux_host_class_meige_semaphore", 1);
        if (status != UX_SUCCESS)
            status = UX_SEMAPHORE_ERROR;
    }

    /* Success things. */
    if (status == UX_SUCCESS)
    {

        /* Mark the meige as live now. */
        meige->ux_host_class_meige_state = UX_HOST_CLASS_INSTANCE_LIVE;

        /* If all is fine and the device is mounted, we may need to inform the application
        if a function has been programmed in the system structure. */
        if (_ux_system_host->ux_system_host_change_function != UX_NULL)
        {

            /* Call system change function. */
            _ux_system_host->ux_system_host_change_function(UX_DEVICE_INSERTION, meige->ux_host_class_meige_class, (VOID *) meige);
        }

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_HOST_CLASS_MEIGE_ACTIVATE, meige, 0, 0, 0, UX_TRACE_HOST_CLASS_EVENTS, 0, 0)

        /* If trace is enabled, register this object. */
        UX_TRACE_OBJECT_REGISTER(UX_TRACE_HOST_OBJECT_TYPE_INTERFACE, meige, 0, 0, 0)

        /* Return success. */
        return(UX_SUCCESS);
    }

    /* There was a problem during the configuration, so free the resources. */
    /* The last resource, semaphore is not created or created error, no need to free. */
    _ux_host_stack_class_instance_destroy(meige->ux_host_class_meige_class, (VOID *)meige);
    device->ux_device_class_instance = UX_NULL;
    _ux_utility_memory_free(meige);

    /* Return completion status.  */
    return(status);
}

