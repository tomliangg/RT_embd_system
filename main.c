#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

void PortF_Init(void) {
    SYSCTL_RCGC2_R |= 0x00000020;
    while ((SYSCTL_PRGPIO_R & 0x00000020) == 0) {};
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_PCTL_R = 0x00000000;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
}

uint32_t In;
uint32_t Out;

int main(void)
{
    PortF_Init();
    while (1) {
        In = GPIO_PORTF_DATA_R & 0x10;
        In = In >> 2;
        Out = GPIO_PORTF_DATA_R;
        Out = Out & 0xF1;
        Out = Out | In;
        GPIO_PORTF_DATA_R = Out;
    }
	return 0;
}
