#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"
#include "touch.h"
#include "songs.h"

// Width of the row for each song
static const unsigned short song_row_y0[NUM_SONGS] = {40, 96, 152, 208, 264};
static const unsigned short song_row_y1[NUM_SONGS] = {85, 141, 197, 253, 309};
#define SONG_ROW_X0 10
#define SONG_ROW_X1 230

// Spacer for the now playing
#define SONG_STATUS_Y0 22
#define SONG_STATUS_Y1 38

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
	int i;

	lcd_fillScreen(DARK_GRAY);
	lcd_putString(105, 10, "Songs");

	// In case theres a Now Playing thing
	lcd_fillRect(SONG_ROW_X0, SONG_STATUS_Y0, SONG_ROW_X1, SONG_STATUS_Y1, DARK_GRAY);

	lcd_fontColor(BLACK, LIGHT_GRAY);

	// Display the songs loaded
	for (i = 0; i < NUM_SONGS; i++) {
		lcd_fillRect(SONG_ROW_X0, song_row_y0[i], SONG_ROW_X1, song_row_y1[i], LIGHT_GRAY);
		lcd_putString(SONG_ROW_X0 + 10, song_row_y0[i] + 18, (unsigned char *)song_names[i]);
	}
	lcd_fontColor(BLACK, DARK_GRAY);

	draw_arrow();
}

int songs_row_hit(unsigned short x_num, unsigned short y_num) {
	int i;

	for (i = 0; i < NUM_SONGS; i++) {
		if (touch_rect(SONG_ROW_X0, song_row_y0[i], SONG_ROW_X1, song_row_y1[i], x_num, y_num)) {
			return i;
		}
	}

	return -1;
}

void draw_now_playing(const char *name) {
	// shows now playing before song plays bc everything freezes
	lcd_fillRect(SONG_ROW_X0, SONG_STATUS_Y0, SONG_ROW_X1, SONG_STATUS_Y1, DARK_GRAY);
	lcd_fontColor(WHITE, DARK_GRAY);
	lcd_putString(SONG_ROW_X0, SONG_STATUS_Y0, (unsigned char *)name);
	lcd_fontColor(BLACK, LIGHT_GRAY);
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
	
	// Dynamic Select
	lcd_fillRect(140, 90, 215, 132, DARK_GRAY);
	lcd_fillRect(140, 142, 215, 185, DARK_GRAY);
	lcd_fillRect(140, 195, 215, 237, DARK_GRAY);
	lcd_fillRect(140, 247, 215, 290, DARK_GRAY);
	lcd_fontColor(BLACK, DARK_GRAY);
	lcd_putString(162, 108, "Solid");
	lcd_putString(160, 160, "Strobe");
	lcd_putString(158, 213, "Cycle 1");
	lcd_putString(158, 265, "Cycle 2");
	lcd_fontColor(BLACK, LIGHT_GRAY);

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

