// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header file is used to get exact delay based upon TIMER0 Intrrupt
// File Name: lpc2148_timer0.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_timer0_H
#define __lpc2148_timer0_H

#include <lpc214x.h>
#include <inttypes.h>

// -----------------------------------------------------------------------------

void timer0_delay_us(uint16_t);			// Generate Delay in Micro SecondsIn multiple of 100us
void timer0_delay_ms(uint16_t);			// Generate Delay in Milli Seconds In Multiples of 1
void timer0_delay_s(uint16_t);			// Generate Delay in Seconds In multiples of 1
void timer0_delay_min(uint16_t);		// Generate Delay in Minutes In multiples of 1

uint16_t timer0_get_min(void);			// Returns Minutes System Runned
uint16_t timer0_get_s(void);			// Returns Seconds System Runned
uint16_t timer0_get_ms(void);			// Returns Milli Seconds System Runned
uint16_t timer0_get_us(void);			// Returns Micro Seconds System Runned
uint64_t timer0_get_ticks(void);		// Returns Ticks in Unit of 100us System Runned

void timer0_init(void);					// Initilize The Timer 0 and Its Interrupt

__irq void timer0_isr(void);			// Timer 0 ISR
	  
static volatile uint64_t ticks;
static volatile uint16_t s;
static volatile uint16_t pasts;
static volatile uint16_t us;
static volatile uint16_t ms;
static volatile uint16_t min;

void rtc_print_on_glcd(void);

// -----------------------------------------------------------------------------

void timer0_delay_us(uint16_t delay)
{
	uint64_t past = ticks;
	past = past + delay/100;
	while(ticks <= past);
}	
void timer0_delay_ms(uint16_t delay)
{
	uint64_t past = ticks;
	past = past + delay*10;
	while(ticks <= past);
}
void timer0_delay_s(uint16_t delay)
{
	uint64_t past = ticks;
	past = past + delay*10000;
	while(ticks <= past);
}
void timer0_delay_min(uint16_t delay)
{
	uint64_t past = ticks;
	past = past + delay*600000;
	while(ticks <= past);
}
// -----------------------------------------------------------------------------
uint16_t timer0_get_min(void)
{
	return min;
}
uint16_t timer0_get_s(void)
{
	return s;
}
uint16_t timer0_get_ms(void)
{
	return ms;
}
uint16_t timer0_get_us(void)
{
	return us;
}
uint64_t timer0_get_ticks(void)
{
	return ticks;
}
// -----------------------------------------------------------------------------
void timer0_init(void)
  {
      T0TCR &= 0<<0;
      T0TCR |= 1<<1;       //Reset Timer
      T0IR = 0xFF;         // Reset match and capture event interrupts
      T0TC = 0;            // Clear timer counter
      T0CTCR = 0;          // Timer Operation Selected
      T0PR = 0;            // Configuring Prescaler
      T0MR0 = 6000-1;      // Count for 0.01MHz interrupt, period = 100us
      T0MCR |= 1<<0;       //Reset the Interrupt
      T0MCR |= 1<<1;       // Reset Timer Counter

	  
      VICProtection = 0;            // VIC registers can be accessed in User or privileged mode
      VICVectAddr = 0;              // Clear interrupt
      

      VICIntSelect &= ~(1<<4);            		   // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)
      VICVectAddr0 = (unsigned int)&timer0_isr;    // Install ISR in VIC addr slot 0
      VICVectCntl0 = 0x20 | 4;            		   // IRQ type, TIMER 0 int enabled
      VICIntEnable |= (1<<4);                      // Turn on Timer0 Interrupt
	  
      T0TCR &= 0<<1;      // Cancelling Reset
      T0TCR |= 1<<0;      // Counting enable
	  
	  ticks = 0;
	  us = 0;
	  ms = 0;
	  s = 0;
	  min = 0;
	  pasts = 5;
  }
  __irq void timer0_isr()
  {
	  long int regVal;
      regVal = T0IR;            //Read current IR value
      us += 100;
      ticks++;
      ms = ticks/10;
	  s = ticks/10000;
	  min = ticks/600000;
	  if(s>pasts)
	  {
		  rtc_print_on_glcd();
		  pasts=s;
	  }
      T0IR = regVal;            //Write back to IR to clear Interrupt Flag                  // Clear timer interrupt
      VICVectAddr = 0;          // Clear interrupt in VIC
  }
  void rtc_print_on_glcd(void)
  {
	  print_int(35,0,HOUR,WHITE);
	  pcd8544_set_cursor(47,0);
	  pcd8544_print_char(':',BLACK);
	  print_int(52,0,MIN,WHITE);
	  pcd8544_set_cursor(64,0);
	  pcd8544_print_char(':',BLACK);
	  print_int(69,0,SEC,WHITE);
  }
#endif
