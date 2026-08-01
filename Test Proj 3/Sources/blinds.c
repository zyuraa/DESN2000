#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "stdio.h"


// Light threshold value (may need to adjust depending on tests (for now it is halfway between 0 --> 4096) 
#define LIGHT_THRESHOLD 					2048
#define I2C_EXPANDER_ADDR_W      	0x70  
#define IO_EXPANDER_OUTPUT_REG   	0x01
#define IO_EXPANDER_CONFIG_REG   	0x03

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
  ADC1->CR2 |= ADC_CR2_ADON //| ADC_CR2_SWSTART;

  // Wait until conversion is complete (EOC flag in SR)
  while ((ADC1->SR & ADC_SR_EOC) == 0);

  // Extract and return 12-bit result from the DR register (Reading the DR register clears EOC)
  return (int)(ADC1->DR & 0xFFF);
}

void gpio_f_init(void) {
  // Enable Port F Clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
  
  // Clear Config for Pins 0 to 7
  GPIOF->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
  GPIOF->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
  GPIOF->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
  GPIOF->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
  GPIOF->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
  GPIOF->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
  GPIOF->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
  GPIOF->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    
  // Set Output General Purpose (Push-Pull, 2MHz or 50MHz - mode bits set to 10 for 2MHz output)
  GPIOF->CRL |= GPIO_CRL_MODE0_1;
  GPIOF->CRL |= GPIO_CRL_MODE1_1;
  GPIOF->CRL |= GPIO_CRL_MODE2_1;
  GPIOF->CRL |= GPIO_CRL_MODE3_1;
  GPIOF->CRL |= GPIO_CRL_MODE4_1;
  GPIOF->CRL |= GPIO_CRL_MODE5_1;
  GPIOF->CRL |= GPIO_CRL_MODE6_1;
  GPIOF->CRL |= GPIO_CRL_MODE7_1;
}


void blinds_init(void){
	adc_init();
	i2c1_init();
}



// Main function to activate blinds -> Take output from ADC and turn ladder lights RED (blinds open) or GREEN (blinds closed)
void update_blinds(void) {
	// int light_value = 0;

	int light_value = adc_read();
	
	if (light_value > LIGHT_THRESHOLD) {
      // Blinds open: Red lights on (Pins 6-7), Green lights off (Pins 0-3)
      // Set bits 6 and 7 high
      GPIOF->BSRR = (1 << 6) | (1 << 7);
      // Reset bits 0, 1, 2, 3 low (using upper 16 bits of BSRR for atomic reset)
        GPIOF->BSRR = (1 << (0 + 16)) | (1 << (1 + 16)) | (1 << (2 + 16)) | (1 << (3 + 16));
  } else {
      // Blinds closing: Green lights on (Pins 0-3), Red lights off (Pins 6-7)
      // Set bits 0, 1, 2, 3 high
      GPIOF->BSRR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
      // Reset bits 6 and 7 low
      GPIOF->BSRR = (1 << (6 + 16)) | (1 << (7 + 16));
  }
}





