#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"
#include "time.h"

#include "rtc.h"
#include "touch.h"
#include "draw.h"
#include "songs.h"

extern struct tone song_data[];
extern int song_duration;

void draw_real_time() {
	unsigned int real_time;
	char time_str[10];
	char date_str[8];
	
	RTC->CRL &= ~RTC_CRL_RSF;
	while (!(RTC->CRL & RTC_CRL_RSF));
	real_time = ((unsigned int)RTC->CNTH << 16) | (unsigned int)(RTC->CNTL & 0xFFFF);
	epoch_to_strings(real_time, time_str, date_str);
	
	lcd_putString(20, 120, (unsigned char *)time_str);
	lcd_putString(20, 140, (unsigned char *)date_str);
}

void draw_xy() {
	char x_str[8];
	char y_str[8];
	unsigned short x_num = 0;
	unsigned short y_num = 0;
	
	//char x_tmp[8];
	//char y_tmp[8];
	
	if (touch_read_xy(&x_num, &y_num)) {
		//sprintf(x_tmp, "%4u", x_num);
		//sprintf(y_tmp, "%4u", y_num);
		// Min is (10, 10), max is (234, 242)
		x_num = map_pixel(x_num, 10, 234, 0, 239);
		y_num = map_pixel(y_num, 10, 242, 0, 319);
		sprintf(x_str, "%4u", x_num);
		sprintf(y_str, "%4u", y_num);
	} else {
		//sprintf(x_tmp, "----");
		//sprintf(y_tmp, "----");

		sprintf(x_str, "----");
		sprintf(y_str, "----");
	}
	
	//lcd_putString(20, 200, (unsigned char *)x_tmp);
	//lcd_putString(20, 220, (unsigned char *)y_tmp);
	
	lcd_putString(20, 250, (unsigned char *)x_str);
	lcd_putString(20, 270, (unsigned char *)y_str);
}

/*
void dac_init() {
	// Enable clock for DAC
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	// Enable GPIO Port A for PA4
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Reset and configure PA4 as analog input 00 00
	GPIOA->CRL &= ~GPIO_CRL_MODE4;
	
	// Enable Channel 1, leave B0FF1 at 0 so the output buffer stays on
	DAC->CR |= DAC_CR_EN1;
	
	// Enable Timer 2 which will be used for the speaker
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void udelay(unsigned int delay_in_us) {
	if (delay_in_us == 0) return;

	// 1. Set prescaler for 1 MHz timer clock (1 tick = 1 us)
	// Adjust '72' to match your APB1 timer clock in MHz (e.g., 36 for 36MHz)
	TIM2->PSC = 72 - 1; 

	// 2. Reset the counter and re-initialize shadow registers
	TIM2->CNT = 0;
	TIM2->EGR = TIM_EGR_UG;  // Generate Update Event to load prescaler immediately
	TIM2->SR &= ~TIM_SR_UIF; // Clear update flag caused by UG

	// 3. Enable Timer (CEN bit)
	TIM2->CR1 |= TIM_CR1_CEN;

	// 4. Wait until the counter reaches delay_in_us
	while (TIM2->CNT < delay_in_us);

	// 5. Disable Timer
	TIM2->CR1 &= ~TIM_CR1_CEN;
}

void play_tone(unsigned int duration, int period, int vol) {
	int i;
	for (i = 0; i < duration / period; i++) {
		DAC->DHR12R1 = (vol << 6);
		udelay(period / 2);
		
		DAC->DHR12R1 = 0;
		udelay(period / 2);
	}
}

void play_song() {
	int i = 0;
	while (i <= song_duration) {
		play_tone(52000 * song_data[i].duration, song_data[i].pitch, song_data[i].volume);
		i++;
	}
}*/

// Chars are 6x8 pixels

int main() {
	volatile int i;
	int page = 0;
	int prev_page = 0;
	int coffee_state = 0;
	
	unsigned short x_num = 0;
	unsigned short y_num = 0;

	rtc_init();
	
	//rtc_set_time(1785030240);
	
	lcd_init();
	spi_init();
	//dac_init();
	draw_home();
	lcd_putString(172, 52, "Off");
	
	
	while(1) {
		if (touch_read_xy(&x_num, &y_num)) {
			x_num = map_pixel(x_num, 10, 234, 0, 239);
			y_num = map_pixel(y_num, 10, 242, 0, 319);
			
			if (page == 0) {
				// Espresso, Alarms, Songs, Lights
				if (touch_rect(140, 20, 220, 75, x_num, y_num)) {
					// Activate coffee machine
					if (coffee_state == 0) {
						coffee_state = 1;
					} else {
						coffee_state = 0;
					}
					
				} else if (touch_rect(140, 95, 220, 150, x_num, y_num)) {
					// Goto Alarms App
					page = 1;
					
				} else if (touch_rect(140, 170, 220, 225, x_num, y_num)) {
					// Goto Songs App
					page = 2;
					
				} else if (touch_rect(140, 245, 220, 300, x_num, y_num)) {
					// Goto Lights App
					page = 3;
					
				}
			} else if (page != 0 && touch_rect(0, 0, 30, 30, x_num, y_num)) {
				// Return Button
				page = 0;
			}
		}
		
		// Avoid refreshing whole screen as it is slow
		if (page != prev_page) {
			if (page == 1) {
				draw_alarms();
			} else if (page == 2) {
				draw_songs();
				//play_song();
			} else if (page == 3) {
				draw_lights();
			} else {
				draw_home();
					
				draw_coffee_state(coffee_state);
			}
		}
		
		// Draw Real time and position if we are on home page
		if (page == 0) {
			draw_real_time();
			
			draw_coffee_state(coffee_state);
			//draw_xy(); for touch debug
		}
	
		prev_page = page;

		for (i = 0; i < 500000; i++);
	}
	

	
	
	
	
	
	return 0;
}
