#ifndef TOUCH_H
#define TOUCH_H

void spi_init(void);

unsigned char touch_read(unsigned char command);

int touch_read_xy(unsigned short *x, unsigned short *y);

unsigned short map_pixel(unsigned short x, short in_min, short in_max, short out_min, short out_max);

int touch_rect(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned short x_num, unsigned short y_num);

#endif
