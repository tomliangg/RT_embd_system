/*
 * PF4interrupts.h
 *
 *  Created on: Feb 24, 2019
 *      Author: Tom
 */

#ifndef PF4INTERRUPTS_H_
#define PF4INTERRUPTS_H_

// initialize PF4 as input and configure edge triggered interrupts
void PF4_Init(void);

// ISR for PF4
void PF4_Handler(void);

#endif /* PF4INTERRUPTS_H_ */
