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
}

void RegulatorMachine_Run(void)
{

}
