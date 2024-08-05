/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  @ATTENTION !
  * 	##Explains CubeMX Variables & IO
  *
  * 	--> ADC_HandleTypeDef hadc1;	 ==> It relates to the 	"SubSys_Sensor_Battery_Driver"
  *
  * 	--> I2C_HandleTypeDef hi2c1;   	 ==> It relates to the 	"SubSys_Sensor_TPGVH_Driver"
  *
  * 	--> SD_HandleTypeDef  hsd;	   	 ==> It relates to the 	"SubSys_SDcard_Driver"
  *		--> SDIO_CK frequency 			 ==> 48MHz fixed
  *
  * 	--> TIM_HandleTypeDef htim3;  	 ==> It relates to the 	"SubSys_AlertControl_Driver"
  *
  *		--> UART_HandleTypeDef huart1; 	 ==> It relates to the 	"SubSys_WirelessCommunication_Setting_Driver" & "SubSys_WirelessCommunication_Telemetry_Driver"
  *		--> UART_HandleTypeDef huart2; 	 ==> It relates to the 	"SubSys_Sensor_GPS_Driver"
  *
  *		--> GPIO_TypeDef |PORT->A|	|GPIO_PIN_11| ==> GPS reset pin (will be reset status for enable sensor)
  *						 |PORT->B|	|GPIO_PIN_13| ==> LORA M0 configuration pin
  *						 |PORT->B|	|GPIO_PIN_14| ==> LORA M1 configuration pin
  *
  *
  *		##Wireless Communication configuration parameters
  *
  *		Satellite Carrier Unit's adress will be 0x1923  and channel is 0x10
  *		Satellite Payload Unit's adress will be 0x1453  and channel is 0x05
  *		Satellite Ground Station adress will be 0x2023  and channel is 0x10
  *
  *
  *		##Clean Code explanation
  *		"The __CLOSED expression can be written at the end of the header guard.
  *		 If you remove this part, the related code will work and complied
  *
  ******************************************************************************
  */


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
	#include "SubSys_Sensor_Battery_Driver.h"
	#include "SubSys_Sensor_TPGVH_Driver.h"
	#include "SubSys_AlertControl_Driver.h"
	#include "SubSys_SDcard_Driver.h"
	#include "SubSys_Sensor_GPS_Driver.h"
	#include "SubSys_WirelessCommunication_Setting_Driver.h"
	#include "SubSys_WirelessCommunication_Telemetry_Driver.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*
	 ===============================================================================
	                      ##### DEFINITIONS #####
	 ===============================================================================
*/

	/**!
	 * Pressure Temperature Sensor (MS5611) Definitions
	 */
	#define MS5611_I2C_ADDRESS_H 0xEE		/*! CSB pin is HIGH */
	#define MS5611_I2C_ADDRESS_L 0xEC 		/*! CSB pin is LOW  */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_tx;
DMA_HandleTypeDef hdma_sdio_rx;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/*
	 ===============================================================================
	                      ##### MULTIPLE VARIABLE #####
	 ===============================================================================
*/
	/**!
	 * SubSys_Sensor_Battery_Driver variables
	 */
	uint8_t NumSerialBat=0;			/*! Number of serial connection battery */
	float BatteryVoltage=0.0;			/*! Voltage of the Satellite's battery  */

	/**!
	 * SubSys_Sensor_TPGVH_Driver variables
	 */
	MS5611_HandleTypeDef MS5611;	/*! MS5611 object					*/
	float  MS5611_Press=0.0;		/*! Pressure data variable 			*/
	float  MS5611_Temp=0.0;			/*! Temperature data variable 		*/
	float  MS5611_Altitude;			/*! Vertical Altitude data variable */
	float  MS5611_VertSpeed;		/*! Vertical Speed data variable    */
	float  MS5611_VertAcc;			/*! Vertical Acceleration variable  */
	float  MS5611_gForce;			/*! Vertical g force data variable  */
	float  SatCar_Mass;				/*! Total mass of Satellites Carrier module */

	/**!
	 * SubSys_Sensor_GPS_Driver variables
	 */
	float GPS_Altitude;				/*! Vertical distance info of satellite beetween */
	float GPS_Longitude;			/*! Location info of satellite on the earth 	 */
	float GPS_Latitude;				/*! Location info of satellite on the earth 	 */

	/**! We create 2 object for subsystem wireless communication,
	 * 	one of them is about configuration setting of wirelesscom device
	 * 	other one is about application object
	 */
	SubSys_WirelesscomConfig_HandleTypeDef dev_WirelessComConfig;
	SubSys_WirelessCom_APP_HandleTypeDef dev_WirelessComApp;

