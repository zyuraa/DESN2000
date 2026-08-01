#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"
#include "time.h"
#include "string.h"

#include "rtc.h"
#include "touch.h"
#include "draw.h"
#include "songs.h"
#include "lights.h"

extern struct tone song_data[];
extern int song_duration;

void get_real_time(char *time_str, char *date_str) {
	unsigned int real_time;
	RTC->CRL &= ~RTC_CRL_RSF;
	while (!(RTC->CRL & RTC_CRL_RSF));
	real_time = ((unsigned int)RTC->CNTH << 16) | (unsigned int)(RTC->CNTL & 0xFFFF);
	epoch_to_strings(real_time, time_str, date_str);
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
// Coffee Machine status is LED0 GPIOF0

void change_minute(int *alarm_minute, int change) {
	if (change == 1) {
		*alarm_minute += 5;
		if (*alarm_minute > 55) {
			*alarm_minute = 55;
		}
	} else {
		*alarm_minute -= 5;
		if (*alarm_minute < 0) {
			*alarm_minute = 0;
		}
	}
}

void change_hour(int *alarm_hour, int change) {
	if (change == 1) {
		*alarm_hour += 1;
		if (*alarm_hour > 23) {
			*alarm_hour = 23;
		}
	} else {
		*alarm_hour -= 1;
		if (*alarm_hour < 0) {
			*alarm_hour = 0;
		}
	}
}

void draw_alarm_time(int hour, int minute, int x, int y) {
	char alarm_time[6];
	snprintf(alarm_time, 6, "%02d:%02d", hour, minute);

	lcd_fontColor(BLACK, DARK_GRAY);
	lcd_putString_15x21(x, y, alarm_time);
	lcd_fontColor(BLACK, LIGHT_GRAY);
}

int check_time(char *time_str, int hour, int minute) {
	char alarm_time[6];
	snprintf(alarm_time, 6, "%02d:%02d", hour, minute);

	return !strcmp(time_str, alarm_time);
}

// Alcohol Alarm assumes less than 24 standards
int main() {
	volatile int i;
	int page = 0;
	int prev_page = 0;
	int coffee_state = 0;
	int rgb_on = 0;
	int light = 0;
	int alarm_hour = 0;
	int alarm_minute = 0;
	int alarm_enable = 0;

	int alcohol_hour = 0;
	int alcohol_minute = 0;
	int alcohol_enable = 0;
	//int alcohol_offset = 0;
	int new_alcohol_hour;
	int hours_remaining;

	int current_hour = 0;
	int current_minute = 0;

	char time_str[6];
	char date_str[6];
	
	unsigned short x_num = 0;
	unsigned short y_num = 0;

	uint16_t time;
	uint16_t rgb_prev_time = 0;
	int step = 0;

	rtc_init();
	
	//rtc_set_time(1785030240);
	
	lcd_init();
	spi_init();
	//dac_init();
	rgb_init();
	led_init();
	delay_init();
	draw_home();
	lcd_putString(172, 52, "Off");
	

	while(1) {
		if (touch_read_xy(&x_num, &y_num)) {
			x_num = map_pixel(x_num, 10, 234, 0, 239);
			y_num = map_pixel(y_num, 10, 242, 0, 319);
			
			if (page == 0) { // Home Page
				// Espresso, Alarms, Songs, Lights
				if (touch_rect(140, 20, 220, 75, x_num, y_num)) {
					// Toggle coffee machine
					coffee_state ^= 1;
					
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
			} else if (page == 1) { // Alarms App
				// Alarm
				if (detect_plus_minus(35, 180, x_num, y_num) == 1) {
					// Increment Hour
					change_hour(&alarm_hour, 1);
				} else if (detect_plus_minus(35, 180, x_num, y_num) == 2) {
					// Decrement Hour
					change_hour(&alarm_hour, 2);
				} else if (detect_plus_minus(90, 180, x_num, y_num) == 1) {
					// Increment Minute by 5
					change_minute(&alarm_minute, 1);
				} else if (detect_plus_minus(90, 180, x_num, y_num) == 2) {
					// Decrement Minute by 5
					change_minute(&alarm_minute, 2);
				} else if (touch_rect(15, 220, 110, 250, x_num, y_num)) {
					// Toggle Alarm Enable
					alarm_enable ^= 1;
				}

				// Alcohol Alarm
				if (detect_plus_minus(177, 180, x_num, y_num) == 1) {
					alcohol_enable = 1;
					alcohol_hour = (alcohol_hour + 1) % 24;

					// Testing since dont want to wait 1 hr
					// alcohol_minute += 1;

				} else if (detect_plus_minus(177, 180, x_num, y_num) == 2) {
					if (alcohol_enable) {
						new_alcohol_hour = (alcohol_hour - 1 + 24) % 24;

						// Check remaining hours between new alarm time and current time 
						hours_remaining = (new_alcohol_hour - current_hour + 24) % 24;

						// If subtracting drops to 0 or less, disable 
						if (hours_remaining == 0 || hours_remaining > 22) {
							alcohol_enable = 0;
						} else {
							alcohol_hour = new_alcohol_hour;
						}
					}
				}

			} else if (page == 2) { // Songs App


			} else if (page == 3) { // Lights App
				if (touch_rect(40, 30, 200, 55, x_num, y_num)) {
					// Toggle RGB
					rgb_on ^= 1;
					
				} else if (touch_rect(25, 90, 100, 115, x_num, y_num)) {
					// Set Red
					light = 0;
					
				} else if (touch_rect(25, 125, 100, 150, x_num, y_num)) {
					// Set Yellow
					light = 1;
					
				} else if (touch_rect(25, 160, 100, 185, x_num, y_num)) {
					// Set Green
					light = 2;
					
				} else if (touch_rect(25, 195, 100, 220, x_num, y_num)) {
					// Set Blue
					light = 3;
					
				} else if (touch_rect(25, 230, 100, 255, x_num, y_num)) {
					// Set Purple
					light = 4;
					
				} else if (touch_rect(25, 265, 100, 290, x_num, y_num)) {
					// Set White
					light = 5;
					
				}
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

		// RTC time
		get_real_time(time_str, date_str);
		sscanf(time_str, "%d:%d", &current_hour, &current_minute);

		// Alcohol Alarm time is RTC when disabled
		if (alcohol_enable) {
			// Check if alarm setting matches current time
			if (check_time(time_str, alcohol_hour, alcohol_minute)) {
				// Play alarm, disable alcohol alarm
				GPIOF->BSRR = GPIO_BSRR_BS6;
				alcohol_enable = 0;
			}
		} else {
			//sscanf(time_str, "%d:%d", &alcohol_hour, &alcohol_minute);
			alcohol_hour = current_hour;
			alcohol_minute = current_minute;
		}
		

		if (alarm_enable) {
			// Check if alarm setting matches current time
			if (check_time(time_str, alarm_hour, alarm_minute)) {
				// Play alarm, disable alarm
				GPIOF->BSRR = GPIO_BSRR_BS7;
				alarm_enable = 0;
			}
		}
		
		// Draw Real time and position if we are on home page
		if (page == 0) {
			draw_real_time(time_str, date_str);
			draw_coffee_state(coffee_state);
			//draw_xy(); for touch debug
		} else if (page == 1) {
			draw_alarm_time(alarm_hour, alarm_minute, 20, 100);
			draw_alarm_time(alcohol_hour, alcohol_minute, 135, 100);

			lcd_fillRect(15, 220, 110, 250, DARK_GRAY);

			if (alarm_enable) {
				lcd_fontColor(BLACK, DARK_GRAY);
				lcd_putString(45, 234, "Enable");
				lcd_fontColor(BLACK, LIGHT_GRAY);
			} else {
				lcd_fontColor(BLACK, DARK_GRAY);
				lcd_putString(43, 234, "Disable");
				lcd_fontColor(BLACK, LIGHT_GRAY);
			}
		} else if (page == 2) {

		} else if (page == 3) {
			if (rgb_on) {
				rgb_set_colour(light, light);
			} else {
				rgb_off();
			}
		}
	
		prev_page = page;

		// More Lights
		/*
		time = (uint16_t)TIM2->CNT;
		if ((uint16_t)(time - rgb_prev_time) >= 1000) {
			rgb_prev_time = time;
			//step = rgb(step);
			step = rgb_cycle(step);
		}*/

		for (i = 0; i < 500000; i++);
	}
	

	
	
	
	
	
	return 0;
}
