/*
*********************************************************************************************************
*
*    ģ������ : BSPģ��(For STM32H7)
*    �ļ����� : bsp.c
*    ��    �� : V1.0
*    ˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*               bsp = Borad surport packet �弶֧�ְ�
*    �޸ļ�¼ :
*        �汾��  ����         ����       ˵��
*
*
*********************************************************************************************************
*/
#include "bsp.h"

#define ARM_MPU_REGION_SIZE_64KB     ((uint8_t)0x0FU) ///!< MPU Region Size 64 KBytes

UART_HandleTypeDef UART3_Handler;
/*
*********************************************************************************************************
*                                       ��������
*********************************************************************************************************
*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
extern void USB_Vbus_Init(void);
extern void bsp_InitDWT(void);
extern void bsp_uart3_init(uint32_t bound);

/*
*********************************************************************************************************
*    �� �� ��: System_Init
*    ����˵��: ϵͳ��ʼ������Ҫ��MPU��Cache��ϵͳʱ������
*    ��    �Σ���
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void System_Init(void)
{
    
    /* ����MPU */
    MPU_Config();
    
    /* ʹ��L1 Cache */
    CPU_CACHE_Enable();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();
    
    bsp_InitDWT();      /* ��ʼ��DWTʱ�����ڼ����� */
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_Init
*    ����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{
    MX_GPIO_Init();
    USB_Vbus_Init();
    bsp_uart3_init(115200);
}

/*
*********************************************************************************************************
*    �� �� ��: SystemClock_Config
*    ����˵��: ��ʼ��ϵͳʱ��
*                System Clock source            = PLL (HSE)
*                SYSCLK(Hz)                     = 400000000 (CPU Clock)
*               HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
*                AHB Prescaler                  = 2
*                D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
*                D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
*                D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
*                D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
*                HSE Frequency(Hz)              = 25000000
*               PLL_M                          = 5
*                PLL_N                          = 160
*                PLL_P                          = 2
*                PLL_Q                          = 4
*                PLL_R                          = 2
*                VDD(V)                         = 3.3
*                Flash Latency(WS)              = 4
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure. */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 24;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        //Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        //Error_Handler();
    }
}

/*
*********************************************************************************************************
*    �� �� ��: Error_Handler
*    ��    ��: file : Դ�����ļ����ơ��ؼ��� __FILE__ ��ʾԴ�����ļ�����
*              line �������кš��ؼ��� __LINE__ ��ʾԴ�����к�
*    �� �� ֵ: ��
*        Error_Handler(__FILE__, __LINE__);
*********************************************************************************************************
*/

void Error_Handler(char *file, uint32_t line)
{
    /* 
        �û���������Լ��Ĵ��뱨��Դ�����ļ����ʹ����кţ����罫�����ļ����кŴ�ӡ������
        printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
    */
    
    /* ����һ����ѭ��������ʧ��ʱ������ڴ˴��������Ա����û���� */
    if (line == 0)
    {
        return;
    }
    
    while(1)
    {
        //BSP_LED_Toggle(LED_RED);
        HAL_Delay(200);
    }
}
/*
*********************************************************************************************************
*    �� �� ��: MPU_Config
*    ����˵��: ����MPU
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void MPU_Config( void )
{
#if 1
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* ��ֹ MPU */
    HAL_MPU_Disable();

    /* ������ܣ���Cache��дCache������ */    
#if 1
       /* ����AXI SRAM��MPU����ΪWrite back, Read allocate��Write allocate */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x24000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* ������ܣ���Cache��дCache���ر� */
#else
    /* ����AXI SRAM��MPU����ΪNORMAL, NO Read allocate��NO Write allocate */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x24000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
#endif
    
    
    /* ����FMC��չIO��MPU����ΪDevice����Strongly Ordered */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x60000000;
    MPU_InitStruct.Size             = ARM_MPU_REGION_SIZE_64KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;    /* ������MPU_ACCESS_CACHEABLE;�����2��CS��WE�ź� */
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*ʹ�� MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
#endif
}

/*
*********************************************************************************************************
*    �� �� ��: CPU_CACHE_Enable
*    ����˵��: ʹ��L1 Cache
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void CPU_CACHE_Enable(void)
{
    /* ʹ�� I-Cache */
    SCB_EnableICache();

    /* ʹ�� D-Cache */
    SCB_EnableDCache();
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(USB_OTG_FS_PWR_EN_GPIO_Port, USB_OTG_FS_PWR_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LD1_Pin LD3_Pin */
    GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_OTG_FS_PWR_EN_Pin */
    GPIO_InitStruct.Pin = USB_OTG_FS_PWR_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_OTG_FS_PWR_EN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : USB_OTG_FS_OVCR_Pin */
    GPIO_InitStruct.Pin = USB_OTG_FS_OVCR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_OTG_FS_OVCR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LD2_Pin */
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}
