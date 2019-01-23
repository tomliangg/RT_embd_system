/*
 * Author: Tom Liang
 * Date: Jan 23, 2019
 * Objective: read the state of SW1, and switch the Blue LED on if SW1 is closed
 * and switch it off if SW1 is open
 */


#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

void PortF_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020; // activate clock for PortF
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0) {}; // wait until PortF is ready
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock PortF PF0; unlocking is needed only for pins PC3-0, PD7, PF0
    GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00; // disable analog function
    GPIO_PORTF_PCTL_R = 0x00000000; // GPIO clear bit PCTL
    GPIO_PORTF_DIR_R = 0x0E; // PF4,PF0 input, PF3,PF2,PF1 output
    GPIO_PORTF_AFSEL_R = 0x00; // no alternate function
    GPIO_PORTF_PUR_R = 0x11; // enable pullup resistors on PF4,PF0
    GPIO_PORTF_DEN_R = 0x1F; // enable digital pins PF4-PF0
}

uint32_t SW1; // SW1 is PF4, located at bottom left corner, SW1 is HIGH when switch is NOT pressed
uint32_t Out; // outputs to PF3,PF2,PF1 (multicolor LED)

int main(void)
{
    PortF_Init();
    while (1) {
        SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1
        SW1 = SW1 >> 2; // 0x10 >> 2 = 0x04, shift into PF2 position
        Out = GPIO_PORTF_DATA_R;
        Out = Out & 0xFB; // clear bit at PF2 position
        Out = Out | SW1; // set bit according to SW1
        GPIO_PORTF_DATA_R = Out;
    }
	return 0;
}
