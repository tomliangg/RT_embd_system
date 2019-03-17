#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "DAC.h"

unsigned char index = 0;
const unsigned char SineWave[32] = {8,9,10,11,12,13,14,15,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,1,2,3,4,5,6,7};
unsigned long period = 0x05;
unsigned char SW_current;  // current switch values for portE
unsigned char SW_prev;  // previous switch values for portE


// **************Sound_Init*********************
// Initialize Systick periodic interrupts
void Sound_Init(unsigned long period) {
    index = 0;
    NVIC_ST_CTRL_R = 0;  // disable SysTick during setup
    NVIC_ST_RELOAD_R = period-1;  // reload value
    NVIC_ST_CURRENT_R = 0;  // any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;  //set priority to 1
    NVIC_ST_CTRL_R = 0x0007;  // enable,core clock, and interrupts
}


// **************Sound_Off*********************
// stop outputing to DAC
void Sound_Off(void) {
 // this routine stops the sound output
    GPIO_PORTB_DATA_R &= ~0x0F;
}


// Interrupt service routine
void SysTick_Handler(void) {
    SW_current = GPIO_PORTE_DATA_R & 0x0F;  // read PortE into SW_current
    if ((SW_current != SW_prev) && SW_current) {
        if (GPIO_PORTE_DATA_R & 0x01) period = 80000000 / (32*523.251);  // set note C pitch
        if (GPIO_PORTE_DATA_R & 0x02) period = 80000000 / (32*587.330);  // set note D pitch
        if (GPIO_PORTE_DATA_R & 0x04) period = 80000000 / (32*659.255);  // set note E pitch
        if (GPIO_PORTE_DATA_R & 0x08) period = 80000000 / (32*783.991);  // set note G pitch
        Sound_Init(period);  //set the new period and frequency to Systick ISR
    }

    if (SW_current) {
        index = (index + 1) & 0x1F;
        DAC_Out(SineWave[index]);  // output one value at each interrupt
    } else if (!SW_current) Sound_Off();

    SW_prev = SW_current;
}