/*
	 ===============================================================================
						  ##### SINGLE VARIABLE #####
	 ===============================================================================
*/
	/**
	 * All system units will be work together at 1Hz
	 */
	uint32_t SystemTick;

	/**
	 * USB-TTL variables, collected datas are sent to Station PC
	 * (The code block has been commented out within the while loop)
	 */
	uint8_t TelemetryData[100]={0};
	uint8_t WrittenBytes;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


/******>>> SENSOR BATTERY INIT BEGIN >>>******/
	#ifdef SAT_CARRIER_SUBSYS_DRIVERS_SENSOR_BATTERY_H
	NumSerialBat = 1;	/*! Number of serial connection battery */
	MeasBattery_Init(NumSerialBat);
	#endif
/******<<< SENSOR BATTERY INIT END <<<******/


/******>>> SENSOR TPGVH INITIALIZATION BEGIN >>>******/
	#ifdef SAT_CARRIER_SUBSYS_DRIVERS_SENSOR_TPGVH_H
	MS5611.I2C_ADDRESS = MS5611_I2C_ADDRESS_H;
	MS5611.i2c = &hi2c1;
	MS5611.Ref_Alt_Sel = 'm';
	MS5611_Init(&MS5611);
	#endif
/******<<< SENSOR TPGVH INITIALIZATION END <<<******/


/******>>> ALERT CONTROL INITIALIZATION BEGIN >>>******/
	#ifdef SAT_CARRIER_SUBSYS_DRIVER_ALERTCONTROL_H
	/**!
	 * @Attention!
	 *
	 * @Calculation ==> ![Duty cycle must be %50 according to  the datasheet]
	 * 					 We sets ARR value as 100 at the beginnig
	 * 					 Frequency  = (APB Tim Clock) / ((PSC)*(ARR))
	 *					 Duty Cycle = ((CCRx/ARR)*100)
	 *
	 * APB1 Timer Clock = 50MHz
	 * PSC = 183-1
	 * ARR = 100-1
	 * PSC and ARR's parameters for 2.73KHz passive buzzer module
	 */
	PassiveBuzz_Init(&htim3, TIM_CHANNEL_1);
	#endif
/******<<< ALERT CONTROL INITIALIZATION END <<<******/


/******>>> SD CARD INITIALIZATION BEGIN >>>******/
	#ifdef SAT_CARRIER_SUBSYS_DRIVERS_SDCARD_H
	/*! We create a buffer that contains the satellite's carrier variables, and we fill it with variables from SD_Data objects */
	extern char SdDatasBuf[LineSize];

	/*!(@warning)	Don't write "E:" , "e:",  "e\" */
	SD_Mount("E/", 0);

	/*
	 * @instructions :
	 * 				First of all, We create a directiory as named "SAT_CAR",
	 * 				Secondly give a name for system txt document as named "STM32.TXT"
	 * 				Finally put our variable buffer
	 *
	 * @Attention!   : If you use lowercase letters, this function will reverse the name to uppercase letters as given below
	 * 					(e.g)CAR_Raw ==> CAR_RAW
	 */
	SD_Create_Dir_File("SAT_CAR", "SAT_CAR/STM32.TXT", SdDatasBuf);
	#endif
/******<<< SD CARD INITIALIZATION END <<<******/


/******>>> SENSOR GPS INITIALIZATION BEGIN >>>******/
	#ifdef SAT_CARRIER_SUBSYS_DRIVERS_SENSOR_GPS_H
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	GPS_Init();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	#endif
/******<<< SENSOR GPS INITIALIZATION END <<<******/



