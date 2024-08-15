/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "API_ds3231.h"
#include "API_ssd1306.h"
//#include "API_numpad_4x4.h"
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "fonts.h"
#include "stdint.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char time[9];
char firstTime[9];
char date[9];

uint8_t key;
uint32_t counter=0;


uint8_t tx_buffer[30];

volatile uint32_t start_time1 = 0;
volatile uint32_t end_time1 = 0;
uint8_t pin_state= 0;

uint32_t firstTimeInSeconds;
uint32_t currentTimeInSeconds;
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

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
// function to set time


float TEMP;
char buffer[15];
int pushbutton;

volatile uint8_t timer_flag = 0;

RTC_HandleTypeDef hrtc;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

RTC_TimeTypeDef sTime;

RTC_DateTypeDef dateToUpdate;

RTC_TimeTypeDef changeTime;
RTC_DateTypeDef changeDate;

void mpu9250_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
void mpu9250_write_reg(uint8_t reg, uint8_t data);

char readKeyMPrincipal(void);
void presentarHora(void);
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  SSD1306_Init();
  //int16_t accel_data;
  //uint8_t imu_data[14];
  //mpu9250_write_reg(28, 0x08);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("CARGANDO..", &Font_11x18, 1);
  SSD1306_UpdateScreen();
  HAL_Delay(3000);
  SSD1306_Clear();

  presentarHora();
  while (1)
  {
	  presentarHora();
	  key = readKeyMPrincipal();
	  if(key == 'A' ){
		  SSD1306_Clear();
		  menuOpciones();
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  hi2c1.Init.ClockSpeed = 400000;
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

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */


  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 13;
  sTime.Minutes = 50;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
  sDate.Month = RTC_MONTH_FEBRUARY;
  sDate.Date = 14;
  sDate.Year = 23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  htim2.Init.Prescaler = 999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, D7_Pin|D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, D6_Pin|D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS_Pin */
  GPIO_InitStruct.Pin = SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : D7_Pin D4_Pin */
  GPIO_InitStruct.Pin = D7_Pin|D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : D2_Pin */
  GPIO_InitStruct.Pin = D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(D2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D6_Pin D5_Pin */
  GPIO_InitStruct.Pin = D6_Pin|D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : D3_Pin */
  GPIO_InitStruct.Pin = D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(D3_GPIO_Port, &GPIO_InitStruct);

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

  /*Configure GPIO pins : D0_Pin D1_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void mpu9250_write_reg(uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg, 1, 100);
	HAL_SPI_Transmit(&hspi1, &data, 1, 100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}
void mpu9250_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
	uint8_t temp_data = 0x80|reg;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &temp_data , 1, 100);
	HAL_SPI_Receive(&hspi1, data, len, 100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim2)
	{
		timer_flag = 1;
	}
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}

void presentarHora(void){
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("Hora, Fecha", &Font_11x18, 1);
	HAL_RTC_GetTime(&hrtc, &sTime,  RTC_FORMAT_BIN);
    sprintf(firstTime, " %02d-%02d-%02d  ", sTime.Hours, sTime.Minutes, sTime.Seconds);
	SSD1306_GotoXY (0,25);
	SSD1306_Puts (firstTime, &Font_11x18, 1);

	HAL_RTC_GetDate(&hrtc, &dateToUpdate, RTC_FORMAT_BIN);
	sprintf(date, " %02d-%02d-%02d  ", dateToUpdate.Date, dateToUpdate.Month, dateToUpdate.Year);
	SSD1306_GotoXY (0,45);
	SSD1306_Puts (date, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}


char readKeyMPrincipal(void){

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'A';
	}
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);
	return ' ';
}

char keyMenuOpciones(){

		HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

		if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

			while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
			return 'A';
		}

		HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

		if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

			while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
			return 'B';
		}


		HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

		if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

			while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
			return 'D';
		}

		HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);
}

char detectNumPad(void){

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin) ){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '1';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '2';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '3';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'A';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '4';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '5';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '6';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'B';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '7';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '8';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '9';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'C';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '*';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '0';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '#';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'D';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

}

void presentarCalculadora(void){
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("Hora, Fecha", &Font_11x18, 1);
	HAL_RTC_GetTime(&hrtc, &sTime,  RTC_FORMAT_BIN);
    sprintf(firstTime, " %02d-%02d-%02d  ", sTime.Hours, sTime.Minutes, sTime.Seconds);
	SSD1306_GotoXY (0,25);
	SSD1306_Puts (firstTime, &Font_11x18, 1);

	HAL_RTC_GetDate(&hrtc, &dateToUpdate, RTC_FORMAT_BIN);
	sprintf(date, " %02d-%02d-%02d  ", dateToUpdate.Date, dateToUpdate.Month, dateToUpdate.Year);
	SSD1306_GotoXY (0,45);
	SSD1306_Puts (date, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}


void calculadora(){
	int salida = 1;
	int opcion = 1;
	char contenido[11] = "0";
	char resultado[11] = "0";
	char value[11];
	char salidaUart[40] = "";
	char key;
	int inicial = 0;
	int tamano = 0;
	int valor1 = 0;
	int valor2 = 0;
	int operacionAritmetica = 0;
	//SSD1306_GotoXY (0,25);
	//SSD1306_GotoXY (0,45);
	SSD1306_Clear();
	while(salida){
		SSD1306_GotoXY (0,0);
		SSD1306_Puts ("Calculadora", &Font_11x18, 1);
		SSD1306_GotoXY (0,45);
		sprintf(value, "%11s", contenido);
		SSD1306_Puts ( value, &Font_11x18, 1);
		SSD1306_UpdateScreen();

		key = detectNumPad();
		switch(key){
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(inicial == 0){
					sprintf(contenido,"%c",key);
					inicial = 1;
				}
				else{
					tamano = strlen(contenido);
					if(tamano > 10)  strncpy(contenido, contenido, 10);
					sprintf(contenido, "%s%c", contenido, key);
				}
				break;
			case '0':
				if(inicial == 0){
					sprintf(contenido,"%c",'0');
					inicial = 0;
				}
				else{
					tamano = strlen(contenido);
					if(tamano > 10)  strncpy(contenido, contenido, 10);
					sprintf(contenido, "%s%c", contenido, key);
				}
				break;
			case 'A': operacionAritmetica = 1;
					if(inicial == 0){
						sprintf(resultado, "%s", "0");
						inicial = 0;
					}
					else{
						sprintf(resultado, "%s", contenido);
						sprintf(contenido, "%s", "0");
						inicial = 0;
					}
					/*
					sprintf(salidaUart,"%s: %s, %d\n\r", "+salidar", resultado);
					strcpy((char*)tx_buffer, salidaUart);
					HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);

					sprintf(salidaUart,"%s: %s, %d\n\r", "+salidac", contenido);
					strcpy((char*)tx_buffer, salidaUart);
					HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
					*/
					break;
			case 'B': operacionAritmetica = 2;
						if(inicial == 0){
							sprintf(resultado, "%s", "0");
							inicial = 0;
						}
						else{
							sprintf(resultado, "%s", contenido);
							sprintf(contenido, "%s", "0");
							inicial = 0;
						}
						/*
						sprintf(salidaUart,"%s: %s\n\r", "+salidar", resultado);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);

						sprintf(salidaUart,"%s: %s\n\r", "+salidac", contenido);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
						*/
					    break;
			case 'C': operacionAritmetica = 3;
						if(inicial == 0){
							sprintf(resultado, "%s", "0");
							inicial = 0;
						}
						else{
							sprintf(resultado, "%s", contenido);
							sprintf(contenido, "%s", "0");
							inicial = 0;
						}
						/*
						sprintf(salidaUart,"%s: %s\n\r", "+salidar", resultado);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);

						sprintf(salidaUart,"%s: %s\n\r", "+salidac", contenido);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
						*/
						break;
			case 'D': operacionAritmetica = 4;
						if(inicial == 0){
							inicial = 0;
							sprintf(resultado, "%s", "0");
						}
						else{
							sprintf(resultado, "%s", contenido);
							sprintf(contenido, "%s", "0");
							inicial = 0;
						}
						/*
						sprintf(salidaUart,"%s: %s\n\r", "+salidar", resultado);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);

						sprintf(salidaUart,"%s: %s\n\r", "+salidac", contenido);
						strcpy((char*)tx_buffer, salidaUart);
						HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
						*/
						break;
			case '#':
					switch(operacionAritmetica){
						case 1:
							valor1 = atoi(resultado);
							valor2 = atoi(contenido);
							valor2 += valor1;
							if(valor2 == 0) 	inicial = 0;
							sprintf(contenido, "%10d", valor2);
							break;
						case 2:
							valor1 = atoi(resultado);
							valor2 = atoi(contenido);
							valor1 -= valor2;
							if(valor1 == 0) 	inicial = 0;
							sprintf(contenido, "%10d", valor1);
							break;
						case 3:
							valor1 = atoi(resultado);
							valor2 = atoi(contenido);
							valor2 *= valor1;
							if(valor2 == 0) 	inicial = 0;
							sprintf(contenido, "%10d", valor2);
							break;
						case 4:
							valor1 = atoi(resultado);
							valor2 = atoi(contenido);
							valor1 /= valor2;
							if(valor1 == 0) 	inicial = 0;
							sprintf(contenido, "%10d", valor1);
							break;
					}
					//sprintf(salidaUart,"%s: %s\n\r", "+salidafr", resultado);
					//strcpy((char*)tx_buffer, salidaUart);
					//HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
					//sprintf(salidaUart,"%s: %s\n\r", "+salidafc", contenido);
					//strcpy((char*)tx_buffer, salidaUart);
					//HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
					break;
			case '*': salida = 0;
					SSD1306_Clear();
					break;
			default: break; // nunca llegar aqui
		}
	}
}

