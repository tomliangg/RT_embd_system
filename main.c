/*
 * Author: Tom Liang
 * Date: Feb 23, 2019
 * Objective: produce a 100Hz PWM waveform with adjustable duty cycles.
 * start with a zero duty cycle which is incremented by 10% each time when
 * SW1 is pressed. When the duty cycle reaches 100% it should be reset to 0%.
 * Use Module1 PWM0 (M1PWM2) and Module1 PWM1 (M1PWM3), where the IO pins to
 * be assigned for these PWM outputs are PE4 and PE5, respectively.
 */

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "PF4interrupts.h"


void PWM_Init(void);
void disable_interrupts(void);
void enable_interrupts(void);
void wait_for_interrupts(void);

volatile signed long ComparatorValue = 10000;

/* main */
int main(void){
    PF4_Init();
    PWM_Init();
    enable_interrupts();

    while(1){
        wait_for_interrupts();
    }
}

void PWM_Init(void) {
    SYSCTL_RCGCPWM_R |= 0x02;     // 1) enable PWM1 clock
    SYSCTL_RCGC2_R |= 0x10;          // 2.1) activate clock for PortE
    while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}; // 2.2) wait until PortE is ready
    GPIO_PORTE_AFSEL_R |= 0x30;       // 3) enable alt function on PE5-4
    GPIO_PORTE_PCTL_R &= 0x00FF0000; // 4.1) clear PE5-4 GPIOPCTL PMCx fields
    GPIO_PORTE_PCTL_R |= 0x00550000; // 4.2) configure PE5-4 as PWM Module 1
    GPIO_PORTE_AMSEL_R &= ~0x30;     // 4.3) disable analog functionality on PE5-4
    GPIO_PORTE_DEN_R |= 0x30;        // 4.4) enable digital I/O on PE5-4
    SYSCTL_RCC_R |= 0x00100000;      // 5.1) configure PWM clock divider as the source for PWM clock
    SYSCTL_RCC_R &= ~0x000E0000;     // 5.2) clear PWMDIV field
    SYSCTL_RCC_R |= 0x000060000;     // 5.3) set divisor to 16 so PWM clock source is 1 MHz
    PWM1_1_CTL_R = 0;                // 6.1) disable PWM while initializing; also configure Count-Down mode
    PWM1_1_GENA_R = 0x08C;           // 6.2) drives pwmA HIGH when counter matches value in PWM1LOAD
                                          // drive pwmA LOW when counter matches comparator A
    PWM1_1_GENB_R = 0x80C;           // 6.3) drives pwmB HIGH when counter matches value in PWM1LOAD
                                          // drive pwmB LOW when counter matches comparator B
    PWM1_1_LOAD_R = 10000 -1;        // 7) since target period is 100Hz, there are 10,000 clock ticks per period
    PWM1_1_CMPA_R = 10000 -10;        // 8) set 0% duty cycle to PE4
    PWM1_1_CMPB_R = 10000 -10;        // 9) set 0% duty cycle to PE5
    PWM1_1_CTL_R |= 0x01;            // 10) start the timers in PWM generator 1 by enabling the PWM clock
    PWM1_ENABLE_R |= 0x0C;           // 11) Enable M1PWM2 and M1PWM3
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

void PF4_Handler(void) {
    GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
    ComparatorValue -= 1000;
    if (ComparatorValue < 0) ComparatorValue = 10000; // reload to 10000 if it's less than 0
    PWM1_1_CMPA_R = ComparatorValue - 1; // update comparatorA value
    PWM1_1_CMPB_R = ComparatorValue - 1; // update comparatorB value
}
