/*
 * API_debounce.h
 *
 * API para realizar debounce por software de un pulsador utilizando una maquina de estados finitos
 *
 *  Created on: Jul 24, 2023
 *      Author: estanislaocrivosgandini
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdbool.h>

#define DELAY_DUR 40 // Duracion del delay para pasar del estado BUTTON_FALLING a BUTTON_DOWN

void debounceFSM_init();
void debounceFSM_update();
bool readKey();

// Defino estados de la maq.
typedef enum
{
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RISING,
	BUTTON_UP
} debounceState_t;


#endif /* API_INC_API_DEBOUNCE_H_ */
