#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

void MSDealy(unsigned int itime) {
    unsigned int i;
    unsigned int j;
    for (i=0; i<itime; i++) {
        for (j=0; j<331; j++);
    }
}

void PortF_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020; // F clock
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0) {}; // delay
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
uint32_t SW2; // SW2 is PF0, located at bottom right corner
uint8_t state = 0;

// at the very beginning the program should keep all the LEDs off and wait until SW1 is pressed, after which
// it starts to switch on and off indefinitely the three LEDs according to the sequence of "RED->
// BLUE->GREEN->RED". You need to add a time delay between the switchings to make the
// flashing visible. Under this mode, if the user presses SW2 the program should goes back to the
// initial state (all LEDs off and waiting for SW1 to be pressed).

int main(void)
{
    PortF_Init();
    GPIO_PORTF_DATA_R = 0x00; // keep all the LEDs off and wait until SW1 is pressed
    SW1 = GPIO_PORTF_DATA_R & 0x10; // read PF4 into SW1
    SW2 = GPIO_PORTF_DATA_R & 0x01; // read PF0 into SW2
    while (1) {
        SW1 = GPIO_PORTF_DATA_R & 0x10; // update SW1
        SW2 = GPIO_PORTF_DATA_R & 0x01; // update SW2
        if (state == 0) {
            GPIO_PORTF_DATA_R = 0x00;
            if (!SW1) state = 1;
        }
        else if (state == 1) {
            GPIO_PORTF_DATA_R &= 0x81;
            GPIO_PORTF_DATA_R |= 0x02; // red
            MSDealy(500);
            GPIO_PORTF_DATA_R &= 0x81;
            GPIO_PORTF_DATA_R |= 0x04; // blue
            MSDealy(500);
            GPIO_PORTF_DATA_R &= 0x81;
            GPIO_PORTF_DATA_R |= 0x08; // green
            MSDealy(500);
            if (!SW2) state = 0;
        }
    }
	return 0;
}
