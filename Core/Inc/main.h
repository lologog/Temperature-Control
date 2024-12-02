/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC3_BUTTON_Pin GPIO_PIN_14
#define ENC3_BUTTON_GPIO_Port GPIOC
#define ENC1_BUTTON_Pin GPIO_PIN_15
#define ENC1_BUTTON_GPIO_Port GPIOC
#define ENC2_BUTTON_Pin GPIO_PIN_0
#define ENC2_BUTTON_GPIO_Port GPIOD
#define RESET_Pin GPIO_PIN_4
#define RESET_GPIO_Port GPIOA
#define RESET_EXTI_IRQn EXTI4_IRQn
#define START_STOP_Pin GPIO_PIN_5
#define START_STOP_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_0
#define COL1_GPIO_Port GPIOB
#define COL2_Pin GPIO_PIN_1
#define COL2_GPIO_Port GPIOB
#define COL3_Pin GPIO_PIN_2
#define COL3_GPIO_Port GPIOB
#define ROW4_Pin GPIO_PIN_12
#define ROW4_GPIO_Port GPIOB
#define ROW3_Pin GPIO_PIN_13
#define ROW3_GPIO_Port GPIOB
#define ROW2_Pin GPIO_PIN_14
#define ROW2_GPIO_Port GPIOB
#define ROW1_Pin GPIO_PIN_15
#define ROW1_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_10
#define BUZZER_GPIO_Port GPIOA
#define HEATING_LED_Pin GPIO_PIN_11
#define HEATING_LED_GPIO_Port GPIOA
#define REGULATION_LED_Pin GPIO_PIN_12
#define REGULATION_LED_GPIO_Port GPIOA
#define SETTINGS_LED_Pin GPIO_PIN_15
#define SETTINGS_LED_GPIO_Port GPIOA
#define FAN_Pin GPIO_PIN_4
#define FAN_GPIO_Port GPIOB
#define HEAT_Pin GPIO_PIN_5
#define HEAT_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_6
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_7
#define LCD_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
