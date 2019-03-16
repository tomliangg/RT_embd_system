/*
 * Author: Tom Liang
 * Date: Feb 23, 2019
 * Objective: produce a 100Hz PWM waveform with adjustable duty cycles.
 * start with a zero duty cycle which is incremented by 10% each time when
 * SW1 is pressed. When the duty cycle reaches 100% it should be reset to 0%.
 * Use Module1 PWM0 (M1PWM2) and Module1 PWM1 (M1PWM3), where the IO pins to
 * be assigned for these PWM outputs are PE4 and PE5, respectively.
 */

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "DAC.h"
#include "Sound.h"
#include "Piano.h"
#include "PLL.h"

void disable_interrupts(void);
void enable_interrupts(void);
void wait_for_interrupts(void);



/* main */
int main(void){
    disable_interrupts();
    PLL_Init();         // 80MHz clock
    DAC_Init();         // configure PB3-0 for DAC
    Piano_Init();       // configure PE3-0 as input pins
    Sound_Init();       // setup Systick interrupts
    enable_interrupts();

    while(1){
        wait_for_interrupts();
    }
}


/* Disable interrupts by setting the I bit in the PRIMASK system register */
void disable_interrupts(void) {
    __asm("    CPSID  I\n"
          "    BX     LR");
}


/* Enable interrupts by clearing the I bit in the PRIMASK system register */
void enable_interrupts(void) {
    __asm("    CPSIE  I\n"
          "    BX     LR");
}


/* Enter low-power mode while waiting for interrupts */
void wait_for_interrupts(void) {
    __asm("    WFI\n"
          "    BX     LR");
}
