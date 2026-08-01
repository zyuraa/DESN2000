#ifndef BLINDS_H
#define BLINDS_H

void adc_init(void);
int adc_read(void);
void i2c1_init(void);
void blinds_init(void);
void update_blinds(void);

#endif
