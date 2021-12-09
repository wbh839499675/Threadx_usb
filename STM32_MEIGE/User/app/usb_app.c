/*
********************************************************************************
* Copyright     :
* Filename      : USB_app.c
* Description   :
* Author        :
* Version       : V1.0
* Date          : 2021-12-01
* History       :
********************************************************************************
* Note(s)       :
*
********************************************************************************
*/

/*
********************************************************************************
*                               INCLUDE FILES
********************************************************************************
*/

#define APP_USB_MODULE

#include "includes.h"
#include "ux_api.h"
#include "ux_host_class_meige.h"
#include "ux_system.h"
#include "ux_host_stack.h"
#include "bsp_usb.h"
#include "ux_host_class_cdc_acm.h"

/*
********************************************************************************
*                                LOCAL DEFINES
********************************************************************************
*/
#define USBX_MEMORY_SIZE                (1024 * 64)
#define USBX_APP_STACK_SIZE             (1024)
#define APP_QUEUE_SIZE                  (10)

#define NEW_RECEIVED_DATA               (0x01)
#define NEW_DATA_TO_SEND                (0x02)

#define APP_RX_DATA_SIZE                (2048)
#define BLOCK_SIZE                      (64)


TX_THREAD                               ux_app_thread;
TX_THREAD                               ux_app_recv_thread;
TX_THREAD                               ux_app_send_thread;
TX_EVENT_FLAGS_GROUP                    ux_app_event_flag;

UX_HOST_CLASS_MEIGE_RECEPTION           *app_meige_reception;
//UX_HOST_CLASS_CDC_ACM_RECEPTION           *app_meige_reception;


TX_BYTE_POOL                            *ux_app_byte_pool;
UCHAR                                   *ux_recv_buffer;

void usbx_app_thread_entry(ULONG arg);
void usbx_app_recv_thread_entry(ULONG arg);
void usbx_app_send_thread_entry(ULONG arg);
void ux_meige_reception_callback(struct UX_HOST_CLASS_MEIGE_STRUCT  *meige,
                                            UINT                            status,
                                            UCHAR                           *reception_buffer,
                                            ULONG                           reception_size);

void cdc_acm_reception_callback(struct UX_HOST_CLASS_CDC_ACM_STRUCT  *cdc_acm,
                                            UINT                            status,
                                            UCHAR                           *reception_buffer,
                                            ULONG                           reception_size);
/*
********************************************************************************
*                            LOCAL GLOBAL VARIABLES
********************************************************************************
*/
unsigned char                   host_out_buffer[UX_HOST_CLASS_MEIGE_PACKET_SIZE];
unsigned char                   host_in_buffer[UX_HOST_CLASS_MEIGE_PACKET_SIZE];

UX_HOST_CLASS_MEIGE             *app_cdc_meig;
UX_HOST_CLASS_CDC_ACM           *app_cdc_acm;


TX_QUEUE                        ux_app_message_queue;

ux_app_devInfotypeDef           ux_dev_info;
ux_app_stateTypeDef             ux_app_state;
/*
********************************************************************************
*                                GLOBAL VARIABLES
********************************************************************************
*/
/* Call in usbh_core.c */
uint8_t receive_data[100];

/*
********************************************************************************
*                                   PROTOTYPES
********************************************************************************
*/
extern void USB_Vbus_Init(void);
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);


