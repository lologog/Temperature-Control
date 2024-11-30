/*
 * regulator_machine.h
 *
 *  Created on: Nov 30, 2024
 *      Author: admin
 */

#ifndef REGULATOR_MACHINE_H
#define REGULATOR_MACHINE_H

//here are gonna be all includes
#include "i2c-lcd.h"
#include "BMPXX80.h"

//states of the machine
typedef enum{
	INITIAL
} RegualatorMachineState;

void RegulatorMachine_Init(void); //initial funcion of the machine - need to be used only once
void RegulatorMachine_Run(void); //state machine function - put in loop function

#endif /* REGULATOR_MACHINE_H */
