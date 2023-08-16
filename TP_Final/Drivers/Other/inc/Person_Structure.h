/*
 * Person_Structure.h
 *
 *  Created on: Aug 12, 2023
 *      Author: Lenovo
 */

#ifndef INC_PERSON_STRUCTURE_H_
#define INC_PERSON_STRUCTURE_H_

#include "stdint.h"

typedef struct
{
    char ID[16];
    uint8_t Registered;
    uint8_t Time_Counter;
} Person;

#endif /* INC_PERSON_STRUCTURE_H_ */
