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
	songs_init();
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
			} else if (page == 2) {
				// play 15s song, attempt to exit too
				int song_hit = songs_row_hit(x_num, y_num);
				if (song_hit >= 0) {
					draw_now_playing(song_names[song_hit]);
					play_song(song_hit);
					draw_songs();
				} else if (touch_rect(0, 0, 30, 30, x_num, y_num)) {
					page = 0;
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
