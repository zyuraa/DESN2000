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

void draw_plus_minus(unsigned short mid_x, unsigned short mid_y) {
   // Drawing Plus
   lcd_line(mid_x - 17, mid_y, mid_x - 7, mid_y, BLACK);
   lcd_line(mid_x - 12, mid_y - 5, mid_x - 12, mid_y + 5, BLACK);

   // Drawing Minus
   lcd_line(mid_x + 7, mid_y, mid_x + 17, mid_y, BLACK);

   lcd_drawRect(mid_x - 20, mid_y - 8, mid_x + 20, mid_y + 8, BLACK);
}


void draw_alarms() {
	lcd_fillScreen(DARK_GRAY);
	lcd_putString(102, 10, "Alarms");
	lcd_fillRect(10, 25, 115, 310, LIGHT_GRAY);
	lcd_fillRect(125, 25, 230, 310, LIGHT_GRAY);

	// Alarm
	lcd_putString(40, 35, "Set Alarm");
	lcd_fillRect(15, 70, 110, 150, DARK_GRAY);
	draw_plus_minus(35, 180);
	draw_plus_minus(90, 180);

	// Alcohol Alarm
	lcd_putString(140, 35, "Alcohol Alarm");
	lcd_putString(150, 50, "Disclaimer: ");
	lcd_putString(130, 60, "Not Legal Advice");
	lcd_fillRect(130, 70, 225, 150, DARK_GRAY);
	lcd_putString(135, 160, "Enter Standards");
	draw_plus_minus(177, 180);

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
	
	// Button
	lcd_fillRect(40, 30, 200, 55, LIGHT_GRAY);
	lcd_putString(88, 38, "Activate RGB");
	
	// Background
	lcd_fillRect(10, 70, 115, 310, LIGHT_GRAY);
	lcd_fillRect(125, 70, 230, 310, LIGHT_GRAY);
	
	// Colour Select
	lcd_fillRect(25, 90, 100, 115, RED);
	lcd_fillRect(25, 125, 100, 150, YELLOW);
	lcd_fillRect(25, 160, 100, 185, GREEN);
	lcd_fillRect(25, 195, 100, 220, BLUE);
	lcd_fillRect(25, 230, 100, 255, PURPLE);
	lcd_fillRect(25, 265, 100, 290, WHITE);
	
	draw_arrow();
}

void draw_coffee_state(int coffee_state) {
	lcd_fillRect(172, 52, 200, 75, LIGHT_GRAY);
	if (coffee_state == 0) {
		lcd_putString(172, 52, "Off");
		GPIOF->BSRR = GPIO_BSRR_BR0;
	} else {
		lcd_putString(176, 52, "On");
		GPIOF->BSRR = GPIO_BSRR_BS0;
	}
}

void draw_real_time(char *time_str, char *date_str) {
	lcd_fontColor(BLACK, DARK_GRAY);
	lcd_putString_15x21(20, 100, (unsigned char *)time_str);
	lcd_putString_15x21(20, 130, (unsigned char *)date_str);
	lcd_fontColor(BLACK, LIGHT_GRAY);
}

