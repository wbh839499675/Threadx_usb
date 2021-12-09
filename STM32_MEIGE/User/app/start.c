/*
********************************************************************************
*                              START SYSTEM
********************************************************************************
*
* Filename      : start.c
* Version       : V1.0.0
* Programmer(s) :  
********************************************************************************
* Note(s)       : 1) The only job for the main is to create the root task whose 
*                    subsequent reponsibility is to process those unfinished business
*                    after board initialization.
*    
*                 2) Root task will take over the control of system while bsp finish its initialization.
*
*                 3) Since the RTOS have to achieve startup before some initialization, It is necessary
*                    to hand these initialization to the root task.
********************************************************************************
*/

/*
********************************************************************************
*                              TASK PRIORITY
********************************************************************************
*
* 0:    OS_IntQTask(),  interrupt queue management task.
* 1:    Task_Start(),   root task.
* 3:    OS_LedTask(),   LED display.
* 4-29: None.
* 30:   OS_StatTask(),  compute some statistics about the multitasking environment.
* 31:   OS_IdleTask(),  executes whenever no other higher priority tasks executes.
********************************************************************************
*/

#define USR_CFG_GLOBAL_DEF

/*
********************************************************************************
*                             INCLUDES
********************************************************************************
*/
#include "includes.h"

/*
********************************************************************************
*                           TASK PRIORITY
********************************************************************************
*/
#define  APP_CFG_TASK_START_PRIO                          2u
#define  APP_CFG_TASK_USB_PRIO                            3u
#define  APP_CFG_TASK_LED_PRIO                            4u
#define  APP_CFG_TASK_KEY_PRIO                            5u
#define  APP_CFG_TASK_STAT_PRIO                           30u
#define  APP_CFG_TASK_IDLE_PRIO                           31u


/*
********************************************************************************
*                         DEFINE TASK STACK SIZE
********************************************************************************
*/
#define APP_CFG_TASK_START_STK_SIZE                     4096u
#define APP_CFG_TASK_USB_STK_SIZE                       (1024 * 200)
#define APP_CFG_TASK_LED_STK_SIZE                       1024u
#define APP_CFG_TASK_KEY_STK_SIZE                       1024u
#define APP_CFG_TASK_IDLE_STK_SIZE                      1024u

/*
********************************************************************************
*                           DEFINE TCB AND STACK
********************************************************************************
*/
static TX_THREAD        tcb_task_start;
static uint64_t         stk_task_start[APP_CFG_TASK_START_STK_SIZE / 8];

static TX_THREAD        tcb_task_usb;
static uint64_t         stk_task_usb[APP_CFG_TASK_USB_STK_SIZE / 8];

static TX_THREAD        tcb_task_led;
static uint64_t         stk_task_led[APP_CFG_TASK_LED_STK_SIZE / 8];

static TX_THREAD        tcb_task_key;
static uint64_t         stk_task_key[APP_CFG_TASK_KEY_STK_SIZE / 8];

static TX_THREAD        tcb_task_idle;
static uint64_t         stk_task_idle[APP_CFG_TASK_IDLE_STK_SIZE / 8];
/*
********************************************************************************
*                              PROTOTYPES
********************************************************************************
*/
static void Task_Start(ULONG thread_input);
extern void Task_USB(ULONG thread_input);
extern void Task_LED(ULONG thread_input);
extern void Task_KEY(ULONG thread_input);
static void Task_IDLE(ULONG thread_input);
static void AppTaskCreate(void);
extern UINT MX_USBX_Host_Init(VOID *memory_ptr);

extern void userShellInit(void);
/*
********************************************************************************
*                             LOCAL VARIABLE
********************************************************************************
*/
__IO uint8_t            OSStatRdy;
__IO uint32_t           OSIdleCtr;
__IO float              OSCPUUsage;
uint32_t                OSIdleCtrMax;
uint32_t                OSIdleCtrRun;

