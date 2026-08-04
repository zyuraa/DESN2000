#ifndef RTC_H
#define RTC_H


void rtc_set_time(unsigned int epoch_seconds);

void rtc_init(void);

void epoch_to_strings(unsigned int epoch, char *time_out, char *date_out);

#endif
