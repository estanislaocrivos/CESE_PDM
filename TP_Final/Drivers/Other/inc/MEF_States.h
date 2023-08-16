/*
 * API_States.h
 *
 * This library contains the Finite State Machine that constitutes the system.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#ifndef INC_MEF_STATES_H_
#define INC_MEF_STATES_H_

#include "stdint.h"
#include "Person_Structure.h"

uint8_t MEF_Idle(uint8_t * Card_ID);
uint8_t MEF_Authenticate(uint8_t * Card_ID, Person *People, uint8_t *ID_Index);
void MEF_Start_Shift(uint8_t * Card_ID, Person *People, uint8_t *ID_Index);
void MEF_Conclude_Shift(uint8_t * Card_ID, Person *People, uint8_t *ID_Index);
void MEF_Error(void);

#endif /* INC_MEF_STATES_H_ */
