/*
 * encoder.h
 *
 *  Created on: Dec 2, 2024
 *      Author: Krzysztof Tomicki
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "main.h"

void Encoder_Init(void);
int16_t Encoder_GetPosition(uint8_t encoderNumber);
uint8_t Encoder_GetButtonState(uint8_t encoderNumber); // New function to read button states

#endif // ENCODER_H