/*
********************************************************************************
*                                MX_USBX_Host_Init()
*
* Description : Application USBX Host Initialization.
*
* Argument(s) : memory_ptr          memory pointer.
*
* Return(s)   : 
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
UINT MX_USBX_Host_Init(VOID *memory_ptr)
{
    UINT ret = UX_SUCCESS;
    ux_app_byte_pool = (TX_BYTE_POOL *)memory_ptr;
    
    /* USER CODE BEGIN MX_USBX_Host_Init */
    CHAR *pointer;
    
    /* Allocate the stack for USBX */
    if (tx_byte_allocate(ux_app_byte_pool, (void **)&pointer,
                        USBX_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
        printf("tx_byte_allocate failed. %d\r\n", __LINE__);
        return TX_POOL_ERROR;
    }
    
    /* Initialize USBX memory */
    if (ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
    {
        printf("ux_system_initialize failed. %d\r\n", __LINE__);
        return UX_ERROR;
    }

    /* Register a callback error function */
    _ux_utility_error_callback_register(&ux_host_error_callback);

    /* Allocate the stack for thread 0 */
    if (tx_byte_allocate(ux_app_byte_pool, (void **)&pointer,
                        USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
        printf("tx_byte_allocate failed. %d\r\n", __LINE__);
        return TX_POOL_ERROR;
    }

    /* Create the main App thread */
    if (tx_thread_create(&ux_app_thread, "thread 0",
                        usbx_app_thread_entry, 0,
                        pointer, USBX_APP_STACK_SIZE,
                        25, 25, 0, TX_AUTO_START) != TX_SUCCESS)
    {
        printf("tx_thread_create failed. %d\r\n", __LINE__);
        return TX_THREAD_ERROR;
    }
#if 1
    /* Allocate the stack for thread 1 */
    if (tx_byte_allocate(ux_app_byte_pool, (void **)&pointer,
                        USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
        printf("tx_byte_allocate failed. %d\r\n", __LINE__);
        return TX_POOL_ERROR;
    }

    /* Create the recv App thread */
    if (tx_thread_create(&ux_app_recv_thread, "thread 1",
                        usbx_app_recv_thread_entry, 0,
                        pointer, USBX_APP_STACK_SIZE,
                        30, 30, 0, TX_AUTO_START) != TX_SUCCESS)
    {
        printf("tx_thread_create failed. %d\r\n", __LINE__);
        return TX_THREAD_ERROR;
    }
    
    /* Allocate the stack for thread 2 */
    if (tx_byte_allocate(ux_app_byte_pool, (void **)&pointer,
                        USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
    {
        printf("tx_byte_allocate failed. %d\r\n", __LINE__);
        return TX_POOL_ERROR;
    }

    /* Create the send App thread */
    if (tx_thread_create(&ux_app_send_thread, "thread 1",
                        usbx_app_send_thread_entry, 0,
                        pointer, USBX_APP_STACK_SIZE,
                        30, 30, 0, TX_AUTO_START) != TX_SUCCESS)
    {
        printf("tx_thread_create failed. %d\r\n", __LINE__);
        return TX_THREAD_ERROR;
    }
#endif

    /* Allocate Memory for the Queue */
    if (tx_byte_allocate(ux_app_byte_pool, (void **)&pointer,
                        APP_QUEUE_SIZE * sizeof(ULONG), TX_NO_WAIT) != TX_SUCCESS)
    {
        printf("tx_byte_allocate failed. %d\r\n", __LINE__);
        return TX_POOL_ERROR;
    }
    
    /* Create the message queue */
    if (tx_queue_create(&ux_app_message_queue, "message queue app", TX_1_ULONG,
                        pointer, APP_QUEUE_SIZE * sizeof(ULONG)) != TX_SUCCESS)
    {
        printf("tx_queue_create failed. %d\r\n", __LINE__);
        return TX_QUEUE_ERROR;
    }

    /* Create the event flags group */
    if (tx_event_flags_create(&ux_app_event_flag, "event falg") != TX_SUCCESS)
    {
        printf("tx_event_flags_create failed. %d\r\n", __LINE__);
        return TX_GROUP_ERROR;
    }

    return ret;
}
/*
********************************************************************************
*                               usbx_app_thread_entry()
*
* Description : The USB Task entry.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
********************************************************************************
*/
void usbx_app_thread_entry(ULONG arg)
{
    MX_USB_Host_Init();

    /* Start Application */
    log_i("*********%s***************\r\n", _ux_version_id);
    log_i("*********Starting CDC Application****************\r\n");
    log_i("*********Connect your meige Cat.1 Module*********\r\n");

    while (1)
    {
        if (tx_queue_receive(&ux_app_message_queue, &ux_dev_info, TX_WAIT_FOREVER) != TX_SUCCESS)
            log_e("tx queue receive failed.\r\n");

        log_d("*******************************ux_dev_info.Dev_state = %d\r\n", ux_dev_info.Dev_state);
        if (ux_dev_info.Dev_state == Device_connected)
        {
            log_d("MEIGE Cat.1 Device found.\r\n");
            //printf("PID: %#x", (UINT)app_cdc_meig->ux_host_class_meige_device->ux_device_descriptor.idProduct);
            //printf("VID: %#x", (UINT)app_cdc_meig->ux_host_class_meige_device->ux_device_descriptor.idVendor);

            //if (app_cdc_meig->ux_host_class_meige_interface->ux_interface_descriptor.bInterfaceClass == 0x0A)
            if (1)
            {
                ux_app_state = App_Ready;
                log_d("ux app state: App_Ready...............\r\n");
            }
            else
            {
                ux_app_state = App_Idle;
                log_d("ux app state: App_Idle.................\r\n");
            }
        }
        //tx_thread_sleep(MS_TO_TICK(100));
    }
}

void usbx_app_recv_thread_entry(ULONG arg)
{
    UINT        status;
    UCHAR       *ptr_read = NULL;
    ULONG       recv_flag = 0;

    static ULONG    allocate_flag = 0;
    
    log_i("start.......\r\n");

    while (1)
    {
        switch (ux_app_state)
        {
            case App_Ready:
                log_d("ux app recv thread entry....App ready.\r\n");
#if 1
                /* Allocate memory form usb_pool for user reception buffer */
                if (allocate_flag == 0)
                {
                    if (tx_byte_allocate(ux_app_byte_pool, (VOID **)&ux_recv_buffer,
                                        APP_RX_DATA_SIZE, TX_NO_WAIT) != TX_SUCCESS)
                    {
                        printf("tx byte allocate for recv failed.\r\n");
                    }
                    else
                    {
                        allocate_flag = 1;
                    }
                }

                ptr_read = ux_recv_buffer;

                app_meige_reception->ux_host_class_meige_reception_block_size = BLOCK_SIZE;
                _ux_utility_memory_set(ptr_read, 0x00, APP_RX_DATA_SIZE);
                app_meige_reception->ux_host_class_meige_reception_data_buffer = ptr_read;
                app_meige_reception->ux_host_class_meige_reception_data_buffer_size = APP_RX_DATA_SIZE;
                app_meige_reception->ux_host_class_meige_reception_callback = ux_meige_reception_callback;
                status = ux_host_class_meige_reception_start(app_cdc_meig, app_meige_reception);

                if (status == UX_SUCCESS)
                {
                    ux_app_state = App_Start;
                    log_i("ux host class meige reception start success.\r\n");
                }
                else
                {
                    ux_app_state = App_Idle;
                    log_e("ux host class meige reception start failed...status = 0x%02x\r\n", status);
                }
                
#else
                ux_app_state = App_Start;
#endif
            break;

            case App_Start:
                log_i("ux app recv thread entry....App start.\r\n");
                if (tx_event_flags_get(&ux_app_event_flag, NEW_RECEIVED_DATA, TX_OR_CLEAR,
                                       &recv_flag, TX_WAIT_FOREVER) != TX_SUCCESS)
                {
                    log_e("tx event flags get failed.\r\n");
                }

                do
                {
                    log_d("....................recv data.\r\n");
                } while (0);
            break;
            case App_Idle:
            default:
                tx_thread_sleep(MS_TO_TICK(100));
            break;
        }
    }
}

void usbx_app_send_thread_entry(ULONG arg)
{
    UINT        status = 0;
    ULONG       send_flag = 0;

    log_i("start.......\r\n");

    while (1)
    {
        if (tx_event_flags_get(&ux_app_event_flag,
                                NEW_DATA_TO_SEND,
                                TX_OR_CLEAR,
                                &send_flag,
                                TX_WAIT_FOREVER) != TX_SUCCESS)
        {
            log_e("tx event flags get failed.\r\n");
        }
        else
        {
            log_d("please send data here\r\n");
            tx_thread_sleep(MS_TO_TICK(100));
        }
    }
}

void ux_meige_reception_callback(struct UX_HOST_CLASS_MEIGE_STRUCT *meige,
                                                                UINT  status,
                                                                UCHAR *reception_buffer,
                                                                ULONG reception_size)
{
    log_i("%s\r\n", __func__);
}
void cdc_acm_reception_callback(struct UX_HOST_CLASS_CDC_ACM_STRUCT  *cdc_acm,
                                            UINT                            status,
                                            UCHAR                           *reception_buffer,
                                            ULONG                           reception_size)
{
    if (tx_event_flags_set(&ux_app_event_flag, NEW_RECEIVED_DATA, TX_OR) != TX_SUCCESS)
    {
        log_e("tx event flags set failed\r\n");
    }
}


/*
********************************************************************************
*                                ux_host_event_callback()
*
* Description : 
*
* Argument(s) : 
*
* Return(s)   : 
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/

UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *Current_class, VOID *Current_instance)
{
    UINT            status_host, status_cdc;
    UX_HOST_CLASS   *host_class;
    UX_HOST_CLASS   *cdc_class;
    UX_DEVICE       *current_device;
    
    //tx_thread_sleep(MS_TO_TICK(100));
    //printf("%s event = %d\r\n", __func__, (UINT)event);
    switch (event)
    {
        case UX_DEVICE_CONNECTION:
#if 0
            //printf("..............device connected\r\n");
            status = ux_host_stack_class_get(_ux_system_host_class_meige_name, &host_class);
            //status = ux_host_stack_class_get(_ux_system_host_class_cdc_acm_name, &host_class);
            if (status != UX_SUCCESS)
            {
                printf("ux_host_stack_class_get failed\r\n");
                return UX_ERROR;
            }

            if (host_class == Current_class)
            {
                printf("host_class == Current_class\r\n");
            }
            ux_dev_info.Dev_state = Device_connected;
            tx_queue_send(&ux_app_message_queue, &ux_dev_info, TX_NO_WAIT);
#endif
        break;

        case UX_DEVICE_DISCONNECTION:
            ux_dev_info.Dev_state = Device_disconnect;
            tx_queue_send(&ux_app_message_queue, &ux_dev_info, TX_NO_WAIT);
        break;
#if 1
        case UX_DEVICE_INSERTION:
            /* Get MEIGE Class */
            status_host = ux_host_stack_class_get(_ux_system_host_class_meige_name, &host_class);
            status_cdc = ux_host_stack_class_get(_ux_system_host_class_cdc_acm_name, &cdc_class);

            if ((status_host != UX_SUCCESS) && (status_cdc != UX_SUCCESS))
            {
                log_e("ux_host_stack_class_get failed, status_host = 0x%02x    status_cdc = 0x%02x\r\n",
                        status_host, status_cdc);
                return UX_ERROR;
            }

            if ((memcmp(host_class->ux_host_class_name,
                        Current_class->ux_host_class_name,
                        UX_MAX_CLASS_NAME_LENGTH) == 0)
                 /*&&
                (app_cdc_acm == NULL) */)
            {
                ux_dev_info.Dev_state = Device_connected;
                app_cdc_acm = Current_class;

                /* Check if the Class different from the actual instance */
#if 0
                if (app_cdc_acm->ux_host_class_cdc_acm_device->ux_device_state !=
                    (ULONG)UX_HOST_CLASS_INSTANCE_LIVE)
                {
                    ux_dev_info.Device_Type = 0;
                }
                else
                {
                    ux_dev_info.Device_Type = 0;
                }
#endif
                tx_queue_send(&ux_app_message_queue, &ux_dev_info, TX_NO_WAIT);
                log_d("UX DEVICE INSERTION!!!!!!!!!!!!!!!!!!!\r\n");
            }
            else
            {
                log_e("NO Class found...host_class->ux_host_class_name = %s, Current_class->ux_host_class_name\r\n",
                    host_class->ux_host_class_name, Current_class->ux_host_class_name);
            }
        break;

        case UX_DEVICE_REMOVAL:
            if (app_cdc_meig != NULL)
            {
                
                app_cdc_meig = NULL;
            }
            log_d("UX DEVICE REMOVAL!!!!!!!!!!!!!!!!!!!\r\n");
            ux_dev_info.Dev_state = Device_disconnect;
            tx_queue_send(&ux_app_message_queue, &ux_dev_info, TX_NO_WAIT);
        break;
#endif
        default:
        break;
    }

    return UX_SUCCESS;
}

/*
********************************************************************************
*                                ux_host_error_callback()
*
* Description : ux_host_error_callback.
*
* Argument(s) : system_level
*               system_context
*               error_code
*
* Return(s)   : none
*
* Caller(s)   : Application
*
* Note(s)     : none
********************************************************************************
*/
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE :
      //ux_dev_info.Device_Type = Unknown_Device;
      //ux_dev_info.Dev_state   = Device_connected;
      //USBH_ErrLog("Enumeration Failure");
      //tx_queue_send(&ux_app_MsgQueue, &ux_dev_info, TX_NO_WAIT);
      break;

    case UX_NO_DEVICE_CONNECTED :
      //USBH_UsrLog("USB Device disconnected");
      break;

    default:
      break;
  }
}
void error_handler(void)
{

    /* Increment error counter.  */
    //error_counter++;

    while(1)
    {
    
        /* Error - just spin here!  Look at call tree in debugger 
           to see where the error occurred.  */
    }
}
