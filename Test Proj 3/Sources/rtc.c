#include "../Headers/stm32f103xe.h"
#include "../LCD/ili9341.h"
#include "../LCD/lcd_hw.h"
#include "../LCD/lcd_grph.h"
#include "stdio.h"
#include "time.h"

void rtc_set_time(unsigned int epoch_seconds) {
    // 1. Wait until previous write operation finishes
    while (!(RTC->CRL & RTC_CRL_RTOFF));

    // 2. Enter Configuration Mode
    RTC->CRL |= RTC_CRL_CNF;

    // 3. Write upper 16 bits and lower 16 bits
    RTC->CNTH = (unsigned short)(epoch_seconds >> 16);
    RTC->CNTL = (unsigned short)(epoch_seconds & 0xFFFF);

    // 4. Exit Configuration Mode
    RTC->CRL &= ~RTC_CRL_CNF;

    // 5. Wait for write completion
    while (!(RTC->CRL & RTC_CRL_RTOFF));
}

void rtc_init() {
	// Enable Power and Backup interface clocks
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
	
	// Enable access to the backup registers and RTC
	PWR->CR |= PWR_CR_DBP;
	
	// Check if its been configured before (preserve across soft resets)
	if (BKP->DR1 != 0xA5A5)
	{
		// Enable Low speed external oscillator 32.768kHz, wait until ready
		RCC->BDCR |= RCC_BDCR_LSEON;
		while (!(RCC->BDCR & RCC_BDCR_LSERDY));
		
		// Clear previous selection and select LSE as source
		RCC->BDCR &= ~RCC_BDCR_RTCSEL;
		RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
		
		// RTC clock enable
		RCC->BDCR |= RCC_BDCR_RTCEN;
		
		// Clear register synchroniser flag and wait for it to synchronise
		RTC->CRL &= ~RTC_CRL_RSF;
    while (!(RTC->CRL & RTC_CRL_RSF));
		
		// Wait for all pending operations to finish
		while(!(RTC->CRL & RTC_CRL_RTOFF));
		
		// Enter configuration mode
		RTC->CRL |= RTC_CRL_CNF;
		
		// Set prescaler to 1 second. High stores top 4 bits and Low stores 16 bits for total 20 bits
		RTC->PRLH = (unsigned short)0x0000;
		RTC->PRLL = (unsigned short)0x7FFF;
		
		// Set time. High stores top 4 bits and low stores 16 bits for total 20 bits
		//RTC->CNTH = (unsigned short)(1785034140UL >> 16);
		//RTC->CNTL = (unsigned short)(1785034140UL & 0xFFFF);
		
		// Exit configuration mode and wait for processes
		RTC->CRL &= ~RTC_CRL_CNF;
		while (!(RTC->CRL & RTC_CRL_RTOFF));
		
		// Set time
		rtc_set_time(1785034140);
		
		// Set magic value to mark as initialised
		BKP->DR1 = 0xA5A5;
	}
	else
	{
		// Clear register synchroniser flag and wait for it to synchronise
		RTC->CRL &= ~RTC_CRL_RSF;
		while (!(RTC->CRL & RTC_CRL_RSF));
		
		// Wait for all pending operations to finish
		while(!(RTC->CRL & RTC_CRL_RTOFF));
		
	}
}

void epoch_to_strings(unsigned int epoch, char *time_out, char *date_out) {
	unsigned int seconds_in_day;
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
	unsigned int days;
	unsigned int era;
	unsigned int doe;
	unsigned int yoe;
	unsigned int y;
	unsigned int doy;
	unsigned int mp;
	unsigned int day;
	unsigned int month;

	// Executable statements start here
	seconds_in_day = epoch % 86400;
	hours   = seconds_in_day / 3600;
	minutes = (seconds_in_day % 3600) / 60;
	seconds = seconds_in_day % 60;

	days = epoch / 86400;

	// Civil calendar calculation
	days += 719468;
	era = days / 146097;
	doe = days - era * 146097;
	yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
	y = yoe + era * 400;
	doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
	mp = (5 * doy + 2) / 153;
	
	day = doy - (153 * mp + 2) / 5 + 1;
	month = (mp < 10) ? (mp + 3) : (mp - 9);

	//sprintf(time_out, "%02u:%02u:%02u", hours, minutes, seconds);
	sprintf(time_out, "%02u:%02u", hours, minutes);
	sprintf(date_out, "%02u/%02u", day, month);
}