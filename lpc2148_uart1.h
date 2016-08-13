// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header File is Used to RX and Tx using UART1 Interrupt
// File Name: lpc2148_uart1.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_uart1_H
#define __lpc2148_uart1_H

#include <lpc214x.h>
#include <inttypes.h>

#define MULVAL 15
#define DIVADDVAL 1
#define NEW_LINE 0xA   // Character for new line .. analogous to '\n'
#define UART1_RX_BUFFER_SIZE 128        // UART1 receive buffer size

void uart1_init(void);
void uart1_tx_char(unsigned char);
void uart1_tx_string(char*);
unsigned char uart1_rx_char(void);

__irq void uart1_isr(void);

volatile uint8_t  uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
volatile uint8_t uart1_rx_insert_idx, uart1_rx_extract_idx;

// -----------------------------------------------------------------------------

void uart1_init(void)
{
  uart1_rx_insert_idx = 0;
  uart1_rx_extract_idx = 127;
   

  U1IER = 0x00;                         // disable all interrupts
  U1IIR = 0;                            // clear interrupt ID
  U1RBR = 0;                            // clear receive register
  U1LSR = 0;                            // clear line status register
	
  PINSEL0 |= 0x50000;
	
  U1LCR = 0x83;   //set Line Control Register (8 bit, 1 stop bit, no parity, enable DLAB)
  U1DLL = 110;    //devisor
  U1DLM = 1;
	
  U1FDR = (MULVAL<<4) | DIVADDVAL; // MULVAL=15(bits - 7:4) , DIVADDVAL=0(bits - 3:0)  */
  U1LCR &= 0x0F;     // Set DLAB=0 to lock MULVAL and DIVADDVAL
					 //BaudRate is now ~9600 and we are ready for UART communication!
  
  VICProtection = 0;            // VIC registers can be accessed in User or privileged mode
  VICVectAddr = 0;              // Clear interrupt

  VICIntSelect &= ~(1<<7);            		  // UART 1 interrupt is an IRQ (VIC_UART1 = 7)
  VICVectAddr1 = (unsigned int)&uart1_isr;    // Install ISR in VIC addr slot 1
  VICVectCntl1 = 0x20 | 7;            		  // IRQ type, UART1 interrupt enabled
  VICIntEnable |= (1<<7);                     // Turn on UART1 Interrupt
  U1IER |= 1<<0;
}
__irq void uart1_isr(void)
{
     uart1_rx_buffer[uart1_rx_insert_idx] = U1RBR;
	 
	  if (uart1_rx_insert_idx == UART1_RX_BUFFER_SIZE-1)  // check for more room in queue
          uart1_rx_insert_idx = 0;         // update insert index
	  else 
		  uart1_rx_insert_idx++;
	  
	  uart1_tx_char(uart1_rx_buffer[uart1_rx_insert_idx-1]);

  VICVectAddr = 0;          // Clear interrupt in VIC
}
void uart1_tx_char(unsigned char data)
{
  while (!(U1LSR&(1<<5)));      //when U0LSR_bit.THRE is 0 - U0THR contains valid data.
  U1THR = data;
}
void uart1_tx_string(char* a)
{
   for(;*a;++a)
  {
    uart1_tx_char(*a);
  }
}
unsigned char uart1_rx_char(void)
{
  while (!(U1LSR&(1<<0)));		//when U0LSR_bit.DR is 1 - U0RBR contains valid data
  return U1RBR;
}
#endif
