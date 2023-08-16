/*
 * main.c
 *
 * This is the main program.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "main.h"
#include "stdio.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "Person_Structure.h"
#include "MEF_States.h"
#include "MEF_Handler.h"
#include "stm32f1_rc522.h"
#include "API_I2C.h"
#include "API_SPI.h"
#include "API_LCD.h"

#define ID_LENGTH 16
#define MAX_PEOPLE 3

/* Handlers */
TIM_HandleTypeDef htim11;
UART_HandleTypeDef huart2;

/* Time counter */
volatile uint32_t Timer_Count = 0;

/* System configuration functions def. */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM11_Init(void);

/* Interrupt Service Routine Function */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/* Structures Database Initialization Function */
void Database_Init(Person *People);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* Structure array including registered people ID*/
	Person People[MAX_PEOPLE];

	/* Load structure array*/
	Database_Init(People);

	/* Initialize HAL */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();		// Init. GPIOs
	MX_TIM11_Init();	// Init. TIM11
	I2C_Init(); 		// Init. I2C Comm.
	SPI_Init(); 		// Init. I2C Comm.
	MFRC522_Init();		// Init. MFR522 RFID Reader
	LCD_Init ();		// Init. LCD
	HAL_TIM_Base_Start_IT(&htim11);

	while (1)
	{
		/* Update Finite State Machine */
		MEF_Update(People);
	}
}

/**
  * @brief  Interrupt Service Routine
  *
  * This routine increases Timer_Count each time the TIM11 counts one second.
  *
  * @retval void
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM11)
  {
    Timer_Count += 1;
  }
}

/**
  * @brief Function used for database loading
  *
  * The database of the system is declared and initialized. The structures containing
  * data of each ID is initialized.
  *
  *	@param Person *People Pointer to structure array
  * @retval void
  */
void Database_Init(Person *People)
{
	uint8_t ID_0[] = "880468ED09";
	for (int i = 0; i < ID_LENGTH; i++)
	{
	  People[0].ID[i] = ID_0[i]; // Conversión explícita
	}
	People[0].ID[ID_LENGTH] = '\0';
	People[0].Registered = 0;
	People[0].Time_Counter = 0;

	uint8_t ID_1[] = "88044171BC";
	for (int i = 0; i < ID_LENGTH; i++)
	{
	  People[1].ID[i] = ID_1[i]; // Conversión explícita
	}
	People[1].ID[ID_LENGTH] = '\0';
	People[1].Registered = 0;
	People[1].Time_Counter = 0;

	uint8_t ID_2[] = "ABA6A92185";
	for (int i = 0; i < 16; i++)
	{
	  People[2].ID[i] = ID_2[i];
	}
	People[2].ID[ID_LENGTH] = '\0';
	People[2].Registered = 0;
	People[2].Time_Counter = 0;

	return;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM11 Initialization Function
  *
  * @param void
  * @retval void
  */
static void MX_TIM11_Init(void)
{
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 7999;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 9999;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  *
  * @param void
  * @retval void
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  *
  * @retval None
  */
void Error_Handler(void)
{
	while(1)
	{
	}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
