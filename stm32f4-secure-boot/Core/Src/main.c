/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "crc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_START_ADDR    0x08008000U
#define APP_IMAGE_SIZE    491516U           // In bytes, not including CRC!
#define CRC_ADDR          (APP_START_ADDR + APP_IMAGE_SIZE)  // 0x0807FFFC

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern CRC_HandleTypeDef hcrc;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint32_t calculate_app_crc(void);
int check_app_valid(void);
void jump_to_app(void);
uint32_t sw_crc32(const uint8_t *data, uint32_t length);

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
  MX_USART2_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  //Configure LED GPIO
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  char msg[] = "Bootloader active\r\n";
  char valid_app_msg[] = "Valid application\r\n";
  char validation_error_msg[] = "Could not validate the application\r\n";

  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  //TEST SW CRC32 calculation:
  uint32_t sw_crc = sw_crc32((uint8_t*)APP_START_ADDR, APP_IMAGE_SIZE);
  char sw_crc_msg[40];
  snprintf(sw_crc_msg, sizeof(sw_crc_msg), "SW CRC: 0x%08lX\n", (unsigned long)sw_crc);
  HAL_UART_Transmit(&huart2, (uint8_t*)sw_crc_msg, strlen(sw_crc_msg), HAL_MAX_DELAY);


  if (check_app_valid()) {
	  HAL_UART_Transmit(&huart2, (uint8_t*)valid_app_msg, strlen(valid_app_msg), HAL_MAX_DELAY);
	  HAL_Delay(100);
	  jump_to_app();
  } else {
	  while (1){
		  HAL_UART_Transmit(&huart2, (uint8_t*)validation_error_msg, strlen(validation_error_msg), HAL_MAX_DELAY);
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  HAL_Delay(500);
	  }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
//    /* USER CODE END WHILE */
//
//    /* USER CODE BEGIN 3 */
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//	HAL_Delay(500);
//  }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  CRC calculation function.
  * @retval uint32_t
  */
uint32_t calculate_app_crc(void)
{
	//STM32 HAL's CRC requires input in 32-bit words.
	uint32_t word_count = APP_IMAGE_SIZE / 4;

	//TEST
	char msg1[40];
	for (uint32_t i = 0; i < 4; ++i) {
	    snprintf(msg1, sizeof(msg1), "Word %lu: 0x%08lX\r\n", i, *((uint32_t*)APP_START_ADDR + i));
	    HAL_UART_Transmit(&huart2, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);
	}

	char msg2[40];
	for (uint32_t i = word_count - 4; i < word_count; ++i) {
		snprintf(msg2, sizeof(msg2), "Last word %lu: 0x%08lX\n", i, *((uint32_t*)APP_START_ADDR + i));
		HAL_UART_Transmit(&huart2, (uint8_t*)msg2, strlen(msg2), HAL_MAX_DELAY);
	}

	char msg3[40];
	snprintf(msg3, sizeof(msg3), "CRC control register: 0x%08lX\n", (unsigned long)hcrc.Instance->CR);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg3, strlen(msg3), HAL_MAX_DELAY);

	//You can remove the above printfs

	//return HAL_CRC_Calculate(&hcrc, (uint32_t*)APP_START_ADDR, word_count);
	uint32_t sw_crc = sw_crc32((uint8_t*)APP_START_ADDR, APP_IMAGE_SIZE);
	return sw_crc;
}

/**
  * @brief  application validation function.
  * @retval int
  */
int check_app_valid(void)
{
	uint32_t crc_calculated = calculate_app_crc();
	uint32_t crc_stored = *(uint32_t*)CRC_ADDR;
	return (crc_calculated == crc_stored);
}

/**
  * @brief  jump to application function.
  * This function is executed if CRC match, the bootloader will jump here.
  * @retval uint32_t
  */
void jump_to_app(void)
{
	//Read vector table (SP) and Reset handler of the app
	uint32_t app_sp = *(uint32_t*)APP_START_ADDR;
	uint32_t app_entry = *(uint32_t*)(APP_START_ADDR + 4); //gives the reset handler address

	//set SP to app
	__set_MSP(app_sp);

	void (*app_reset_handler)(void) = (void (*)(void))app_entry;

	//jump
	app_reset_handler();
}

/**
  * @brief  SW CRC32 calculation
  * @retval uint32_t
  */
uint32_t sw_crc32(const uint8_t *data, uint32_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
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
