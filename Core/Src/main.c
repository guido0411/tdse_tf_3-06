/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "cod_display.h"

///Defino la seccion de memoria que voy a utilizar para guardar modo
#define FLASH_USER_START_ADDR  0x0801FC00
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
	volatile uint8_t adc_flag = 0; ///Flag para leer valor del adc
	volatile uint8_t timer_flag = 0;
	///MANEJO DE MEMORIA
	typedef struct {
	    uint32_t firma;
	    uint32_t modo;
	    uint32_t temp_usuario;
	} Config;

	#define CONFIG_FIRMA 0xDEADBEEF

	///FUNCION PARA ESCRIBIR EN LA FLASH
	void Flash_Write_Config(Config *config)
	{
	    HAL_FLASH_Unlock();

	    FLASH_EraseInitTypeDef EraseInitStruct;
	    uint32_t PageError;

	    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
	    EraseInitStruct.NbPages = 1;

	    HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	    uint32_t *data = (uint32_t*) config;
	    for (int i = 0; i < sizeof(Config)/4; i++)
	    {
	        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
	                          FLASH_USER_START_ADDR + i*4,
	                          data[i]);
	    }

	    HAL_FLASH_Lock();
	}

	///FUNCION PARA LEER MEMORIA

	void Flash_Read_Config(Config *config)
	{
	    memcpy(config, (void*)FLASH_USER_START_ADDR, sizeof(Config));
	}
	///ADC FLAG!!!!
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
	    if (htim->Instance == TIM2)
	    {
	       adc_flag = 1;

	    }
	}

	static uint8_t rx_buf[32];
	static char* tx_1 = "Bienvenido, ingrese el modo que desea:\n Opciones: MATE - TE - SET [XX] - REPETIR\r\n";
	static char* tx_3 = "Opciones: MATE - TE - SET [XX] - REPETIR\r\n";


	volatile uint8_t rx_ready = 0; ///variable para la funcion callback -> indica el fin de la recepcion de datos
	volatile uint16_t rx_size = 0;

	///Me conviene usar este callback ya que los mensajes que el micro recibe no tienen todos la misma longitud
	void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
	{
	    if (huart->Instance == USART1)
	    {
	        rx_size = Size;   //
	        rx_ready = 1;

	        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buf, sizeof(rx_buf));
	    }
	}
///la tension del ADC no puede superar los 3.3V
	uint32_t adc_read_once(void)
	{
	    HAL_ADC_Start(&hadc1);

	    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	    {
	        uint32_t val = HAL_ADC_GetValue(&hadc1);
	        HAL_ADC_Stop(&hadc1);
	        return val;
	    }

	    HAL_ADC_Stop(&hadc1);
	    return 0;   // error
	}
	uint32_t adc_in;///Valor de entrada ADC, luego convertir a tension
	float tension_a3;
	float r_ntc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
//  if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
//  {
//      Error_Handler();
//  }
  ///int size = strlen(tx_1);
  ///int size2 = strlen(tx_2);
//  HAL_UART_Receive_DMA(&huart1,rx_buf,size);
///MEMORIA, CONFIG
  Config config;

  Flash_Read_Config(&config);

  if(config.firma != CONFIG_FIRMA)
  {
      // Valores por defecto
      config.modo = 0;
      config.temp_usuario = 75;
      config.firma = CONFIG_FIRMA;

      Flash_Write_Config(&config);
  }
  HAL_Delay(200);
  char msg[60];



  HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tx_1, strlen(tx_1));




  ///Esta funcion recibe hasta el "silencio" osea no espera a que se llene el buffer de rx
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buf, sizeof(rx_buf));
  __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT); ///No se para que es esto -> Para prevenir que el callback se dispare dos veces

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  uint8_t alerta_confirmada = 0;
  uint8_t buzzer_activo = 0;
  uint8_t switch_activo = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);

