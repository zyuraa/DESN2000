#ifndef LIGHTS_H
#define LIGHTS_H

void rgb_init(void);

void led_init(void);

void delay_init(void);

void rgb_set_colour(int colour0, int colour1);

void rgb_off(void);

int rgb(int step);

int rgb_cycle(int step);

#endif
