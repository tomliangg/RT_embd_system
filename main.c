/*
 * Author: Tom Liang
 * Date: March 16, 2019
 * Objective: Build a 4-key digital piano with DAC, GPIO and Systick ISR.
 * When PE0 is pressed, output note C.
 * When PE1 is pressed, output note D.
 * When PE2 is pressed, output note E.
 * When PE3 is pressed, output note G.
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
int main(void) {
    disable_interrupts();
    PLL_Init();         // 80MHz clock
    DAC_Init();         // configure PB3-0 for DAC
    Piano_Init();       // configure PE3-0 as input pins
    Sound_Init(8000);    // setup Systick interrupts with 10KHz
    enable_interrupts();

    while(1) {
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
