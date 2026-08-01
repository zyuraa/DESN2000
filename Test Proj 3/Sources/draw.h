#ifndef DRAW_H
#define DRAW_H

void draw_home(void);

void draw_arrow(void);

void draw_plus_minus(unsigned short mid_x, unsigned short mid_y);

void draw_alarms(void);

void draw_songs(void);

// list of songs
int songs_row_hit(unsigned short x_num, unsigned short y_num);

// now playing banner
void draw_now_playing(const char *name);

void draw_lights(void);

void draw_coffee_state(int coffee_state);

void draw_real_time(char *time_str, char *date_str);

#endif
