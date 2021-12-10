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


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    ux_host_class_meige.h                                PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains all the header and extern functions used by the  */
/*    USBX MEIGE Class.                                                   */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            used UX prefix to refer to  */
/*                                            TX symbols instead of using */
/*                                            them directly,              */
/*                                            resulting in version 6.1    */
/*  01-12-2021     Meige Team               Modified to port SLMxxx       */
/*                                                                        */
/**************************************************************************/

#ifndef UX_HOST_CLASS_MEIGE_H
#define UX_HOST_CLASS_MEIGE_H

//#include "main.h"
#include "ux_api.h"

/* Define MEIGE Class constants. */

#define UX_HOST_CLASS_MEIGE_CLASS_TRANSFER_TIMEOUT               10000

#ifndef UX_HOST_CLASS_MEIGE_VENDOR_ID
#define UX_HOST_CLASS_MEIGE_VENDOR_ID                            0x2DEE
#endif

/* Define MEIGE Class Instance class */
#define UX_HOST_CLASS_VENDOR_SPECIFIC_CLASS                      0xFF

/* Define MEIGE Class packet equivalences. */
#define UX_HOST_CLASS_MEIGE_PACKET_SIZE                          128

/* Define MEIGE Class data interface. */
#define UX_HOST_CLASS_MEIGE_DATA_INTERFACE                       0    //Qualcomm 2 AT  3 PPP  ;  ASR 3 AT  4 PPP; MEIGE 0 AT  3 PPP

/* Define MEIGE IOCTL functions. */
#define UX_HOST_CLASS_MEIGE_IOCTL_ABORT_IN_PIPE                  1
#define UX_HOST_CLASS_MEIGE_IOCTL_ABORT_OUT_PIPE                 2

/* Define CDC ACM Reception States. */

#define UX_HOST_CLASS_MEIGE_RECEPTION_STATE_STOPPED              0
#define UX_HOST_CLASS_MEIGE_RECEPTION_STATE_STARTED              1
#define UX_HOST_CLASS_MEIGE_RECEPTION_STATE_IN_TRANSFER          2


/* Define MEIGE Class instance structure. */

typedef struct UX_HOST_CLASS_MEIGE_STRUCT
{
#if 0
    struct UX_HOST_CLASS_MEIGE_STRUCT *ux_host_class_meige_next_instance;
    UX_HOST_CLASS   *ux_host_class_meige_class;
    UX_DEVICE       *ux_host_class_meige_device;
    UX_INTERFACE    *ux_host_class_meige_interface;
    UX_ENDPOINT     *ux_host_class_meige_bulk_out_endpoint;
    UX_ENDPOINT     *ux_host_class_meige_bulk_in_endpoint;
    UINT            ux_host_class_meige_state;
    UX_SEMAPHORE    ux_host_class_meige_semaphore;

    struct UX_HOST_CLASS_MEIGE_RECEPTION_STRUCT *ux_host_class_meige_reception;
    ULONG           ux_host_class_meige_notification_count;
    UINT            ux_host_class_meige_data_interface;
#else
    struct UX_HOST_CLASS_MEIGE_STRUCT *ux_host_class_meige_next_instance;

    UX_HOST_CLASS   *ux_host_class_meige_class;
    UX_DEVICE       *ux_host_class_meige_device;
    UX_ENDPOINT     *ux_host_class_meige_bulk_out_endpoint;
    UX_ENDPOINT     *ux_host_class_meige_bulk_in_endpoint;
    UX_INTERFACE    *ux_host_class_meige_interface;
    UINT            ux_host_class_meige_instance_status;
    UINT            ux_host_class_meige_state;
    UX_SEMAPHORE    ux_host_class_meige_semaphore;
    ULONG           ux_host_calss_meige_notification_count;
    UCHAR           ux_host_calss_meige_capabilities;
    ULONG           ux_host_class_meige_device_state;

    struct UX_HOST_CLASS_MEIGE_RECEPTION_STRUCT *ux_host_class_meige_reception;

    VOID  (*ux_host_class_meige_device_status_change_callback)
                    (struct UX_HOST_CLASS_MEIGE_STRUCT *meigem,
                    ULONG notification_type, ULONG notification_value);
    UINT            ux_host_class_meige_data_interface;
#endif
} UX_HOST_CLASS_MEIGE;

/* Define MEIGE reception structure. */

typedef struct UX_HOST_CLASS_MEIGE_RECEPTION_STRUCT
{
    ULONG           ux_host_class_meige_reception_state;
    ULONG           ux_host_class_meige_reception_block_size;
    UCHAR           *ux_host_class_meige_reception_data_buffer;
    ULONG           ux_host_class_meige_reception_data_buffer_size;
    UCHAR           *ux_host_class_meige_reception_data_head;
    UCHAR           *ux_host_class_meige_reception_data_tail;
    VOID            (*ux_host_class_meige_reception_callback)(struct UX_HOST_CLASS_MEIGE_STRUCT *meige,
                                                                UINT  status,
                                                                UCHAR *reception_buffer,
                                                                ULONG reception_size);

} UX_HOST_CLASS_MEIGE_RECEPTION;

/* Define MEIGE Class function prototypes. */

UINT _ux_host_class_meige_activate(UX_HOST_CLASS_COMMAND *command);
UINT _ux_host_class_meige_configure(UX_HOST_CLASS_MEIGE *meige);
UINT _ux_host_class_meige_deactivate(UX_HOST_CLASS_COMMAND *command);
UINT _ux_host_class_meige_endpoints_get(UX_HOST_CLASS_MEIGE *meige);
UINT _ux_host_class_meige_entry(UX_HOST_CLASS_COMMAND *command);
UINT _ux_host_class_meige_read (UX_HOST_CLASS_MEIGE *meige, UCHAR *data_pointer,
                                    ULONG requested_length, ULONG *actual_length);
UINT _ux_host_class_meige_write(UX_HOST_CLASS_MEIGE *meige, UCHAR *data_pointer,
                                    ULONG requested_length, ULONG *actual_length);
UINT _ux_host_class_meige_ioctl(UX_HOST_CLASS_MEIGE *meige, ULONG ioctl_function,
                                    VOID *parameter);
VOID _ux_host_class_meige_reception_callback(UX_TRANSFER *transfer_request);
UINT _ux_host_class_meige_reception_stop(UX_HOST_CLASS_MEIGE *meige,
                                    UX_HOST_CLASS_MEIGE_RECEPTION *meige_reception);
UINT _ux_host_class_meige_reception_start(UX_HOST_CLASS_MEIGE *meige,
                                    UX_HOST_CLASS_MEIGE_RECEPTION *meige_reception);

/* Define MEIGE Class API prototypes.  */

#define ux_host_class_meige_entry                           _ux_host_class_meige_entry
#define ux_host_class_meige_read                            _ux_host_class_meige_read
#define ux_host_class_meige_write                           _ux_host_class_meige_write
#define ux_host_class_meige_ioctl                           _ux_host_class_meige_ioctl
#define ux_host_class_meige_command                         _ux_host_class_meige_command
#define ux_host_class_meige_reception_stop                  _ux_host_class_meige_reception_stop
#define ux_host_class_meige_reception_start                 _ux_host_class_meige_reception_start
#define ux_host_class_meige_setup                           _ux_host_class_meige_setup

#endif
