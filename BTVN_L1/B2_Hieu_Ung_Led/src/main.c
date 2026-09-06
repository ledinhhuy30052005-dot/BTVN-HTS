#include "stm32f10x.h"

static void delay(volatile uint32_t time)
{
    volatile uint32_t i;
    volatile uint32_t j;

    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 0x2aff; j++);
    }
}

int main(void)
{
   // Clock PA
    RCC->APB2ENR |= (1 << 2);

    GPIOA->CRL &=~ 0xFFFFFFFF;
    GPIOA->CRL |=  0x33333333;


    while (1)
    {
        // PA0 -> PA7

        for (int i = 0; i < 8; i++)
        {
            GPIOA->ODR = (1 << i);

            delay(50);
        }


        for (int i = 6; i >= 0; i--)
        {
            GPIOA->ODR = (1 << i);

            delay(50);
        }
    }
}
