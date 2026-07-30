#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"

void draw_home() {
	lcd_fillScreen(DARK_GRAY);
	lcd_fillRect(140, 20, 220, 75, LIGHT_GRAY);
	lcd_fillRect(140, 95, 220, 150, LIGHT_GRAY);
	lcd_fillRect(140, 170, 220, 225, LIGHT_GRAY);
	lcd_fillRect(140, 245, 220, 300, LIGHT_GRAY);
	
	lcd_fontColor(BLACK, LIGHT_GRAY);
	lcd_putString(158, 40, "Espresso");
	lcd_putString(162, 121, "Alarms");
	lcd_putString(164, 196, "Songs");
	lcd_putString(162, 271, "Lights");
}

void draw_arrow() {
	lcd_line(10, 10, 15, 5, BLACK);
	lcd_line(11, 10, 16, 5, BLACK);
	lcd_fillRect(10, 10, 20, 11, BLACK);	
	lcd_line(10, 11, 15, 16, BLACK);
	lcd_line(11, 11, 16, 16, BLACK);
}

void draw_alarms() {
	lcd_fillScreen(DARK_GRAY);
	lcd_putString(102, 10, "Alarms");
	lcd_fillRect(10, 25, 115, 310, LIGHT_GRAY);
	lcd_fillRect(125, 25, 230, 310, LIGHT_GRAY);
	
	draw_arrow();
}

void draw_songs() {
	lcd_fillScreen(DARK_GRAY);
	lcd_putString(105, 10, "Songs");
	lcd_fillRect(10, 25, 115, 310, LIGHT_GRAY);
	lcd_fillRect(125, 25, 230, 310, LIGHT_GRAY);
	
	draw_arrow();
}

void draw_lights() {
	lcd_fillScreen(DARK_GRAY);
	lcd_putString(102, 10, "Lights");
	lcd_fillRect(10, 25, 115, 310, LIGHT_GRAY);
	lcd_fillRect(125, 25, 230, 310, LIGHT_GRAY);
	
	draw_arrow();
}

void draw_coffee_state(int coffee_state) {
	lcd_fillRect(172, 52, 200, 75, LIGHT_GRAY);
	if (coffee_state == 0) {
		lcd_putString(172, 52, "Off");
	} else {
		lcd_putString(176, 52, "On");
	}
}