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
/**   Host Sierra Wireless AR module class                                */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_meige.h"
#include "ux_host_stack.h"
#include "includes.h"
#include "ux_api.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_host_class_meige_entry                           PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the entry point of the meige class. It will be     */
/*    called by the USBX stack enumeration module when there is a new     */
/*    meige on the bus or when the USB meige is removed.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    command                                     meige class command     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_host_class_meige_activate         Activate meige class          */
/*    _ux_host_class_meige_deactivate       Deactivate meige class        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Data pump Class                                                     */
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
UINT _ux_host_class_meige_entry(UX_HOST_CLASS_COMMAND *command)
{
    UINT status;

    /* The command request will tell us we need to do here, either a enumeration
       query, an activation or a deactivation.  */
    switch (command->ux_host_class_command_request)
    {

        case UX_HOST_CLASS_COMMAND_QUERY:
            /* The query command is used to let the stack enumeration process know if we want to own
               this device or not.  */
            log_i("ux_host_class_command_usage = 0x%04x, ux_host_class_command_vid = 0x%08x\r\n",
                    command->ux_host_class_command_usage,
                    command->ux_host_class_command_vid);
            if ((command->ux_host_class_command_usage == UX_HOST_CLASS_COMMAND_USAGE_PIDVID) &&
                (command->ux_host_class_command_vid == UX_HOST_CLASS_MEIGE_VENDOR_ID))
            {
                return (UX_SUCCESS);
            }
            else
            {
                log_e("USBX NO CLASS MATCH\r\n");
                return (UX_NO_CLASS_MATCH);
            }

        case UX_HOST_CLASS_COMMAND_ACTIVATE:

            /* The activate command is used when the device inserted has found a parent and
               is ready to complete the enumeration. */
            status =  _ux_host_class_meige_activate(command);
            return (status);

        case UX_HOST_CLASS_COMMAND_DEACTIVATE:
            /* The deactivate command is used when the device has been extracted either
               directly or when its parents has been extracted. */
            status = _ux_host_class_meige_deactivate(command);
            return (status);

        default: 
            /* Error trap. */
            _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_FUNCTION_NOT_SUPPORTED);
            /* If trace is enabled, insert this event into the trace buffer.  */
            UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_FUNCTION_NOT_SUPPORTED, 0, 0, 0, UX_TRACE_ERRORS, 0, 0)

            log_e("UX_FUNCTION_NOT_SUPPORTED\r\n");
            return (UX_FUNCTION_NOT_SUPPORTED);
    }
}

