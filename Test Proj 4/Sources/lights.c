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
	// Enable Timer 3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	// Set prescaler and Auto reload register to count in 1us to 1000us = 1ms
	TIM3->PSC = 8000 - 1;
	TIM3->ARR = 0xFFFF;
	
	// Force update to initialise registers
	TIM3->EGR |= TIM_EGR_UG;
	
	// Start timer and keep it running forever
	TIM3->CR1 |= TIM_CR1_CEN;
}

void lights_init() {
	rgb_init();
	led_init();
	delay_init();
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

// Strobe, ie turn on and off
int rgb_strobe(int step, int colour) {
	if (step == 0) {
		rgb_off();
	} else {
		rgb_set_colour(colour, colour);
	}

	return (step + 1) % 2;
}

// Rgb cycle, both LED same colour
int rgb(int step) {
	rgb_set_colour(step, step);
	return (step + 1) % 6;
}

// Rgb Cycle Downwards
int rgb_cycle(int step) {
	int step0 = (step + 1) % 6;
	rgb_set_colour(step0, step);
	return (step + 1) % 6;
}

void rgb_dynamic_run(int rgb_dynamic_type, int colour) {
	static int step = 0;
	static unsigned short rgb_prev_time = 0;
	uint16_t time = (uint16_t)TIM3->CNT;

	if ((uint16_t)(time - rgb_prev_time) >= 1000) {
		rgb_prev_time = time;

		// Type Selection
		if (rgb_dynamic_type == 1) {
			step = rgb_strobe(step, colour);
		} else if (rgb_dynamic_type == 2) {
			step = rgb(step);
		} else if (rgb_dynamic_type == 3) {
			step = rgb_cycle(step);
		}
	}
}

void update_rgb(int rgb_on, int rgb_dynamic_type, int colour) {
	if (rgb_on) {
		if (rgb_dynamic_type == 0) {
			rgb_set_colour(colour, colour);
		} else {
			rgb_dynamic_run(rgb_dynamic_type, colour);
		}
	} else {
		rgb_off();
	}
}




