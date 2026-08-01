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

// Low-level I2C write function
void i2c_expander_write(uint8_t reg_addr, uint8_t data) {
    
	// Generate START condition
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB));   // Wait for Start Bit (SB) flag to set

  // 2. Send Device Write Address
  I2C1->DR = I2C_EXPANDER_ADDR_W;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Wait for Address sent (ADDR) flag
    
  // Clear ADDR flag by reading SR1 followed by SR2
  volatile uint32_t dummy = I2C1->SR1;
  dummy = I2C1->SR2;

  // 3. Send Register Address / Command byte to target inside expander
  I2C1->DR = reg_addr;
  while (!(I2C1->SR1 & I2C_SR1_TXE));  // Wait until Data Register Empty (TXE)

  // 4. Send the Data Byte
  I2C1->DR = data;
  while (!(I2C1->SR1 & I2C_SR1_TXE));  
  while (!(I2C1->SR1 & I2C_SR1_BTF));  // Wait for Byte Transfer Finished (BTF)

  // 5. Generate STOP condition
  I2C1->CR1 |= I2C_CR1_STOP;
}


void i2c1_init(void) {
  // Enable clocks for I2C1 and GPIOB
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  
  // Configure PB6 (SCL) and PB7 (SDA) as Alternate Function Open-Drain (MODE = 11, CNF = 11)
  GPIOB->CRL &= ~((0xF << (4 * 6)) | (0xF << (4 * 7))); 
  GPIOB->CRL |=  ((0xF << (4 * 6)) | (0xF << (4 * 7))); 

  // Configure I2C peripheral speed and timings (assuming 36MHz APB1 clock / PCLK1)
  I2C1->CR2 = 36;                     // Set peripheral frequency in MHz (36 MHz)
  I2C1->CCR = 180;                    // Standard mode 100kHz: 36,000,000 / (2 * 100,000) = 180
  I2C1->TRISE = 37;                   // Maximum rise time: 36 + 1 = 37

  // Enable I2C1 Peripheral
  I2C1->CR1 |= I2C_CR1_PE;

  // Configure I/O expander pins as outputs (0 = Output, 1 = Input)
  // We want pins 0-3 (Green) and pins 6-7 (Red) set as outputs. 
  // Binary: 00111111 (Hex: 0x3F) -> Sets IO0-IO5 as outputs, IO6-IO7 as inputs? 
  // Wait: bits 0,1,2,3 (Green) and bits 6,7 (Red) -> binary 00110111 = 0x37 or 0x0F depending on exact mapping.
  // To be safe, set all lower 8 pins (0 to 7) as outputs by writing 0x00 to the configuration register:
  i2c_expander_write(IO_EXPANDER_CONFIG_REG, 0x00);
}


void blinds_init(void){
	adc_init();
	i2c1_init();
}



// Main function to activate blinds -> Take output from ADC and turn ladder lights RED (blinds open) or GREEN (blinds closed)
void update_blinds(void) {
	//adc_init();
	//i2c1_init();
		
	int light_value = 0;

	light_value = adc_read();
	
	if (light_value > LIGHT_THRESHOLD) {
		// Blinds open: Red LEDs on (bits 6-7), Green off (bits 0-3)
    i2c_expander_write(IO_EXPANDER_OUTPUT_REG, 0xC0);
	// blinds are closed 	
	} else {
		// Blinds closing: Green LEDs on (bits 0-3), Red off (bits 6-7)
    i2c_expander_write(IO_EXPANDER_OUTPUT_REG, 0x0F);
	}
}





