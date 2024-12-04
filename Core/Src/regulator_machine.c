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
static float globalKp = 0.0;
static float globalKi = 0.0;
static float globalKd = 0.0;

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
		case END_STATE:
			HandleEndState();
			break;
		case PID_TEMP_STATE:
			HandlePIDTempState();
			break;
		case PID_KP_STATE:
			HandlePIDKPState();
			break;
		case PID_KI_STATE:
			HandlePIDKIState();
			break;
		case PID_KD_STATE:
			HandlePIDKDState();
			break;
		case PID_REGULATION_STATE:
			HandlePIDRegulationState();
			break;
		default:
			currentState = REGULATION_TYPE_STATE;
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
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);

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
		currentState = PID_TEMP_STATE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleBangBangState(void)
{
	static char writtenTemperature[6] = {0};
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
		delayStarted = false;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleBangBangRegulationState(void)
{
	static bool targetReached = false;
	static float temperature = 0;
	static uint32_t pressure = 0;
	static float tolerance = 0.1;
	static uint32_t timeReached = 0.0;
	static uint32_t regulationTimeLimit = 30000;

	HAL_GPIO_WritePin(REGULATION_LED_GPIO_Port, REGULATION_LED_Pin, 1);
	HAL_GPIO_WritePin(SETTINGS_LED_GPIO_Port, SETTINGS_LED_Pin, 0);

	BMP280_ReadTemperatureAndPressure(&temperature, &pressure);
	static char buffer1[16];
	static char buffer2[16];

	//changing table of chars into float and putting it on the screen
	sprintf(buffer1, "%.2f", finalTemperature);
	lcd_put_cur(0, 0);
	lcd_send_string("SETPOINT    ");
	lcd_put_cur(0, 11);
	lcd_send_string(buffer1);

	//measured temperature
	sprintf(buffer2, "%.2f", temperature);
	lcd_put_cur(1, 0);
	lcd_send_string("ACTUAL  ");
	lcd_put_cur(1, 11);
	lcd_send_string(buffer2);

	//bang bang regulation
	if (temperature <= finalTemperature - tolerance)
	{
		HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, 1);
		HAL_GPIO_WritePin(HEATING_LED_GPIO_Port, HEATING_LED_Pin, 1);
		HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, 0);
	}
	else if (temperature >= finalTemperature + tolerance)
	{
		HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, 0);
		HAL_GPIO_WritePin(HEATING_LED_GPIO_Port, HEATING_LED_Pin, 0);
		HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, 1);
	}
	else
	{
		if (!targetReached)
		{
			targetReached = true;
			timeReached = HAL_GetTick();
		}
	}

	if (targetReached && (HAL_GetTick() - timeReached >= regulationTimeLimit))
	{
		targetReached = false;
		currentState = END_STATE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandleEndState(void)
{
	static uint32_t lastBuzzerToggleTime = 0;
	static uint32_t lastTextToggleime = 0;
	static uint32_t buzzerInterval = 500;
	static bool buzzerState = false;
	static bool showClickStart = true;

	HAL_GPIO_WritePin(HEATING_LED_GPIO_Port, HEATING_LED_Pin, 0);
	HAL_GPIO_WritePin(REGULATION_LED_GPIO_Port, REGULATION_LED_Pin, 0);
	HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, 0);
	HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, 0);

	// beeping logic
	if (HAL_GetTick() - lastBuzzerToggleTime >= buzzerInterval)
	{
		lastBuzzerToggleTime = HAL_GetTick();
		buzzerState = !buzzerState;
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, buzzerState ? 1 : 0);
	}

	// blinking "CLICK START" text
	if (HAL_GetTick() - lastTextToggleime >= 1000)
	{
		lcd_put_cur(0, 4);
		lcd_send_string("SUCCESS");
		lcd_put_cur(1, 2);

		if (showClickStart)
		{
			lcd_send_string("CLICK START");
		}
		else
		{
			lcd_send_string("             ");
		}

		lastTextToggleime = HAL_GetTick();
		showClickStart = !showClickStart;
	}

	//changing the state
	if (HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin) == 1)
	{
		currentState = REGULATION_TYPE_STATE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDTempState(void)
{
	static char writtenTemperature[6] = {0};
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
		currentState = PID_KP_STATE;
		delayStarted = false;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDParameter(const char *label, float *globalParam, uint8_t encoderNumber, RegualatorMachineState nextState)
{
    static float lastValue = -1.0; // last parameter value - not 0 cuz someone can set 0.0
    static bool startReleased = false;
    char buffer[16];

    //get encoder value and set it from int to float 1->0.1
    int16_t rawValue = Encoder_GetPosition(encoderNumber);
    float value = rawValue / 10.0;

    //parameter cannot be negative
    if (rawValue < 0)
    {
        Encoder_ResetPosition(encoderNumber);
        value = 0.0;
    }

    // reset screen if value is different
    if (value != lastValue)
    {
        lcd_put_cur(0, 0);
        lcd_send_string("CHOOSE ");
        lcd_send_string(label);

        lcd_put_cur(1, 0);
        lcd_send_string("                ");

        snprintf(buffer, sizeof(buffer), "%s: %.1f", label, value);
        lcd_put_cur(1, 0);
        lcd_send_string(buffer);

        lastValue = value;
    }

    //user has to unpush the button
    if (HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin) == GPIO_PIN_RESET)
    {
        startReleased = true;
    }

    //if the button was released and then clicked again go to next state
    if (startReleased && HAL_GPIO_ReadPin(START_STOP_GPIO_Port, START_STOP_Pin) == GPIO_PIN_SET)
    {
        *globalParam = value; //write the value as global value for parameter
        currentState = nextState;
        startReleased = false;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDKPState(void)
{
    HandlePIDParameter("Kp", &globalKp, 2, PID_KI_STATE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDKIState(void)
{
    HandlePIDParameter("Ki", &globalKi, 3, PID_KD_STATE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDKDState(void)
{
    HandlePIDParameter("Kd", &globalKd, 1, PID_REGULATION_STATE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//function to calculate the PID control output - THE HARD PART
float PID_Regulation(float setpoint, float measuredValue, float* lastError, float* integral, float dt)
{
    //calculate error
    float error = setpoint - measuredValue;

    //calculate proportional term
    float proportional = globalKp * error;

    //calculate integral term
    *integral += error * dt;
    float integralTerm = globalKi * (*integral);

    //calculate derivative term
    float derivative = (error - *lastError) / dt;
    float derivativeTerm = globalKd * derivative;

    //update last error
    *lastError = error;

    //compute control output
    float controlOutput = proportional + integralTerm + derivativeTerm;

    // Limit control output to range 0-100
    if (controlOutput > 100.0) controlOutput = 100.0;
    if (controlOutput < 0.0) controlOutput = 0.0;

    return controlOutput;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HandlePIDRegulationState(void)
{
    static float lastError = 0.0; //previous error variable
    static float integral = 0.0;  //accumulated integral of the error
    static uint32_t lastTime = 0;  //last time update

    //read current temperature and pressure
    float temperature = 0.0;
    uint32_t pressure = 0;
    BMP280_ReadTemperatureAndPressure(&temperature, &pressure);

    //delta t: (t1 - t0) = delta
    uint32_t currentTime = HAL_GetTick();
    float dt = (currentTime - lastTime) / 1000.0; //+ convert milliseconds to seconds
    lastTime = currentTime;

    //main PID algorithm
    float controlOutput = PID_Regulation(finalTemperature, temperature, &lastError, &integral, dt);

    //display setpoint and current temperature on the LCD
    char buffer[16];
    lcd_put_cur(0, 0);
    snprintf(buffer, sizeof(buffer), "SP: %.2f", finalTemperature); //final temperature
    lcd_send_string(buffer);

    lcd_put_cur(1, 0);
    snprintf(buffer, sizeof(buffer), "TEMP: %.2f", temperature); //current temperture
    lcd_send_string(buffer);

    //control the heating and cooling system based on control output
    if (controlOutput > 50.0) //threshold for activating the heating system cuz there is a hardware issue with PWM singal for fan and heat
    {
        HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);
    }
}
