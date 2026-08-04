#ifndef LIGHTS_H
#define LIGHTS_H

void lights_init(void);

void rgb_set_colour(int colour0, int colour1);

void rgb_off(void);

void rgb_dynamic_run(int rgb_dynamic_type, int colour);

void update_rgb(int rgb_on, int rgb_dynamic_type, int colour);

int rgb(int step);

int rgb_cycle(int step);

#endif
