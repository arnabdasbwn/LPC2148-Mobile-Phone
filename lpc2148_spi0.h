// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header File is used to Interface SPI0 with LPC2148
// File Name: lpc2148_spi0.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_spi0_H
#define __lpc2148_spi0_H

#include "lpc214x.h"  
#include <inttypes.h>

#define SCK 4000000  // 4Mhz as PCD8455 has 4Mbit/s Max speed
#define counter 16   // 60Mhz/4Mhz = 15 Which is not Allowed as it is ODD .. so Nearest 16

void spi0_init();
void spi0_tx_char(uint8_t);
void spi0_tx_number(uint32_t);
void spi0_tx_string(unsigned char*);

uint8_t spi0_rx_char(void);

uint8_t spi_rec;

// -----------------------------------------------------------------------------

void spi0_init()
  {
		 PINSEL0 |=0x1500;      // P0.4, P0.5, P0.6, P0.7 are set as SCK, MISO, MOSI and GPIO 
		 IODIR0 |= 0X80; 		// SSEL is output 
		 IOSET0 |=0X80; 		// set SSEL to high 
		 S0SPCCR = counter;		// SPI clock prescale register minimum value is 8 
		 S0SPCR |= 1<<5 ;		// Device select as master, Set data to 8-bit, CPOL = 0, CPHA = 0
  }
 
void spi0_tx_char(uint8_t data)
  {
	 IO0CLR |= (1<<7);  				//enable transmission
     S0SPDR = data;
     while ( !(S0SPSR & 0x80) );	// Wait until the SPIF bit is set to indicate trabsfer complete
	 IO0SET |= (1<<7);   				// disable transmission
	  
  }
 
uint8_t spi0_rx_char()
  {
	  while ( !(S0SPSR & 0x80) );	// Wait until the SPIF bit is set to indicate trabsfer complete 
      spi_rec= S0SPDR;
      return spi_rec;
  }
 
void spi0_tx_string(unsigned char *str)
  {
     while(*str!='\0')
      {
        spi0_tx_char(*str);
        str++;
      }
  }
void spi0_tx_number(uint32_t n)
    {
        int digit[10],i,j,k=0,flag=0,top = 9;

        digit[0]=0;
        digit[1]=0;
        digit[2]=0;
        digit[3]=0; 
        digit[4]=0;
        digit[5]=0;
        digit[6]=0;
        digit[7]=0;
        digit[8]=0;
        digit[9]=0;


        while( n > 0 )
          {
              i = n % 10;
              digit[top--] = i;
              n = n / 10;
              k++;
              flag=1;
          }
        if(flag==0) k=1;

        for(j=10-k;j<10;j++)
          {
              spi0_tx_char(digit[j]+0x30);
          }

}
#endif
