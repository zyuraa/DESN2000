#include "../Headers/stm32f103xe.h"
#include "stdio.h"

void button_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);

    // set MODE and CNF for input
    GPIOE->CRL |= GPIO_CRL_CNF1_0 | GPIO_CRL_MODE1_0;
}

/*
returns 1 or 0 reading the input data register
*/
void read_doorbell() {
	unsigned int btn;
    unsigned int prev;

    // read btn from IDR and set prev to not pressed
    btn = GPIOE->IDR & GPIO_IDR_IDR1_Msk;
    prev = ~GPIO_IDR_IDR1_Pos;

    // if btn pressed ring doorbell
    if ((btn == GPIO_IDR_IDR1_Pos) && (prev != GPIO_IDR_IDR1_Pos)) {
        // imported function from songs
        ring_doorbell();

        prev = GPIO_IDR_IDR1_Pos;
    }

    return btn;
}