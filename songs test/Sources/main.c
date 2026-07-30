#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"

#include "songs.h"

extern struct tone song_data[];
extern int song_duration;

extern struct tone song1_data[];
extern int song1_duration;

extern struct tone song2_data[];
extern int song2_duration;

/* Safe microsecond delay using TIM2 */
void udelay(unsigned int delay_in_us) {
    unsigned int chunk;

    /* Safety guard 1: Don't run timer for 0 microseconds */
    if (delay_in_us == 0) return;

    while (delay_in_us > 0) {
        /* Keep chunk under TIM2 16-bit max (65535) */
        chunk = (delay_in_us > 30000) ? 30000 : delay_in_us;

        TIM2->CNT = 0;
        TIM2->CR1 |= TIM_CR1_CEN; /* Enable timer */

        /* Safety guard 2: Hard break if CNT misses chunk or wraps around */
        while ((TIM2->CNT < chunk) && (TIM2->CNT <= 30000));

        TIM2->CR1 &= ~TIM_CR1_CEN; /* Disable timer */

        if (delay_in_us <= chunk) {
            break;
        }
        delay_in_us -= chunk;
    }
}

void dac_init(void) {
    /* 1. ENABLE CLOCKS FIRST before accessing registers */
    RCC->APB1ENR |= (RCC_APB1ENR_DACEN | RCC_APB1ENR_TIM2EN);
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. Configure PA4 as Analog Output (0000 in CRL) */
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);

    /* 3. Setup TIM2 for 1MHz counting (1 tick = 1 us) 
       Assuming 64MHz System Clock (HSI PLL x16) -> APB1 Timer clock is 64MHz */
    TIM2->CR1 = 0;
    TIM2->PSC = 64 - 1; 
    TIM2->EGR = TIM_EGR_UG;     /* Force update to load prescaler */
    TIM2->SR &= ~TIM_SR_UIF;    /* Clear update flag */

    /* 4. Enable DAC Channel 1 (Software trigger by default) */
    DAC->CR = DAC_CR_EN1;
}

void play_tone(unsigned int duration, int period, int vol) {
    int half_period;
    int cycles;
    int i;

    /* Instantly process rests/silence */
    if (vol == 0 || period == 100 || period <= 0) {
        DAC->DHR12R1 = 0;
        udelay(duration);
        return;
    }

    half_period = period / 2;
    if (half_period < 1) half_period = 1;

    cycles = duration / period;

    for (i = 0; i < cycles; i++) {
        DAC->DHR12R1 = vol; /* Output voltage to DAC PA4 */
        udelay(half_period);

        DAC->DHR12R1 = 0;
        udelay(half_period);
    }
}

void play_song(void) {
    int i = 0;
    //for (i = 0; i < song_duration; i++) {
     //   play_tone(52000 * song_data[i].duration, song_data[i].pitch, song_data[i].volume);
    //}
		//for (i = 0; i < song1_duration; i++) {
    //    play_tone(150000 * song1_data[i].duration, song1_data[i].pitch, song1_data[i].volume);
    //}
		
	for (i = 0; i < song2_duration; i++) {
		play_tone(1200 * song2_data[i].duration, song2_data[i].pitch, song2_data[i].volume);
	}
}

/* Bulletproof Clock Config using internal HSI (No external crystal required) */
void SystemClock_Config_HSI_64MHz(void) {
    /* Flash latency for 64MHz */
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    /* HSI / 2 = 4MHz. PLL mul 16 = 64 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLMULL16); /* HSI/2 is default PLLSRC */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* Select PLL as System Clock */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

int main(void) {
    SystemClock_Config_HSI_64MHz();
    dac_init();

    /* Quick Test Pulse: You should hear a short beep here at boot */
    play_tone(100000, 3831/2, 0x500); 
    udelay(100000);

    

    while (1) {
			play_song();
    }

    return 0;
}

