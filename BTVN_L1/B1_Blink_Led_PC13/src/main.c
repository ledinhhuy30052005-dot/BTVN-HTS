#include "stm32f10x.h"



static void delay(volatile uint32_t time)
{
    int i=0,j=0;
    for(i=0;i<time;i++){
        for(j=0;j<0x2aff;j++); 
    }
}

int main(void)
{
   // clock PortC
    RCC->APB2ENR |= 1<<4;
   // cleat bit cu
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |=  (0x3 << 20);


    while (1)
    {
        GPIOC->ODR &= ~(1 << 13);   // LED ON
        delay(100);

        GPIOC->ODR |= (1 << 13);    // LED OFF
        delay(100);
    }
}