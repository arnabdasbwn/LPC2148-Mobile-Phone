// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header File is Used to Initiate The PCLK and CCLK of LPC2148 System 
// File Name: lpc2148_clock.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_clock_H
#define __lpc2148_clock_H

#include <LPC214x.h>

#define FOSC 12000000 		// 12Mhz
#define M 4                 // 5-1 = 4
#define CCLK FOSC*M 		// 60Mhz
#define P 1
#define FCCO CCLK*2*P       // 240Mhz
#define PCLK 60000000       // 60Mhz  VPBDIV = 1
#define PRESCALE 0          // 1 PCLK clock cycles to increment TC by 1

//------------------------------------------------------------------------------

void clock_init(void);     // Initilizes all the Clocks CPU Clock AND Peripheral Clock
void feed(void);			// Feed Sequence to Configure the PLL

//------------------------------------------------------------------------------

void clock_init(void)
  {
      PLL0CON = 0x01; // PPLE=1 & PPLC=0 so it will be enabled but not connected after FEED sequence
	  PLL0CFG = 0x24;
      feed();
      while( !(PLL0STAT & 1<<10));
      PLL0CON = 0x03; // Enabled and Connected 
      feed();
      VPBDIV = 1;
  }

void feed(void)
  {
      PLL0FEED = 0xAA;
      PLL0FEED = 0x55;
  }
#endif
