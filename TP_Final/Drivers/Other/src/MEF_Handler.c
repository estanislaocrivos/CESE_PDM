/*
 * MEF_Handler.c
 *
 * This library contains the definition of the MEF that constitutes the system.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "stm32f4xx.h"
#include "MEF_Handler.h"
#include "stdint.h"
#include "Person_Structure.h"
#include "MEF_States.h"
#include "API_LCD.h"

MEF_State_t SYSTEM_STATE;

/**
  * @brief This function initializes the MEF.
  *
  * @param void
  * @retval void
  */
void MEF_Init(void)
{
	SYSTEM_STATE = IDLE;
}

/**
  * @brief This function updates the MEF.
  *
  * This function is executed recursively in the main while(1) loop
  * for updating the MEF according to the inputs and outputs.
  *
  * @param Person *People It takes a pointer to the database structure array.
  * @retval void
  */
void MEF_Update(Person *People)
{
	uint8_t Card_ID[16];
	uint8_t Auth_Status;
	uint8_t ID_Index;

	switch(SYSTEM_STATE)
	{
		case IDLE:
			/* If a card is detected, change status to Authenticate... */
			if(MEF_Idle(Card_ID) == 0)
			{		
				SYSTEM_STATE = AUTHENTICATE;
			}
			break;
		
		case AUTHENTICATE:
			Auth_Status = MEF_Authenticate(Card_ID, People, &ID_Index);

			/* If the ID has been already registered, end shift... */
			if(Auth_Status == 1)
			{
				SYSTEM_STATE = END_SHIFT;
			}
			/* If the ID was not registered, start shift... */
			else if(Auth_Status == 2)
			{
				SYSTEM_STATE = START_SHIFT;
			}
			/* If the ID is non-existent... */
			else if(Auth_Status == 3)
			{
				MEF_Error();
				SYSTEM_STATE = IDLE;
			}
			break;

		case START_SHIFT:
			/* Start shift and return to idle state */
			MEF_Start_Shift(Card_ID, People, &ID_Index);
			SYSTEM_STATE = IDLE;
			break;

		case END_SHIFT:
			/* End shift and return to idle state */
			MEF_Conclude_Shift(Card_ID, People, &ID_Index);
			SYSTEM_STATE = IDLE;
			break;
	}
}
