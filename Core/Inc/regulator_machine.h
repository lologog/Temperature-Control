/*
 * regulator_machine.h
 *
 *  Created on: Nov 30, 2024
 *      Author: Krzysztof Tomicki
 */

#ifndef REGULATOR_MACHINE_H
#define REGULATOR_MACHINE_H

//here are gonna be all includes
#include "i2c-lcd.h"
#include "BMPXX80.h"
#include "keyboard.h"
#include "encoder.h"

#include <stdbool.h>

//states of the machine
typedef enum{
	INITIAL_STATE,
	REGULATION_TYPE_STATE
} RegualatorMachineState;

void RegulatorMachine_Init(void); //initial funcion of the machine - need to be used only once
void RegulatorMachine_Run(void); //state machine function - put in loop function

//states of the machine
static void HandleInitialState(void);

#endif /* REGULATOR_MACHINE_H */
