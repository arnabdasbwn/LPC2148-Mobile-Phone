// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header File is Used to Transmit And Receive using UART0 & UART0
// File Name: lpc2148_uart0.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_uart0_H
#define __lpc2148_uart0_H

#include <lpc214x.h>
#include <inttypes.h>

#define MULVAL 15
#define DIVADDVAL 1
#define NEW_LINE 0xA   // Character for new line .. analogous to '\n'
#define UART0_RX_BUFFER_SIZE 256        // UART1 receive buffer size

void uart0_init(void);
void uart0_tx_char(unsigned char);
void uart0_tx_string(char*);
unsigned char uart0_rx_char(void);

__irq void uart0_isr(void);

volatile uint8_t  uart0_rx_buffer[UART0_RX_BUFFER_SIZE];
volatile uint8_t uart0_rx_insert_idx;
volatile uint8_t msg_flag,call_flag;

// -----------------------------------------------------------------------------

void uart0_init()
{
	
  U0IER = 0x00;                         // disable all interrupts
  U0IIR = 0;                            // clear interrupt ID
  U0RBR = 0;                            // clear receive register
  U0LSR = 0;                            // clear line status register
	
  PINSEL0 |= 0x5;                 // Select TxD for P0.0 and RxD for P0.1
  U0LCR = 0x83;                   //set Line Control Register (8 bit, 1 stop bit, no parity, enable DLAB)
  U0DLL	= 110; 				      //devisor
  U0DLM = 1;	
	
  U0FDR = (MULVAL<<4) | DIVADDVAL;// MULVAL=15(bits - 7:4) , DIVADDVAL=0(bits - 3:0)  */
  U0LCR &= 0x0F;                  // Set DLAB=0 to lock MULVAL and DIVADDVAL
                                  //BaudRate is now ~9600 and we are ready for UART communication!
	
  VICProtection = 0;            // VIC registers can be accessed in User or privileged mode
  VICVectAddr = 0;              // Clear interrupt

  VICIntSelect &= ~(1<<6);            		  // UART 1 interrupt is an IRQ (VIC_UART0 = 7)
  VICVectAddr1 = (unsigned int)&uart0_isr;    // Install ISR in VIC addr slot 1
  VICVectCntl1 = 0x20 | 6;            		  // IRQ type, UART0 interrupt enabled
  VICIntEnable |= (1<<6);                     // Turn on UART0 Interrupt
  U0IER |= 1<<0;
  
  msg_flag = 0;
  call_flag = 0;

} 
__irq void uart0_isr(void)
{
     uart0_rx_buffer[uart0_rx_insert_idx] = U0RBR;//uart0_rx_char();
	 
	  if (uart0_rx_insert_idx == UART0_RX_BUFFER_SIZE-1)  // check for more room in queue
          uart0_rx_insert_idx = 0;         // update insert index
	  else 
		  uart0_rx_insert_idx++;
	  
	  //uart0_tx_char(uart0_rx_buffer[uart0_rx_insert_idx-1]);
	  
	  if(uart0_rx_buffer[uart0_rx_insert_idx-4] == '+' && uart0_rx_buffer[uart0_rx_insert_idx-3] == 'C' && uart0_rx_buffer[uart0_rx_insert_idx-2] == 'L' && uart0_rx_buffer[uart0_rx_insert_idx-1] == 'I' && uart0_rx_insert_idx > 5 )
	  {
		  call_flag = 1;
	  }
	  if(uart0_rx_buffer[uart0_rx_insert_idx-4] == '+' && uart0_rx_buffer[uart0_rx_insert_idx-3] == 'C' && uart0_rx_buffer[uart0_rx_insert_idx-2] == 'M' && uart0_rx_buffer[uart0_rx_insert_idx-1] == 'T' )
	  {
		  msg_flag = 1;
	  }

  VICVectAddr = 0;          // Clear interrupt in VIC
}

void uart0_tx_string(char* a)
{
   for(;*a;++a)
  {
    uart0_tx_char(*a);
  }
}
unsigned char uart0_rx_char(void)
{
  while (!(U0LSR&(1<<0)));    //when U0LSR_bit.DR is 1 - U0RBR contains valid data
  return U0RBR;
}
void uart0_tx_char(unsigned char data)
{
  while (!(U0LSR&(1<<5)));  //when U0LSR_bit.THRE is 0 - U0THR contains valid data.
  U0THR = data;
}
#endif