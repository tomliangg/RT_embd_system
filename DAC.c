// Port B bits 3-0 have the 4-bit DAC

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "DAC.h"


// **************DAC_Init*********************
// Initialize 4-bit DAC on PortB
void DAC_Init(void) {
    unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x02;            // activate port B
	delay = SYSCTL_RCGC2_R;            // allow time to finish activating
	GPIO_PORTB_AMSEL_R &= ~0x0F;       // no analog
	GPIO_PORTB_PCTL_R &= ~0x0000FFFF;  // regular GPIO function
	GPIO_PORTB_DIR_R |= 0x0F;          // make PB3-0 out
	GPIO_PORTB_DR8R_R |= 0x0F;         // can drive up to 8mA out
	GPIO_PORTB_AFSEL_R &= ~0x0F;       // disable alt funct on PB3-0
	GPIO_PORTB_DEN_R |= 0x0F;          // enable digital I/O on PB3-0
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
void DAC_Out(unsigned long data) {
	GPIO_PORTB_DATA_R = data;
}
