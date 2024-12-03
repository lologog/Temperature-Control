/*
 * encoder.c
 *
 *  Created on: Dec 2, 2024
 *      Author: Krzysztof Tomicki
 */

#include "encoder.h"

// External declarations of timers
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

// Initialization of encoders
void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

// Get the position of a specific encoder
int16_t Encoder_GetPosition(uint8_t encoderNumber)
{
    switch (encoderNumber) {
        case 1:
            return (int16_t)__HAL_TIM_GET_COUNTER(&htim1) / 2;
        case 2:
            return (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
        case 3:
            return (int16_t)__HAL_TIM_GET_COUNTER(&htim3) / 2;
        default:
            return 0;
    }
}

// Get the button state of a specific encoder
uint8_t Encoder_GetButtonState(uint8_t encoderNumber)
{
    switch (encoderNumber) {
        case 1:
            return HAL_GPIO_ReadPin(ENC1_BUTTON_GPIO_Port, ENC1_BUTTON_Pin) == GPIO_PIN_RESET ? 1 : 0;
        case 2:
            return HAL_GPIO_ReadPin(ENC2_BUTTON_GPIO_Port, ENC2_BUTTON_Pin) == GPIO_PIN_RESET ? 1 : 0;
        case 3:
            return HAL_GPIO_ReadPin(ENC3_BUTTON_GPIO_Port, ENC3_BUTTON_Pin) == GPIO_PIN_RESET ? 1 : 0;
        default:
            return 0;
    }
}
