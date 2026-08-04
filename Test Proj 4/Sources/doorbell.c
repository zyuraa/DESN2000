#include "../Headers/stm32f103xe.h"
#include "stdio.h"
#include "songs.h"

void button_init() {

    // enable clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

    // reset registers
    GPIOE->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);

    // set input pull up/down 00 10
    GPIOE->CRL |= GPIO_CRL_CNF1_1;

    // set active low (pull up)
    //GPIOE->ODR |= GPIO_ODR_ODR1;
}

/*
returns 1 or 0 reading the input data register
*/
void read_doorbell() {
    static unsigned int prev = 0;
    unsigned int btn = (GPIOE->IDR & GPIO_IDR_IDR1) >> 1;

    if (prev != btn) {
        play_doorbell();
    }

    prev = btn;
}