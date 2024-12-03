/*
 * regulator_machine.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Krzysztof Tomicki
 */

#include "regulator_machine.h"

//global state of the machine
static RegualatorMachineState currentState = INITIAL_STATE;

void RegulatorMachine_Init(void)
{
	lcd_init();
	BMP280_Init(&hi2c2, BMP280_TEMPERATURE_16BIT, BMP280_STANDARD, BMP280_FORCEDMODE);
	Encoder_Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void RegulatorMachine_Run(void)
{
	switch (currentState)
	{
		case INITIAL_STATE:
			HandleInitialState();
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleInitialState(void)
{
	static uint32_t last_toggle_time = 0;
	static bool show_click_start = true;

	// settings leds on the board
	HAL_GPIO_WritePin(SETTINGS_LED_GPIO_Port, SETTINGS_LED_Pin, 1);
	HAL_GPIO_WritePin(REGULATION_LED_GPIO_Port, REGULATION_LED_Pin, 0);
	HAL_GPIO_WritePin(HEATING_LED_GPIO_Port, HEATING_LED_Pin, 0);

	// first line of text
	lcd_put_cur(0, 2);
	lcd_send_string("TEMP CONTROL");
	lcd_put_cur(1, 2);

	//blinking of the second line of text
	if (HAL_GetTick() - last_toggle_time >= 1000)
	{
		if (show_click_start)
		{
			lcd_send_string("CLICK START");
		}
		else
		{
			lcd_send_string("                ");
		}

		last_toggle_time = HAL_GetTick();
		show_click_start = !show_click_start;
	}

	//click start to start choosing regulation type
	if (HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin) == 1)
	{
		currentState = REGULATION_TYPE_STATE;
	}
}


