/*
 * MEF_Handler.h
 *
 *  Created on: Aug 11, 2023
 *      Author: Lenovo
 */

#ifndef INC_MEF_HANDLER_H_
#define INC_MEF_HANDLER_H_

#include "Person_Structure.h"

// DEFINO MAQ DE ESTADOS
typedef enum
{
	IDLE,
	AUTHENTICATE,
	START_SHIFT,
//	CONFIRM_END_SHIFT,
	END_SHIFT
} MEF_State_t;

void MEF_Init(void);

void MEF_Update(Person *People);




#endif /* INC_MEF_HANDLER_H_ */
