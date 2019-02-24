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

void PortF_Init(void);
void PWM_Init(void);
void disable_interrupts(void);
void enable_interrupts(void);
void wait_for_interrupts(void);

void GPIO_Handler(void);


/* main */
int main(void){
  PortF_Init();
  count = 0;

  enable_interrupts();

  while(1){                   // interrupts every 1ms
  }
}

void PWM_Init(void) {
    SYSCTL_RCGC0_R |= 0x0010000;     // 1) enable PWM clock
    SYSCTL_RCGC2_R |= 0x10;          // 2.1) activate clock for PortE
    while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}; // 2.2) wait until PortE is ready
    GPIO_PORTE_AFSEL_R = 0x30;       // 3) enable alt function on PE5-4
    GPIO_PORTE_PCTL_R &= 0x00FF0000; // 4.1) clear PE5-4 GPIOPCTL PMCx fields
    GPIO_PORTE_PCTL_R |= 0x00550000; // 4.2) configure PE5-4 as PWM Module 1
    SYSCTL_RCC_R |= 0x00100000;      // 5.1) configure PWM clock divider as the source for PWM clock
    SYSCTL_RCC_R &= ~0x000E000;      // 5.2) clear PWMDIV
    SYSCTL_RCC_R |= 0x00006000;      // 5.3) set divisor to 16 so PWM clock source is 1 MHz
    PWM1_1_CTL_R = 0;                // 6.1) disable PWM while initializing; also configure Count-Down mode
    PWM1_1_GENA_R = 0x8C;            // 6.2) drives pwmA HIGH when counter matches value in PWM1LOAD
                                          // drive pwmA LOW when counter matches comparator A
    PWM1_1_GENA_R = 0x8C;            // 6.3) drives pwmB HIGH when counter matches value in PWM1LOAD
    PWM1_1_LOAD_R = 0x186A0 -1;      // 7) since target period is 100Hz, there are 100,000 clock ticks per period
    PWM1_1_CMPA_R = 0x124F8 -1;      // 8) set 25% duty cycle to PE4
    PWM1_1_CMPB_R = 0x061A8 -1;      // 9) set 75% duty cycle to PE5
    PWM1_1_CTL_R = 0x01;             // 10) start the timers in PWM generator 1 by enabling the PWM clock
    PWM1_ENABLE_R = 0x0C;            // 11) Enable M1PWM2 and M1PWM3
}

/* Initialize PortF GPIOs */
void PortF_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020;           // activate clock for PortF
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0)
    {};                          // wait until PortF is ready
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // unlock GPIO PortF
    GPIO_PORTF_CR_R = 0x1F;                 // allow changes to PF4-0
    GPIO_PORTF_AMSEL_R = 0x00;              // disable analog on PortF
    GPIO_PORTF_PCTL_R = 0x00000000;         // use PF4-0 as GPIO
    GPIO_PORTF_DIR_R = 0x0E;                // PF4,PF0 in, PF3-1 out
    GPIO_PORTF_AFSEL_R = 0x00;              // disable alt function on PF
    GPIO_PORTF_PUR_R = 0x11;                // enable pull-up on PF0,PF4
    GPIO_PORTF_DEN_R = 0x1F;                // enable digital I/O on PF4-0
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

