// Port E bits 3-0 have 4 piano keys

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Piano.h"


// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){
    unsigned long volatile delay;
    SYSCTL_RCGC2_R |= 0x10; // activate port E
    delay = SYSCTL_RCGC2_R; // allow time to finish activating
    GPIO_PORTE_AMSEL_R &= ~0x0F; // no analog
    GPIO_PORTE_PCTL_R &= ~0x0000FFFF; // regular GPIO function
    GPIO_PORTE_DIR_R &= ~0x0F; // make PE3-0 in
    GPIO_PORTE_AFSEL_R &= ~0x0F; // disable alt funct on PE3-0
    GPIO_PORTE_PDR_R |= 0x0F; // enable pulldown resistors on PE3-0
    GPIO_PORTE_DEN_R |= 0x0F; // enable digital I/O on PE3-0
}
