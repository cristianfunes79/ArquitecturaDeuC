/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "asm_func.h"
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

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

 ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* Ejercicio 1.
 * @brief 	Inicializa un array con 0's
 * @param 	vector array que se va a inicializar
 * @param 	longitud longitud del array a inicializar
 * @retval 	None
 */
void zeros(uint32_t* vector, uint32_t longitud)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vector[i] = 0;
	}
}

/*
 * @brief 	Inicializa un array con 0's
 * @param 	vector array que se va a inicializar
 * @param 	longitud longitud del array a inicializar
 * @retval 	None
 */
void zeros16(uint16_t* vector, uint32_t longitud)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vector[i] = 0;
	}
}

/* Ejercicio 2.
 * @brief	Realiza el producto de un vector por un escalar
 * @param 	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array donde se almacena el resultado
 * @param 	longitud Longitud del array
 * @param 	escalar Valor escalar por el que se va a multiplicar vectorIn
 */
void productoEscalar32(uint32_t* vectorIn, uint32_t* vectorOut, uint32_t longitud, uint32_t escalar)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vectorOut[i] = escalar * vectorIn[i];
	}
}

/* Ejercicio 3.
 * @brief	Realiza el producto de un vector por un escalar
 * @param 	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array donde se almacena el resultado
 * @param 	longitud Longitud del array
 * @param 	escalar Valor escalar por el que se va a multiplicar vectorIn
 */
void productoEscalar16(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vectorOut[i] = escalar * vectorIn[i];
	}
}

/* Ejercicio 4.
 * @brief	Realiza el producto de un vector por un escalar saturando el resultado a 12bits
 * @param 	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array donde se almacena el resultado
 * @param 	longitud Longitud del array
 * @param 	escalar Valor escalar por el que se va a multiplicar vectorIn
 */
void productoEscalar12(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitud, uint16_t escalar)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vectorOut[i] = (escalar * vectorIn[i] <= 4095) ? escalar * vectorIn[i] : 4095;
	}
}

/* Ejercicio 5.
 * @brief	Realice una funcion que implemente un filtro de ventana movil de 10 valores sobre un vector de muestras.
 * @param	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array donde se almacena el resultado
 * @param 	longitudVectorIn Longitud del darray
 */
void filtroVentana10(uint16_t* vectorIn, uint16_t* vectorOut, uint32_t longitudVectorIn)
{
	for (uint32_t i=0; i<longitudVectorIn; ++i)
	{
		// para cada indice es necesario tomar 5 muestras a la izquierda y 5 muestras a la derecha
		// vectorIn = [0][1]...[i-5][i-4][i-3][i-2][i-1][i][i+1][i+2]...[i+5]...[longitudVectorIn-1]
		uint32_t muestrasIzquierda 	= (i>4) ? 5 : i;
		uint32_t muestrasDerecha 	= ((longitudVectorIn-1-i)>5) ? 5 : (longitudVectorIn-1-i);
		uint32_t totalIzquierda = 0;
		uint32_t totalDerecha = 0;

		for (uint32_t j=1; j<=muestrasIzquierda; ++j)
		{
			totalIzquierda += vectorIn[i-j];
		}

		for (uint32_t j=1; j<=muestrasDerecha; ++j)
		{
			totalDerecha += vectorIn[i+j];
		}

		vectorOut[i] = (totalIzquierda+totalDerecha)/(muestrasIzquierda+muestrasDerecha);
	}
}

/* Ejercicio 6.
 * @brief	Realizar una funcion que reciba un vector de numeros signados de 32bits y los empaquete en otro vector de 16bits.
 * 			La funcion debera adecuar los valores de entrada a la nueva precision.
 * @param	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array con el resultado
 * @param 	longitud Longitud del array
 */
void pack32to16(int32_t* vectorIn, int16_t* vectorOut, uint32_t longitud)
{
	for (uint32_t i=0; i<longitud; ++i)
	{
		vectorOut[i] = (int16_t) (vectorIn[i]/0x10000);
	}
}

/* Ejercicio 7.
 * @brief	Realizar una funcion que reciba un vector de numeros signados de 32ibts y devuelva la posicion del maximo del vector
 * @param	vectorIn Array con los datos de entrada
 * @param 	longitud Longitud del array
 */
int32_t max(int32_t* vectorIn, uint32_t longitud)
{
	int32_t maximo = vectorIn[0];
	int32_t posicionMaximo = 0;
	for (uint32_t i=1; i<longitud; ++i)
	{
		if (vectorIn[i] > maximo)
		{
			maximo = vectorIn[i];
			posicionMaximo = i;
		}
	}

	return posicionMaximo;
}

/* Ejercicio 8.
 * @brief	Realizar una funcion que reciba un vector de muestras signadas de 32bits y lo decime descartando una cada N muestras
 * @param	vectorIn Array con los datos de entrada
 * @param 	vectorOut Array resultante
 * @param	longitud Longitud del array
 * @param 	N Muestra a descartar
 */
void downsampleM(int32_t* vectorIn, int32_t* vectorOut, uint32_t longitud, uint32_t N)
{
	uint32_t j=0;

	for (uint32_t i=0; i<longitud; ++i)
	{
		if (i%N != 0)
		{
			vectorOut[j] = vectorIn[i];
			j++;
		}
	}
}

/* Ejercicio 9.
 * @brief	Realizar una funcion que reciba un vector de muestras no signadas de 16bits e invierta su orden
 * @param 	vector Array de entrada
 * @param	longitud Longitud del array de entrada
 */
void invertir(uint16_t* vector, uint32_t longitud)
{
	for (uint32_t i=0; i<longitud/2; ++i)
	{
		uint16_t aux = vector[i];
		vector[i] = vector[longitud-1-i];
		vector[longitud-1-i] = aux;
	}
}

/* Ejercicio 10.
 * @brief	Realizar una funcion que recibe un vector de 4096 valores de 16bits (signados), que corresponden
 * 			a muestras de audio tomadas a una tasa de muestreo de 44100muestas/s. La funcion debe introducir
 * 			un eco de la mitad de la amplitud de la muestra original a los 20ms de comenzada la grabacion.
 * @param	vector Array de entrada
 */
void eco(int16_t* vector)
{
	// Para agregar eco se adiciona la senal dividida por 2 y desplazada 20ms.
	// Como la tasa de muestreo es de 44100 muestras/s, quiere decir que a los 20ms estamos
	// en la muestra 44100 * 20 / 1000 = 882000/1000 = 882
	for (uint32_t i=882; i<4096; ++i)
	{
		vector[i] = vector[i] + vector[i-882] / 2;
	}
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo
    // asm_func.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  PrivilegiosSVC ();

  const uint32_t Resultado = asm_sum (5, 3);

  uint32_t data_in[10]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
  uint32_t data_out[10]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

  zeros(data_in, 10);
  zeros(data_out, 10);

  for (uint32_t i=0; i<10; ++i) data_in[i] = i;

  productoEscalar32(data_in, data_out, 10, 2);

  // Prueba producto escalar 16 -------------------
  uint16_t data_in16[10]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
  uint16_t data_out16[10]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};
  zeros16(data_in16, 10);
  zeros16(data_out16, 10);

  for (uint32_t i=0; i<10; ++i) data_in16[i] = i;

  productoEscalar16(data_in16, data_out16, 10, 2);

  // Prueba producto escalar 12 -------------------
  zeros16(data_in16, 10);
  zeros16(data_out16, 10);

  for (uint32_t i=0; i<10; ++i) data_in16[i] = i;

  productoEscalar12(data_in16, data_out16, 10, 2);


  /* USER CODE END 2 */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
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
