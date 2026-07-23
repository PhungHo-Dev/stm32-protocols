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
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void writeCMD(uint8_t cmd){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); // CS =0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0); // A0 =0 ->cmd
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 1000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); // CS=1
}

void writeData(uint8_t data){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); // CS =0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1); // A0 =1 -> data
	HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); // CS=1
}

void setPOS(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	writeCMD(0x2A);// Column address set
	writeData(0x00);
	writeData(x1);
	writeData(0x00);
	writeData(x2);

	writeCMD(0x2B);// Row address set
	writeData(0x00);
	writeData(y1);
	writeData(0x00);
	writeData(y2);
}

//RGB 16bit 565
void fullDisplay(uint16_t color){
	writeCMD(0x2C);
	int i;
	for(i=0; i< 128*160 ; i++){
		writeData(color & 0xFF);
		writeData(color >> 8);

	}

}

void initTFT(){
	// hw reset
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
	HAL_Delay(20);
	//sw reset
	writeCMD(0x01);
	HAL_Delay(200);
	// sleep out
	writeCMD(0x11);
	HAL_Delay(200);

	writeCMD(0xB1);
	// 0x01   0x2C  0x2D
	writeData(0x01);
	writeData(0x2C);
	writeData(0x2D);

	writeCMD(0xB2);
	writeData(0x01);
	writeData(0x2C);
	writeData(0x2D);

	writeCMD(0xB3);
	writeData(0x01);
	writeData(0x2C);
	writeData(0x2D);
	writeData(0x01);
	writeData(0x2C);
	writeData(0x2D);

	writeCMD(0xB4);
	writeData(0x07);

	writeCMD(0xC0);
	//0xA2 0x02  0x84
	writeData(0x2D);
	writeData(0xA2);
	writeData(0x02);
	writeData(0x84);

	writeCMD(0xC1);
	writeData(0xC5);

	writeCMD(0xC2); // 0x0A 0x00
	writeData(0x0A);
	writeData(0x00);

	writeCMD(0xC3); // 0x8A 0x2A
	writeData(0x8A);
	writeData(0x2A);

	writeCMD(0xC4); // 0x8A 0xEE
	writeData(0x8A);
	writeData(0xEE);

	writeCMD(0xC5);
	writeData(0x0E);

	writeCMD(0xE0); // 0x02  0x1C  0x07  0x12  0x37  0x32  0x29  0x2D  0x29  0x25 0x2B  0x39  0x00  0x01  0x03  0x10
	writeData(0x02);
	writeData(0x1C);
	writeData(0x07);
	writeData(0x12);
	writeData(0x37);
	writeData(0x32);
	writeData(0x29);
	writeData(0x2D);
	writeData(0x29);
	writeData(0x25);
	writeData(0x2B);
	writeData(0x39);
	writeData(0x00);
	writeData(0x01);
	writeData(0x03);
	writeData(0x10);

	writeCMD(0xE1); // 0x03  0x1D 0x07 0x06 0x2E 0x2C 0x29  0x2D  0x2E  0x2E  0x37  0x3F  0x00  0x00 0x02 0x10
	writeData(0x03);
	writeData(0x1D);
	writeData(0x07);
	writeData(0x06);
	writeData(0x2E);
	writeData(0x2C);
	writeData(0x29);
	writeData(0x2D);
	writeData(0x2E);
	writeData(0x2E);
	writeData(0x37);
	writeData(0x3F);
	writeData(0x00);
	writeData(0x00);
	writeData(0x02);
	writeData(0x10);

	writeCMD(0x36); // memory data access control
	writeData(0x08);

	writeCMD(0x3A); // interface pixel format
	writeData(0x05);

	writeCMD(0x20);

	setPOS(0,0, 128, 160);
	//EN display
	writeCMD(0x29);
	HAL_Delay(100);
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
  initTFT();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  fullDisplay(0xF800);
	  HAL_Delay(100);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
