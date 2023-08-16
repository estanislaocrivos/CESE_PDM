/*
 * API_States.c
 *
 * This library contains the functions that are executed in each state of the
 * Finite State Machine that constitutes the system.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "MEF_States.h"
#include "API_LCD.h"
#include "stm32f1_rc522.h"
#include "Person_Structure.h"
#include "API_delay.h"
#include "API_Tools.h"

#define MAX_PEOPLE 3
#define ID_LENGTH 16

/* Import timer counter variable that tracks time from MCU initialization */
extern volatile uint32_t Timer_Count;

/**
  * @brief This function executes in IDLE state.
  *
  *	This function refreshes RFID sensor and waits until a card is present.
  *
  *	@param uint8_t *Card_ID This is a pointer to a string where the ID will be stored.
  * @retval uint8_t It returns 0 when a card is present.
  */
uint8_t MEF_Idle(uint8_t *Card_ID)
{
	uint8_t status = 1;

	Red_LED_ON();
	Green_LED_OFF();

	/* Welcome display */
	LCD_Print(((uint8_t *)"    Welcome!    "), ((uint8_t *)" Present ID Tag "));

	/* Polls RFID sensor waiting for card presence */
	status = RFID_Sense(Card_ID);

	return status;
}

/**
  * @brief This function is executed in the AUTHENTICATE state.
  *
  * This function is executed when a card is present in front of the reader. It
  * authenticates its ID and checks is the ID exists in the database, in which case
  * it checks if it has already been registered or if it is not registered yet. 
  * If the ID exists, it writes its index inside the array in the variable ID_Index.
  *
  * @param uint8_t * Card_ID, Person *People It takes the card ID to authenticate and a pointer to the
  * database structure array.
  * @retval uint8_t It returns a value according to the result of the authentication.
  */
uint8_t MEF_Authenticate(uint8_t * Card_ID, Person *People, uint8_t *ID_Index)
{
	delay_t delay;
	tick_t Delay_Duration = 1000;

	Buzzer_Toggle();
	
	/* Show message */
	delayInit(&delay, Delay_Duration);
	while (delayRead(&delay) != true)
	{
		LCD_Print(((uint8_t *)" Authenticating "), ((uint8_t *)"    RFID Tag    "));
	}

	Buzzer_Toggle();

	/* Go over database structure array and find Card_ID */
	for(uint8_t i = 0; i < MAX_PEOPLE; i++)
	{
		uint8_t Equal = 1;

		/* Compare Card_ID and stored ID in database for each structure (authentication) */
		for (uint8_t j = 0; j < 10; j++)
		{
			if (Card_ID[j] != People[i].ID[j])
			{
				Equal = 0;
				break;
			}
		}

		/* If Card_ID and one ID inside the structure array match... */
		if (Equal == 1)
		{
			Red_LED_Toggle();
			Green_LED_Toggle();

			/* Stores structure ID index in variable ID_Index */
			*ID_Index = i;

			if (People[*ID_Index].Registered == 1)
			{
				/* If ID has already been registered */
				return 1;
			}
			else if (People[*ID_Index].Registered == 0)
			{
				/* If ID is not registered */
				return 2;
			}
		}
	}
	return 3;
}

/**
  * @brief This function is executed in the START_SHIFT state.
  *
  * This function is executed after ID authentication if the ID has never
  * been registered. It starts the shift.
  *
  * @param uint8_t * Card_ID, Person *People It takes the card ID to authenticate, a pointer to the
  * database structure array and a pointer to the ID index inside the array.
  * @retval void
  */
void MEF_Start_Shift(uint8_t * Card_ID, Person *People, uint8_t *ID_Index)
{
	delay_t delay;
	tick_t Delay_Duration = 1000;

	delayInit(&delay, Delay_Duration);
	while (delayRead(&delay) != true)
	{
		LCD_Print(((uint8_t *)" Starting Shift "), ((uint8_t *)"       OK       "));
	}

	/* Set registered state inside ID structure and save current time */
	People[*ID_Index].Registered = 1;
	People[*ID_Index].Time_Counter = Timer_Count;

	return;
}

/**
  * @brief This function is executed in the END_SHIFT state.
  *
  * This function is executed after ID authentication if the ID has already
  * been registered. It ends the shift.
  *
  * @param uint8_t * Card_ID, Person *People It takes the card ID to authenticate, a pointer to the
  * database structure array and a pointer to the ID index inside the array.
  * @retval void
  */
void MEF_Conclude_Shift(uint8_t * Card_ID, Person *People, uint8_t *ID_Index)
{
	uint32_t elapsed_time = 0;
	uint32_t hours = 0;
	uint32_t minutes = 0;
	uint32_t seconds = 0;

	char Formatted_Time[50] = "";

	delay_t delay;
	tick_t Delay_Duration = 1000;

	/* Non-blocking delay for showing message on LCD */
	delayInit(&delay, Delay_Duration);
	while (delayRead(&delay) != true)
	{
		LCD_Print(((uint8_t *)"  Ending Shift  "), ((uint8_t *)"      ....      "));
	}

	/* Register elapsed time from ID registration and reset the ID states */
	elapsed_time = Timer_Count - People[*ID_Index].Time_Counter;
	People[*ID_Index].Registered = 0;
	People[*ID_Index].Time_Counter = 0;

	/* Format elapsed time in the form HH:MM:SS */
	hours = elapsed_time / 3600;
	minutes = (elapsed_time % 3600) / 60;
	seconds = elapsed_time % 60;
	sprintf(Formatted_Time, "    %02lu:%02lu:%02lu    ", hours, minutes, seconds);

	/* Non-blocking delay for showing message on LCD */
	delayInit(&delay, Delay_Duration);
	while (delayRead(&delay) != true)
	{
		/* Show elapsed time on display */
		LCD_Print(((uint8_t *)"  Elapsed Time  "), ((uint8_t *)Formatted_Time));
	}

	return;
}

/**
  * @brief This function is executed inside the AUTHENTICATE state.
  *
  * This function is executed if authentication failed due to non-existant ID in the database.
  *
  * @param void
  * @retval void
  */
void MEF_Error(void)
{
	delay_t delay;
	tick_t Delay_Duration = 1000;

	/* Non-blocking delay for showing message on LCD */
	delayInit(&delay, Delay_Duration);
	while (delayRead(&delay) != true)
	{
		LCD_Print(((uint8_t *)"  Non-Existent  "), ((uint8_t *)"       ID       "));
	}
}
