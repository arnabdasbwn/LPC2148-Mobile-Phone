// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header file is used to CONFIGURE and USE the inbuilt RTC of LPC2148
// File Name: lpc2148_rtc.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_rtc_H
#define __lpc2148_rtc_H

#include <lpc214x.h>
#include <inttypes.h>

uint8_t rtc_get_sec(void);			// Return Seconds of RTC
uint8_t rtc_get_min(void);			// Return Minutes of RTC
uint8_t rtc_get_hour(void);			// Return Hours of RTC
uint8_t rtc_get_dom(void);			// Return Day Of Month of RTC
uint8_t rtc_get_dow(void);			// Return Day Of Week of RTC
uint8_t rtc_get_doy(void);			// Return Day Of Year of RTC
uint8_t rtc_get_month(void);		// Return Month of RTC
uint16_t rtc_get_year(void);			// Return Year of RTC

void rtc_set_sec(uint8_t);			// Set Seconds of RTC
void rtc_set_min(uint8_t);			// Set Minutes of RTC
void rtc_set_hour(uint8_t);			// Set Hours of RTC
void rtc_set_dom(uint8_t);			// Set Day of Month of RTC
void rtc_set_dow(uint8_t);			// Set Day of Week of RTC
void rtc_set_doy(uint8_t);			// Set Day of Year of RTC
void rtc_set_month(uint8_t);		// Set Month of RTC
void rtc_set_year(uint16_t);		// Set Year of RTC
void rtc_set_default_time(void);		// Set Default Time of RTC

void rtc_init(void);				// Initilize RTC 
void rtc_interrupt_init(void);		// Initilize RTC Interrupt
__irq void rtc_isr(void);			// RTC ISR

// -----------------------------------------------------------------------------

uint8_t rtc_get_sec(void)
{
	return SEC;
}
uint8_t rtc_get_min(void)
{
	return MIN;
}
uint8_t rtc_get_hour(void)
{
	return HOUR;
}
uint8_t rtc_get_dom(void)
{
	return DOM;
}
uint8_t rtc_get_dow(void)
{
	return DOW;
}
uint8_t rtc_get_doy(void)
{
	return DOY;
}
uint8_t rtc_get_month(void)
{
	return MONTH;
}
uint16_t rtc_get_year(void)
{
	return YEAR;
}

// -----------------------------------------------------------------------------

void rtc_set_sec(uint8_t sec)
{
	SEC = sec;
}
void rtc_set_min(uint8_t min)
{
	MIN = min;
}
void rtc_set_hour(uint8_t hour)
{
	HOUR = hour;
}
void rtc_set_dom(uint8_t dom)
{
	DOM = dom;
}
void rtc_set_dow(uint8_t dow)
{
	DOW = dow;
}
void rtc_set_doy(uint8_t doy)
{
	DOY = doy;
}
void rtc_set_month(uint8_t month)
{
	MONTH = month;
}
void rtc_set_year(uint16_t year)
{
	YEAR = year;
}

// -----------------------------------------------------------------------------

__irq void rtc_isr(void)
{
	//WRITE ISR TASK
	
	ILR |= 1;            // Clear interrupt flag
	VICVectAddr = 0;     // Acknowledge Interrupt
	PCON = 1;            // IDLE mode
}
void rtc_init(void)
{
   ILR = 3;        // Disable 32768 Interrupt and Alarm Interrupt
   CCR = 0x11;     // Clock enable + 32.767 KHz quartz enable
}
void rtc_interrupt_init(void)
{
	VICIntEnClr = 0xFFFFFFFF;           	// clear all interrupts
	VICIntSelect = 0x00000000;          	// clear all FIQ selections
    CIIR = 0x01;    						// Interupt every second
    VICVectAddr1 = (unsigned long)rtc_isr;  // set interrupt vector in 1
    VICVectCntl1 = 0x0000002D;     			// use it for RTC Interrupt
    VICIntEnable = 0x00002000;     			// Enable RTC Interrupt
}
void rtc_set_default_time(void) // Setet the DEFAULT TIME i.e. 13/07/2016  5:40:00am  Wednesday IST 
{
   YEAR = 2016;  // Year
   MONTH = 12;    // Month
   DOM = 23;     // Day of month
   //DOY = 38;   // Day of year
   DOW = 7;      // Day of week
   HOUR = 23;     // Hours
   MIN = 52;      // Minutes
   SEC = 30;     // Seconds
}
#endif