//  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//  DWT->CYCCNT = 0;
//  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  while (1)
  {
	    char r_txt[50];

		///resolucion del ADC 12 bits == 0 - 4095
	    if (rx_ready)
	    {
	        rx_ready = 0;

	        if (rx_size < sizeof(rx_buf))
	            rx_buf[rx_size] = '\0';
	        else
	        {
	            rx_buf[sizeof(rx_buf)-1] = '\0';
	        }
	        ///COMPARO EL MENSAJE RECIBIDO PARA VERIFICAR ELECCION DEL USUARIO
		    if (strncmp(rx_buf, "MATE", 4) == 0)
		    {
		    	sprintf(r_txt, "Modo MATE - 80C\r\n");
		    	HAL_UART_Transmit(&huart1, (uint8_t*)r_txt, strlen(r_txt), 100);
		        config.modo = 1;
		        config.temp_usuario = 80;
		        Flash_Write_Config(&config);
		    }

		    else if (strncmp(rx_buf, "TE", 2) == 0)
		    {
		    	sprintf(r_txt, "Modo TE - 90C\r\n");
		    	HAL_UART_Transmit(&huart1, (uint8_t*)r_txt, strlen(r_txt), 100);
		        config.modo = 2;
		        config.temp_usuario = 90;
		        Flash_Write_Config(&config);
		    }

		    else if (strncmp(rx_buf, "SET ", 4) == 0)
		    {

		        char *endptr;
		        int temp = strtol(rx_buf + 4, &endptr, 10);

		        if (endptr != rx_buf + 4)
		        {
		            config.modo = 3;
		            config.temp_usuario = temp;
		            Flash_Write_Config(&config);
		        }
		        else
		        {
		            sprintf(r_txt, "Error formato. Use: SET XX\r\n");
		        }
		    	sprintf(r_txt, "Modo PERSONALIZADO - %d C\r\n", temp);
		    	HAL_UART_Transmit(&huart1, (uint8_t*)r_txt, strlen(r_txt), 100);
		    }
		    else if (strncmp(rx_buf, "REPETIR", 7) == 0)
		    {
		        // no leo flash, uso lo que ya está en config

		        if (config.modo == 1)
		        {
		            sprintf(r_txt, "Repitiendo Modo MATE - 80C\r\n");
		        }
		        else if (config.modo == 2)
		        {
		            sprintf(r_txt, "Repitiendo Modo TE - 90C\r\n");
		        }
		        else if (config.modo == 3)
		        {
		            sprintf(r_txt, "Repitiendo Modo PERSONALIZADO - %ld C\r\n", config.temp_usuario);
		        }

		        HAL_UART_Transmit(&huart1, (uint8_t*)r_txt, strlen(r_txt), 100);


   			if (strncmp((char*)rx_buf, "OK!", 3) == 0)
   			{
   				alerta_confirmada = 1;
   				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
   			}


	    }
	    if (adc_flag)
	   	    {

	   	        adc_flag = 0;
	   	        adc_in = adc_read_once();
	   		  	float tension_a3 = ((float)adc_in/4095.0f) * 3.3f;

	   		  	///Constantes del termistor NTC
	   		  	float beta = 3950.0f;
	   		  	float T0 = 298.15f;      // 25°C en Kelvin
	   		  	float R0 = 10000.0f;     // 10k nominal
	   		  	///CALCULO LA RESISTENCIA MEDAINTE EL DISVISOR RESISTIVO
	   		  	r_ntc = (tension_a3*9400.0f) / (3.3f - tension_a3);
	   		  	float tempK = 1.0f / ( (1.0f/T0) + (1.0f/beta)*log(r_ntc/R0) );
	   		  	float tempC = tempK - 273.15f;


	   		  	int tempC_int = (int) (tempC);
	   		  	char tx_buf[40];
	   		 	 sprintf(tx_buf, "ALERTA TEMP = %d \r\n", tempC_int);


	   		 	if (tempC_int > config.temp_usuario) ///temp del sensor supera la del usuario -> envia notificacion, activa led y buzzer
	   			{
	   		  			if(!alerta_confirmada)
	   		  			{
	   		  				HAL_UART_Transmit(&huart1, (uint8_t*)tx_buf, strlen(tx_buf), 1000);
	   		  				buzzer_activo =1;
	   		  			}
	   		  			else
	   		  			{
	   		  				buzzer_activo = 0;
	   		  			}
	   			 }

	   		  	else
	   		  	{
	   		  		alerta_confirmada = 0;
	   		  		buzzer_activo = 0;
	   		  	}
	   		  	///depende del flag del buzzer lo enciendo o no
	   		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, buzzer_activo ? GPIO_PIN_SET : GPIO_PIN_RESET);

	   		uint32_t start = DWT->CYCCNT;

	   		// procesamiento ADC

	   		uint32_t end = DWT->CYCCNT;
	   		uint32_t ciclos = end - start;
	   	    }
	    __WFI();

  }


}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1599;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D1_Pin|D0_Pin|LD2_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D6_Pin|D5_BUZ_Pin|D4_LED_rojo_Pin|D10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D9_GPIO_Port, D9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D1_Pin D0_Pin LD2_Pin D7_Pin */
  GPIO_InitStruct.Pin = D1_Pin|D0_Pin|LD2_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D6_Pin D5_BUZ_Pin D4_LED_rojo_Pin D10_Pin */
  GPIO_InitStruct.Pin = D6_Pin|D5_BUZ_Pin|D4_LED_rojo_Pin|D10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : D9_Pin */
  GPIO_InitStruct.Pin = D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(D9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : D14_SWITCH_Pin */
  GPIO_InitStruct.Pin = D14_SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(D14_SWITCH_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
