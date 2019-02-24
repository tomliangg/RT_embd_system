/*
 * PF4interrupts.c
 *
 *  Created on: Feb 24, 2019
 *      Author: Tom
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PF4interrupts.h"

extern void disable_interrupts(void);
extern void enable_interrupts(void);

/* Initialize PF4 (SW1) */
void PF4_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020;           // activate clock for PortF
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0)
    {};                          // wait until PortF is ready
    GPIO_PORTF_DIR_R &= ~0x10;              // make PF4 input
    GPIO_PORTF_AFSEL_R &= ~0x10;            // disable alt function on PF4
    GPIO_PORTF_DEN_R |= 0x10;               // enable digital I/O on PF4
    GPIO_PORTF_PCTL_R &= ~0x000F0000;       // use PF4 as GPIO
    GPIO_PORTF_AMSEL_R &= ~0x10;            // disable analog on PF4
    GPIO_PORTF_PUR_R = 0x10;                // enable pull-up on PF4
    GPIO_PORTF_IS_R &= ~0x10;               // PF4 is edge-sensitive
    GPIO_PORTF_IBE_R &= ~0x10;              // PF4 is not both edges
    GPIO_PORTF_IEV_R &= ~0x10;              // PF4 falling edge event
    GPIO_PORTF_ICR_R = 0x10;                // clear flag4
    GPIO_PORTF_IM_R |= 0x10;                // arm interrupt on PF4
    NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // priority 5
    NVIC_EN0_R = 0x40000000;                // enable interrupt 30 in NVIC
    enable_interrupts();                    // Enable global Interrupt flag (I)
}