#if 1   //add for USBX Thread
__attribute__((section(".UsbPoolSection")))
#define UX_HOST_APP_MEM_POOL_SIZE       (1024 * 70)
static UCHAR            ux_host_byte_pool_buffer[UX_HOST_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL     ux_host_app_byte_pool;
#endif
/*
********************************************************************************
*                                main()
*
* Description : Ceate root task and start RTOS.
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
int main(void)
{
    System_Init();
    //HAL_SuspendTick();
    bsp_Init();

    tx_kernel_enter();

    while(1);
}

void tx_application_define(void *first_unused_memory)
{
    /**************Create Start Task*********************/
    tx_thread_create(&tcb_task_start,
                       "The Start Task",
                       Task_Start,
                       0,
                       &stk_task_start[0],
                       APP_CFG_TASK_START_STK_SIZE,
                       APP_CFG_TASK_START_PRIO,
                       APP_CFG_TASK_START_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);
#if 1   //add for USB thread
    VOID *memory_ptr;
    if (tx_byte_pool_create(&ux_host_app_byte_pool, "Ux App memory pool",
                            ux_host_byte_pool_buffer, UX_HOST_APP_MEM_POOL_SIZE) != TX_SUCCESS)
    {
        printf("tx_byte_pool_create failed. %d\r\n", __LINE__);
    }
    else
    {
        memory_ptr = (VOID *)&ux_host_app_byte_pool;

        if (MX_USBX_Host_Init(memory_ptr) != UX_SUCCESS)
        {
            printf("MX_USBX_Host_Init failed. %d\r\n", __LINE__);
        }
    }

#endif
}
void OSStatInit(void)
{
    OSStatRdy = 0;

    tx_thread_sleep(2u);
    OSIdleCtr = 0uL;
    tx_thread_sleep(100);

    OSIdleCtrMax = OSIdleCtr;
    OSStatRdy = 1;
}
/*
********************************************************************************
*                                Task_Start()
*
* Description : Root Task created by main.
*
* Argument(s) : p_arg       this pointer is not used at this time.
*
* Return(s)   : none
*
* Caller(s)   : none
*
* Note(s)     : Because of its high priority, no task is able to interrupt it except OS_IntQTask.
********************************************************************************
*/
//__weak void Task_StartHook(TX_THREAD *p_tcb) { (void *)p_tcb; }
static void Task_Start(ULONG thread_input)
{
    (void)thread_input;

    OSStatInit();
    HAL_ResumeTick();
    bsp_Init();

    userShellInit();
    AppTaskCreate();                /* Create Appliction tasks */


    //Task_StartHook(&tcb_task_start);

    while (1)
    {
        if (TX_SUSPEND_ERROR == tx_thread_suspend(&tcb_task_start))
            return;
    }
}

/*
********************************************************************************
*                               AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none.
*
* Return(s)   : none
*
* Caller(s)   : Task_Start.
*
* Note(s)     : none.
********************************************************************************
*/
static void AppTaskCreate(void)
{
    /********************* Create USB Task *******************/
#if 0
    tx_thread_create(&tcb_task_usb,
                       "The USB Task",
                       Task_USB,
                       0,
                       &stk_task_usb[0],
                       APP_CFG_TASK_USB_STK_SIZE,
                       APP_CFG_TASK_USB_PRIO,
                       APP_CFG_TASK_USB_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);
 #endif
    /********************* Create LED Task *******************/
    tx_thread_create(&tcb_task_led,
                       "The LED Task",
                       Task_LED,
                       0,
                       &stk_task_led[0],
                       APP_CFG_TASK_LED_STK_SIZE,
                       APP_CFG_TASK_LED_PRIO,
                       APP_CFG_TASK_LED_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);

    /********************* Create KEY Task *******************/
    tx_thread_create(&tcb_task_key,
                       "The KEY Task",
                       Task_KEY,
                       0,
                       &stk_task_key[0],
                       APP_CFG_TASK_KEY_STK_SIZE,
                       APP_CFG_TASK_KEY_PRIO,
                       APP_CFG_TASK_KEY_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);

    /********************* Create IDLE Task *******************/
    tx_thread_create(&tcb_task_idle,
                       "The IDLE Task",
                       Task_IDLE,
                       0,
                       &stk_task_idle[0],
                       APP_CFG_TASK_IDLE_STK_SIZE,
                       APP_CFG_TASK_IDLE_PRIO,
                       APP_CFG_TASK_IDLE_PRIO,
                       TX_NO_TIME_SLICE,
                       TX_AUTO_START);
}

static void Task_IDLE(ULONG thread_input)
{
    TX_INTERRUPT_SAVE_AREA

    (void)thread_input;

    while(1)
    {
        TX_DISABLE
        OSIdleCtr++;
        TX_RESTORE
    } 
}
