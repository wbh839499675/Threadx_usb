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
/**   Host Stack                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_host_stack.h"
#include "includes.h"

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_host_stack_interface_instance_create            PORTABLE C      */ 
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function will create an interface instance. It creates each    */
/*    endpoint associated with the interface.                             */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    interface                             Pointer to interface          */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_host_stack_endpoint_instance_create Create instance endpoint    */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    USBX Components                                                     */ 
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
UINT _ux_host_stack_interface_instance_create(UX_INTERFACE *interface)
{
    UX_ENDPOINT     *endpoint;
    UINT            status;

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_HOST_STACK_INTERFACE_INSTANCE_CREATE, interface, 0, 0, 0, UX_TRACE_HOST_STACK_EVENTS, 0, 0)

    /* Obtain the first endpoint for this alternate setting.  */
    endpoint = interface->ux_interface_first_endpoint;
    //printf("%s ...line = %d, endpoint = 0x%x\r\n", __FILE__, __LINE__, endpoint);
/*
    typedef struct UX_ENDPOINT_DESCRIPTOR_STRUCT
    {
    
        ULONG           bLength;
        ULONG           bDescriptorType;
        ULONG           bEndpointAddress;
        ULONG           bmAttributes;
        ULONG           wMaxPacketSize;
        ULONG           bInterval;
    } UX_ENDPOINT_DESCRIPTOR;
*/

    log_i("============Interface Descriptor============\r\nbLength = 0x%02x\r\nbDescriptorType = 0x%02x\r\nbEndpointAddress = 0x%02x\r\nbmAttributes = 0x%02x\r\nwMaxPacketSize = 0x%02x\r\nbInterval = 0x%02x\r\n",
            endpoint->ux_endpoint_descriptor.bLength,
            endpoint->ux_endpoint_descriptor.bDescriptorType,
            endpoint->ux_endpoint_descriptor.bEndpointAddress,
            endpoint->ux_endpoint_descriptor.bmAttributes,
            endpoint->ux_endpoint_descriptor.wMaxPacketSize,
            endpoint->ux_endpoint_descriptor.bInterval);


    /* Loop to create each endpoint.  */
    while (endpoint != UX_NULL)
    {
        /* Create an endpoint for the instance.  */
        status = _ux_host_stack_endpoint_instance_create(endpoint);
        /* Check status, the controller may have refused the endpoint creation.  */
        if (status != UX_SUCCESS)
        {
            log_e("_ux_host_stack_endpoint_instance_create failed...ret = 0x%02x\r\n", status);
            /* An error occurred at the controller level.  */
            return(status);
        }
        /* Move to next endpoint.  */
        endpoint =  endpoint->ux_endpoint_next_endpoint;
    }

    /* If trace is enabled, register this object.  */
    UX_TRACE_OBJECT_REGISTER(UX_TRACE_HOST_OBJECT_TYPE_INTERFACE, interface, 0, 0, 0);

    /* Return completion status.  */
    return(UX_SUCCESS);
}