void reloj(){
	int salida = 1;
	int opcion = 0;
	int valor;
	char contenido[11] = "0";
	char resultado = "";
	char value[11];
	char key;
	int inicial = 0;
	int tamano = 0;
	int operacionAritmetica = 0;

	char horas[3] = "";
	char fecha[6][3];
	//RTC_TimeTypeDef changeTime;
	//RTC_DateTypeDef changeDate;
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("Hora, Fecha", &Font_11x18, 1);

	HAL_RTC_GetTime(&hrtc, &changeTime,  RTC_FORMAT_BIN);
	sprintf(fecha[0], "%02d", changeTime.Hours);
	sprintf(fecha[1], "%02d", changeTime.Minutes);
	sprintf(fecha[2], "%02d", changeTime.Seconds);
    sprintf(firstTime, " %s-%s-%0s  ", fecha[0], fecha[1], fecha[2]);
	SSD1306_GotoXY (0,25);
	SSD1306_Puts (firstTime, &Font_11x18, 1);

	HAL_RTC_GetDate(&hrtc, &changeDate, RTC_FORMAT_BIN);
	sprintf(fecha[3], "%02d", changeDate.Date);
	sprintf(fecha[4], "%02d", changeDate.Month);
	sprintf(fecha[5], "%02d", changeDate.Year);
	sprintf(date, " %s-%s-%0s  ", fecha[3], fecha[4], fecha[5]);
	SSD1306_GotoXY (0,45);
	SSD1306_Puts (date, &Font_11x18, 1);

	SSD1306_UpdateScreen();
	while(salida){

		key = detectNumPad();
		switch(key){
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				sprintf(fecha[opcion], "%c%c", fecha[opcion][1], key);
				break;
			case 'D':
				valor = atoi(fecha[opcion]);
				if(opcion == 0){
					if(valor > 0 && valor <= 24)	opcion++;
				}else if(opcion == 1){
					if(valor > 0 && valor <= 60)	opcion++;
				}else if(opcion == 2){
					if(valor > 0 && valor <= 60)	opcion++;
				}else if(opcion == 3){
					if(valor > 0 && valor <= 31)	opcion++;
				}else if(opcion == 4){
					if(valor > 0 && valor <= 12)	opcion++;
				}else if(opcion == 5){
					opcion++;
				}

				if(opcion == 6){
					changeTime.Hours = atoi(fecha[0]);
					changeTime.Minutes = atoi(fecha[1]);
					changeTime.Seconds = atoi(fecha[2]);
					HAL_RTC_SetTime(&hrtc, &changeTime, RTC_FORMAT_BIN);

					changeDate.Date = atoi(fecha[3]);
					changeDate.Month = atoi(fecha[4]);
					changeDate.Year = atoi(fecha[5]);
					HAL_RTC_SetDate(&hrtc, &changeDate, RTC_FORMAT_BIN);

					salida = 0;
					SSD1306_Clear();
				}

				break;
			case '*':
				salida = 0;
				SSD1306_Clear();
				break;
			default: break; // nunca llegar aqui
		}
		sprintf(firstTime, " %s-%s-%0s  ", fecha[0], fecha[1], fecha[2]);
		SSD1306_GotoXY (0,25);
		SSD1306_Puts (firstTime, &Font_11x18, 1);
		sprintf(date, " %s-%s-%0s  ", fecha[3], fecha[4], fecha[5]);
		SSD1306_GotoXY (0,45);
		SSD1306_Puts (date, &Font_11x18, 1);
		SSD1306_UpdateScreen();
	}
}




