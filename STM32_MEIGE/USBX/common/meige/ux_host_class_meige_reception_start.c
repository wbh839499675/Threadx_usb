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
/**   Host MEIGE module class                                             */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_class_meige.h"
#include "ux_host_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_host_class_meige_reception_start                 PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts a reception with the Sierra modem. This way    */
/*    allows for non blocking calls based on a packet orientated round    */
/*    robbin buffer. When a packet is fully or partially received, an     */
/*    application callback function is invoked and a new transfer request */
/*    is rescheduled.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    meige                               Pointer to meige class          */
/*    meige_reception                     Pointer to reception struct     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_host_stack_class_instance_verify  Verify the class instance     */
/*    _ux_host_stack_transfer_request       Process transfer request      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
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
UINT _ux_host_class_meige_reception_start(UX_HOST_CLASS_MEIGE *meige,
                                    UX_HOST_CLASS_MEIGE_RECEPTION *meige_reception)
{
    UX_TRANSFER *transfer_request;
    UINT        status;

    /* If trace is enabled, insert this event into the trace buffer. */
    //UX_TRACE_IN_LINE_INSERT(UX_TRACE_HOST_CLASS_MEIGE_RECEPTION_START, meige, 0, 0, 0, UX_TRACE_HOST_CLASS_EVENTS, 0, 0)

    /* Ensure the instance is valid. */
    //if (_ux_host_stack_class_instance_verify(_ux_system_host_class_meige_name, (VOID *)meige) != UX_SUCCESS)
    //status = _ux_host_stack_class_instance_verify(_ux_system_host_class_meige_name, (VOID *)meige);
    status = UX_SUCCESS;

    if (status != UX_SUCCESS)
    {
        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_HOST_CLASS_INSTANCE_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        //UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_HOST_CLASS_INSTANCE_UNKNOWN, meige, 0, 0, UX_TRACE_ERRORS, 0, 0)
        printf("%s failed...line = %d, ret = 0x%02x\r\n", __FILE__, __LINE__, status);
        return(UX_HOST_CLASS_INSTANCE_UNKNOWN);
    }

    /* Start by aligning the head and tail of buffers to the same address supplied by the application. */
    meige_reception->ux_host_class_meige_reception_data_head = meige_reception->ux_host_class_meige_reception_data_buffer;
    meige_reception->ux_host_class_meige_reception_data_tail = meige_reception->ux_host_class_meige_reception_data_buffer;

    /* Get the pointer to the bulk in endpoint in the transfer_request. */
    transfer_request =  &meige->ux_host_class_meige_bulk_in_endpoint->ux_endpoint_transfer_request;

    /* Initialize the transfer request. */
    transfer_request->ux_transfer_request_class_instance      = (VOID *)meige;
    transfer_request->ux_transfer_request_data_pointer        = meige_reception->ux_host_class_meige_reception_data_head;
    transfer_request->ux_transfer_request_requested_length    = meige_reception->ux_host_class_meige_reception_block_size;
    transfer_request->ux_transfer_request_completion_function = _ux_host_class_meige_reception_callback;

    /* Save the acm reception structure in the acm structure. */
    meige->ux_host_class_meige_reception = meige_reception;

    /* And declare we have a transfer in progress. */
    meige_reception->ux_host_class_meige_reception_state = UX_HOST_CLASS_MEIGE_RECEPTION_STATE_STARTED;

    /* Arm a first transfer on the bulk in endpoint. There is a callback to this function so we return to the caller right away. */
    status = _ux_host_stack_transfer_request(transfer_request);

    /* We do not know if the first transfer was successful yet. If the status is not OK, we need to stop the transfer
       in progress flag. */
    if (status != UX_SUCCESS)
        meige_reception->ux_host_class_meige_reception_state = UX_HOST_CLASS_MEIGE_RECEPTION_STATE_STOPPED;

    return(status);
}
