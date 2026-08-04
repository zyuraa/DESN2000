#ifndef DOORBELL_H
#define DOORBELL_H

void button_init(void);

/*
reads status of debounced button and plays doorbell if pressed
*/
void read_doorbell(void);

#endif