/******>>> WIRELESS COMMUNICATION SETTING & TELEMETRY INITIALIZATION BEGIN >>>******/
	/**
	 * @brief : First of all, we upload initial settings into the wireless communication device.
	 * 		  After that, we determine Target Address high and low byte and Target Channel.
	 * 		  Some LoRa module has two pin as named M0 and M1. These provides selecting working mode
	 * 		  For E220400T30 are M0 and M1 pins.
	 * @note  : If you use dma for receiving and transmiting, fill it parameters that
	 *		  come after channel info
	 */
	 #ifdef SAT_CARRIER_SUBSYS_DRIVERS_WIRELESSCOMMUNICATION_SETTING_H__CLOSED

	 dev_WirelessComConfig.interface.huart = &huart1;
	 dev_WirelessComConfig.interface.GPIOx = GPIOB;
	 dev_WirelessComConfig.LORA_PIN_M0= GPIO_PIN_14;
	 dev_WirelessComConfig.LORA_PIN_M1= GPIO_PIN_13;
	 dev_WirelessComConfig.Mode_SW = DeepSleep; 		/*! Module goes to sleep, that provides you to configure settings */

	 SubSys_WirelessCom_Config_Init(&dev_WirelessComConfig);

	 #endif

	 #ifdef SAT_CARRIER_SUBSYS_DRIVERS_WIRELESSCOMMUNICATION_TELEMETRY_H

	 /*! Will be filled for your dev that use now*/
	 dev_WirelessComApp.huartX = &huart1;
	 dev_WirelessComConfig.Mode_SW = NormalMode; 		/*! UART and wireless channel are open, transparent transmission is on*/
	 SubSys_WirelessCom_Config_WORK_MODE(&dev_WirelessComConfig);

	 /*! Will be filled for the PAYLOAD(Target) Device */
	 dev_WirelessComApp.Target_ADDH = 0x20;
	 dev_WirelessComApp.Target_ADDL = 0x23;
	 dev_WirelessComApp.Target_Ch   = 0x10;

	 #endif
/******<<< WIRELESS COMMUNICATION SETTING & TELEMETRY INITIALIZATION END <<<******/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	   /*! At the beginning of each loop, the system retrieves the last recorded milliseconds */
	   SystemTick = HAL_GetTick();

	   /*! It reads the battery voltage and stores it */
	   ReadBatteryVoltage(&hadc1);

	  /*! It reads the TPGVH data and saves it into the variables created in the system
	   * (T) = Temperature
	   * (P) = Pressure
	   * (G) = G force
	   * (V) = Vertical Speed
	   * (H) = Vertical Height
	   **/
	   MS5611_Read_ActVal(&MS5611);

	   /*! The collected data is stored into variables that created for the SD card */
	   SD_FillVariables();

	   /*! The recorded variables are written to the SD card */
	   SD_Write(SdDatasBuf,"SAT_CAR/STM32.TXT");

	   /*! This block is used to send the collected data to the Station PC using USB-TTL */
//
//				  WrittenBytes = sprintf(TelemetryData,
//													  "<%.1f> <%.1f> <%.1f> <%.1f> <%.1f> \n",
//																							  BatteryVoltage , MS5611_Press,MS5611_Temp,
//																							  MS5611_Altitude, MS5611_VertSpeed,
//																							  GPS_Latitude   , GPS_Longitude,
//																							  GPS_Altitude);
//				  HAL_UART_Transmit(&huart1, TelemetryData, WrittenBytes, 1000);


	   /*! Transfer all necessary datas from Carrier to Payload of Satellite*/
	   SubSys_WirelessCom_Telemetry_Transfer_From_To(Sat_Carrier, Sat_Payload, &dev_WirelessComApp);


	   /*! The system time is retrieved again and the loop waits until the elapsed time reaches 1000 milliseconds*/
	   HAL_Delay(abs(1000 - (HAL_GetTick()-SystemTick)));

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 183-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  huart1.Init.BaudRate = 115200;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
