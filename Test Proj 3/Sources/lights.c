#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"

void rgb_init() {
	// RGB 0 GPIOA [8..10]
	// RGB 1 GPIOE [2..4]

	// Set Ports A, E as active
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	// Clear CRH Config
	GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
	GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
	
	GPIOE->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
	GPIOE->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
	GPIOE->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
	
	
	// Set GPIOA pins 8-10 as output 2MHz, corresponds to RGB0
	GPIOA->CRH |= GPIO_CRH_MODE8_1;
	GPIOA->CRH |= GPIO_CRH_MODE9_1;
	GPIOA->CRH |= GPIO_CRH_MODE10_1;
	
	// Set GPIOE pins 2-4 as output 2MHz, corresponds to RGB1
	GPIOE->CRL |= GPIO_CRL_MODE2_1;
	GPIOE->CRL |= GPIO_CRL_MODE3_1;
	GPIOE->CRL |= GPIO_CRL_MODE4_1;
}

void led_init() {
	// Enable Port F
	RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
	
	// Clear Config
	GPIOF->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
	GPIOF->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
	GPIOF->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
	GPIOF->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
	GPIOF->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
	GPIOF->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
	GPIOF->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
	GPIOF->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
	
	// Set Output general purpose
	GPIOF->CRL |= GPIO_CRL_MODE0_1;
	GPIOF->CRL |= GPIO_CRL_MODE1_1;
	GPIOF->CRL |= GPIO_CRL_MODE2_1;
	GPIOF->CRL |= GPIO_CRL_MODE3_1;
	GPIOF->CRL |= GPIO_CRL_MODE4_1;
	GPIOF->CRL |= GPIO_CRL_MODE5_1;
	GPIOF->CRL |= GPIO_CRL_MODE6_1;
	GPIOF->CRL |= GPIO_CRL_MODE7_1;
}

void delay_init() {
	// Enable Timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	// Set prescaler and Auto reload register to count in 1us to 1000us = 1ms
	TIM2->PSC = 8000 - 1;
	TIM2->ARR = 0xFFFF;
	
	// Force update to initialise registers
	TIM2->EGR |= TIM_EGR_UG;
	
	// Start timer and keep it running forever
	TIM2->CR1 |= TIM_CR1_CEN;
}


void rgb_off() {
	// GPIOA 8..10, GPIOE 2..4 Reset
	GPIOA->BSRR = GPIO_BSRR_BR8;
	GPIOA->BSRR = GPIO_BSRR_BR9;
	GPIOA->BSRR = GPIO_BSRR_BR10;
	
	GPIOE->BSRR = GPIO_BSRR_BR2;
	GPIOE->BSRR = GPIO_BSRR_BR3;
	GPIOE->BSRR = GPIO_BSRR_BR4;
}

void rgb_set_0(int colour) {
	if (colour == 0) {
		GPIOA->BSRR = GPIO_BSRR_BS9;
	} else if (colour == 1) {
		GPIOA->BSRR = GPIO_BSRR_BS9;
		GPIOA->BSRR = GPIO_BSRR_BS10;
	} else if (colour == 2) {
		GPIOA->BSRR = GPIO_BSRR_BS10;
	} else if (colour == 3) {
		GPIOA->BSRR = GPIO_BSRR_BS8;
	} else if (colour == 4) {
		GPIOA->BSRR = GPIO_BSRR_BS8;
		GPIOA->BSRR = GPIO_BSRR_BS9;
	} else if (colour == 5) {
		GPIOA->BSRR = GPIO_BSRR_BS8;
		GPIOA->BSRR = GPIO_BSRR_BS9;
		GPIOA->BSRR = GPIO_BSRR_BS10;
	}
}

void rgb_set_1(int colour) {
	if (colour == 0) {
		GPIOE->BSRR = GPIO_BSRR_BS3;
	} else if (colour == 1) {
		GPIOE->BSRR = GPIO_BSRR_BS3;
		GPIOE->BSRR = GPIO_BSRR_BS4;
	} else if (colour == 2) {
		GPIOE->BSRR = GPIO_BSRR_BS4;
	} else if (colour == 3) {
		GPIOE->BSRR = GPIO_BSRR_BS2;
	} else if (colour == 4) {
		GPIOE->BSRR = GPIO_BSRR_BS2;
		GPIOE->BSRR = GPIO_BSRR_BS3;
	} else if (colour == 5) {
		GPIOE->BSRR = GPIO_BSRR_BS2;
		GPIOE->BSRR = GPIO_BSRR_BS3;
		GPIOE->BSRR = GPIO_BSRR_BS4;
	}
}


void rgb_set_colour(int colour0, int colour1) {
	rgb_off();
	rgb_set_0(colour0);
	rgb_set_1(colour1);
}

//void rgb_breathe(int colour) {

//}

//void rgb_static(int colour) {
	//rgb_set_colour(colour, colour);
//}


int rgb(int step) {
	rgb_set_colour(step, step);
	return (step + 1) % 6;
}

int rgb_cycle(int step) {
	int step0 = (step + 1) % 6;
	rgb_set_colour(step0, step);
	return (step + 1) % 6;
}
