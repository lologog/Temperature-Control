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
#include <stdlib.h>

//states of the machine
typedef enum{
	INITIAL_STATE,
	REGULATION_TYPE_STATE,
	BANG_BANG_STATE,
	PID_TEMP_STATE,
	PID_KP_STATE,
	PID_KI_STATE,
	PID_KD_STATE,
	PID_REGULATION_STATE,
	BANG_BANG_REGULATION_STATE,
	END_STATE
} RegualatorMachineState;

void RegulatorMachine_Init(void); //initial funcion of the machine - need to be used only once
void RegulatorMachine_Run(void); //state machine function - put in loop function

//states of the machine
static void HandleInitialState(void); //first state when you turn on the machine
static void HandleRegulationTypeState(void); //state after clicking start, you can choose your regulation type here
static void HandleBangBangState(void); // here user can choose the temperature that he wants to test
static void HandleBangBangRegulationState(void); // final Bang Bang regulation
static void HandleEndState(void); // the screen after regulation
static void HandlePIDTempState(void); // here user can choose temperature for PID regulation
static void HandlePIDKPState(void); // here user can choose Kp for the PID regulation
static void HandlePIDKIState(void);
static void HandlePIDKDState(void);
static void HandlePIDRegulationState(void); //actual PID regulation

#endif /* REGULATOR_MACHINE_H */
