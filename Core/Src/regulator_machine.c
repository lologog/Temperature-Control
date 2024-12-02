/*
 * regulator_machine.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Krzysztof Tomicki
 */

#include "regulator_machine.h"

//global state of the machine
static RegualatorMachineState currentState = INITIAL;

void RegulatorMachine_Init(void)
{
	lcd_init();
	BMP280_Init(&hi2c2, BMP280_TEMPERATURE_16BIT, BMP280_STANDARD, BMP280_FORCEDMODE);
	Encoder_Init();
}

void RegulatorMachine_Run(void)
{
	HAL_GPIO_WritePin(SETTINGS_LED_GPIO_Port, SETTINGS_LED_Pin, 1);
}
