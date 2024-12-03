/*
 * regulator_machine.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Krzysztof Tomicki
 */

#include "regulator_machine.h"

//global state of the machine
static RegualatorMachineState currentState = INITIAL_STATE;
static RegualatorMachineState previousState = INITIAL_STATE;

static float finalTemperature = 0;

void RegulatorMachine_Init(void)
{
	lcd_init();
	BMP280_Init(&hi2c2, BMP280_TEMPERATURE_16BIT, BMP280_STANDARD, BMP280_FORCEDMODE);
	Encoder_Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void RegulatorMachine_Run(void)
{
	//clear the screen after every state change
	if (currentState != previousState)
	{
		lcd_clear();
		previousState = currentState;
	}

	switch (currentState)
	{
		case INITIAL_STATE:
			HandleInitialState();
			break;
		case REGULATION_TYPE_STATE:
			HandleRegulationTypeState();
			break;
		case BANG_BANG_STATE:
			HandleBangBangState();
			break;
		case BANG_BANG_REGULATION_STATE:
			HandleBangBangRegulationState();
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleRegulationTypeState(void)
{
	static char pressedKey = '\0';
	// settings leds on the board
	HAL_GPIO_WritePin(SETTINGS_LED_GPIO_Port, SETTINGS_LED_Pin, 1);
	HAL_GPIO_WritePin(REGULATION_LED_GPIO_Port, REGULATION_LED_Pin, 0);
	HAL_GPIO_WritePin(HEATING_LED_GPIO_Port, HEATING_LED_Pin, 0);

	//text with users regulation types to click
	lcd_put_cur(0, 0);
	lcd_send_string("1-BANG BANG CONT");
	lcd_put_cur(1, 0);
	lcd_send_string("2-PID CONT");

	//choosing regulation type on keyboard
	pressedKey = Keyboard_readKey();
	if (pressedKey == '1')
	{
		currentState = BANG_BANG_STATE;
	}
	else if (pressedKey == '2')
	{
		currentState = PID_STATE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleBangBangState(void)
{
	static char writtenTemperature[5] = {0};
	static uint8_t length = 0;
	static bool delayStarted = false;
	static uint32_t enterTime = 0;
	static bool keyReleased = true;

	lcd_put_cur(0, 0);
	lcd_send_string("CHOOSE TEMP");

	//one time delay to not get the 1 from the previous state
	if (!delayStarted)
	{
		enterTime = HAL_GetTick();
		delayStarted = true;
	}

	if (HAL_GetTick() - enterTime >= 500)
	{
		//read the clicked number
		char currentKey = Keyboard_readKey();

		//checking if the key is released
		if (currentKey == '\0')
		{
			keyReleased = true;
		}

		// if the key was pressed and released
		if (currentKey != '\0' && keyReleased)
		{
			keyReleased = false;

			length = strlen(writtenTemperature);
			//checing if there is still place in the table
			if (length < sizeof(writtenTemperature) - 1)
			{
				writtenTemperature[length] = currentKey;
				writtenTemperature[length+1] = '\0';
			}
		}
	}

	lcd_put_cur(1, 0);
	lcd_send_string(writtenTemperature);

	//accepting written temperature
	if (HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin) == 1)
	{
		finalTemperature = atof(writtenTemperature);
		memset(writtenTemperature, 0, strlen(writtenTemperature)); //clear the table
		currentState = BANG_BANG_REGULATION_STATE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleBangBangRegulationState(void)
{

}