void menuOpciones(){
	int salida = 1;
	int opcion = 1;
	char contenido[30];
	char key;

	while(salida){
		SSD1306_GotoXY (0,0);
		SSD1306_Puts ("Menu", &Font_11x18, 1);
		SSD1306_GotoXY (10,25);
		if(opcion == 1) SSD1306_Puts ("->Hora,Fecha", &Font_11x18, 1);
		if(opcion == 2) SSD1306_Puts ("->Calculadora", &Font_11x18, 1);
		if(opcion == 3) SSD1306_Puts ("->Salir   ", &Font_11x18, 1);

		SSD1306_UpdateScreen();
		//if(opcion == 3) SSD1306_Puts ("->Salir", &Font_11x18, 1);
		//if(opcion = 3) SSD1306_Puts ("->Salir", &Font_11x18, 1);
		key = keyMenuOpciones();
		switch(key){
			case 'A': if(opcion > 0) opcion--;
				//		sprintf(contenido,"%s: %d, %d", "valor", key, opcion);
				//		strcpy((char*)tx_buffer, contenido);
				//		HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);

						break;
			case 'B': if(opcion < 3) opcion++;
				//		sprintf(contenido,"%s: %d, %d", "valor", key, opcion);
				//		strcpy((char*)tx_buffer, contenido);
				//		HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
						break;
			case 'D':
						if(opcion == 1){
							reloj();
							SSD1306_Clear();
						}
						if(opcion == 2){
							calculadora();
							SSD1306_Clear();
						}
						if(opcion == 3){ salida = 0; }
				//		sprintf(contenido,"%s: %d, %d", "valor", key, opcion);
				//		strcpy((char*)tx_buffer, contenido);
				//		HAL_UART_Transmit(&huart3, tx_buffer, 30, 10);
					    break;
			default: break; // nunca llegar aqui
		}
	}
}





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
