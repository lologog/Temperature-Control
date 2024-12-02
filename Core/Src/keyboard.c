/*
 * keyboard.c
 *
 *  Created on: Dec 2, 2024
 *      Author: Krzysztof Tomicki
 */

#include "keyboard.h"

// Table of all keys on the keyboard
const char keyMap[4][3] = {
		{'1', '2', '3'}, //Row 1
		{'4', '5', '6'}, //Row 2
		{'7', '8', '9'}, //Row 3
		{'.', '0', '#'}  //Row 4
};

// Function to read the pressed key
char Keyboard_readKey(void)
{
    for (int row = 0; row < 4; row++) {
        // Set only one row to LOW, others to HIGH
        HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, row == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, row == 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, row == 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, row == 3 ? GPIO_PIN_RESET : GPIO_PIN_SET);

        // Check columns for the current row
        if (HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin) == GPIO_PIN_RESET) return keyMap[row][0];
        if (HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin) == GPIO_PIN_RESET) return keyMap[row][1];
        if (HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin) == GPIO_PIN_RESET) return keyMap[row][2];
    }

    // if no key is pressed, return '\0'
    return '\0';
}
