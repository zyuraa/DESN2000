#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "stdio.h"


// Light threshold value (may need to adjust depending on tests (for now it is halfway between 0 --> 4096) 
#define LIGHT_THRESHOLD 					2048

// Define 2 States
typedef enum {
    STATE_CLOSED = 0,
    STATE_OPEN
} blind_state_t;

void adc_init(void) {
	
	// Init ADC clock 
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		
	// Configure PA1 as Analog Input (CNF = 00, MODE = 00) - PA1 uses CRL (pins 0-7)
  GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
	
	// Set ADC Prescaler (PCLK2 divided by 6 via RCC_CFGR ADCPRE bits)
  RCC->CFGR &= ~RCC_CFGR_ADCPRE;
  RCC->CFGR |= RCC_CFGR_ADCPRE_1; // Example PCLK2 / 6
	
	// Turn on the ADC and then calibrate it
	ADC1->CR2 |= ADC_CR2_ADON;
	
	// Brief delay for stabilization, then calibrate the ADC
  ADC1->CR2 |= ADC_CR2_RSTCAL;
  while (ADC1->CR2 & ADC_CR2_RSTCAL); // Wait for reset calibration to finish
   
  ADC1->CR2 |= ADC_CR2_CAL;
  while (ADC1->CR2 & ADC_CR2_CAL);    // Wait for calibration to finish

	// Set the sampling rate of the ADC on channel 1 using ADC_SMPR2 
  ADC1->SMPR2 |= ADC_SMPR2_SMP2_1 | ADC_SMPR2_SMP2_0;  
		
	// Set Data Alignment to Right-aligned
  ADC1->CR2 &= ~ADC_CR2_ALIGN;

  // 6. Set regular sequence length (L = 0000 for 1 conversion)
  ADC1->SQR1 &= ~ADC_SQR1_L;

  // 7. Assign Channel 1 as the 1st conversion in the regular sequence (SQ1)
  ADC1->SQR3 &= ~ADC_SQR3_SQ2;
  ADC1->SQR3 |= (2 << 0);
}

int adc_read(void) {
	// Turn on ADC / start conversion
  ADC1->CR2 |= ADC_CR2_ADON; //| ADC_CR2_SWSTART;

  // Wait until conversion is complete (EOC flag in SR)
  while ((ADC1->SR & ADC_SR_EOC) == 0);

  // Extract and return 12-bit result from the DR register (Reading the DR register clears EOC)
  return (int)(ADC1->DR & 0xFFF);
}

void gpio_f_init(void) {
  // Enable Port F Clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
  
  // Clear Config for Pins 1 to 7
  GPIOF->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
  GPIOF->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
  GPIOF->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
  GPIOF->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
  GPIOF->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
  GPIOF->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
  GPIOF->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    
  // Set Output General Purpose (Push-Pull, 2MHz or 50MHz - mode bits set to 10 for 2MHz output)
  GPIOF->CRL |= GPIO_CRL_MODE1_1;
  GPIOF->CRL |= GPIO_CRL_MODE2_1;
  GPIOF->CRL |= GPIO_CRL_MODE3_1;
  GPIOF->CRL |= GPIO_CRL_MODE4_1;
  GPIOF->CRL |= GPIO_CRL_MODE5_1;
  GPIOF->CRL |= GPIO_CRL_MODE6_1;
  GPIOF->CRL |= GPIO_CRL_MODE7_1;

  // Default to closed: Green lights on (Pins 1-3)
  GPIOF->BSRR = GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3;
}


void blinds_init(void){
	adc_init();
	gpio_f_init();
}



// Main function to activate blinds -> Take output from ADC and turn ladder lights RED (blinds open) or GREEN (blinds closed)
void update_blinds(void) {

  static int open_timer = 0;
  static int close_timer = 0;
  static blind_state_t current_state = STATE_CLOSED;

  int light = adc_read();

  // Evaluate light level and update timers
  if (light >= LIGHT_THRESHOLD) {
    open_timer++;
    close_timer = 0;
  } else {
    close_timer++;
    open_timer = 0;
  }
	
	// State Transition Logic based on Timer Thresholds (e.g., 10 consecutive ticks)
  if (current_state != STATE_OPEN && open_timer >= 10) {
    current_state = STATE_OPEN;
      
    // Action: Open Blinds -> Red lights on (Pins 6-7), Green lights off (Pins 1-3)
    GPIOF->BSRR = GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
    GPIOF->BSRR = GPIO_BSRR_BS6 | GPIO_BSRR_BS7;
  } 
  else if (current_state != STATE_CLOSED && close_timer >= 10) {
    current_state = STATE_CLOSED;
      
    // Action: Close Blinds -> Green lights on (Pins 1-3), Red lights off (Pins 6-7)
    GPIOF->BSRR = GPIO_BSRR_BR6 | GPIO_BSRR_BR7;
    GPIOF->BSRR = GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3;
  }
}





