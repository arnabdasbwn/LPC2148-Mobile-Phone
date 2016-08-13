// -----------------------------------------------------------------------------
// -          Arnab Kumar Das  LPC21xx  ARM Keil MDK5 + Legacy Support
// -----------------------------------------------------------------------------
// Header: This Header is Usd to access any GPIO pin of LPC2148
// File Name: lpc2148_gpio.h
// Author: Arnab Kumar Das
// Youtube Link: https://www.youtube.com/c/ArnabDasBwn
// GitHub Link: https://github.com/arnabdasbwn
// Date: 13/07/2016 5:48:20 AM  IST
// Licence: GNU General Public License
// -----------------------------------------------------------------------------
#ifndef __lpc2148_gpio_H
#define __lpc2148_gpio_H

#include <lpc214x.h>
#include <inttypes.h>
#include <stdbool.h>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

//------------------------------------------------------------------------------

__inline void gpio_set_pin_function(uint8_t,uint8_t); 	// PINSEL0 PINSEL1 PINSEL2 Select GPIO Pin Function
__inline void gpio_set_pin_output(uint8_t); 			// IO0DIR IO1DIR Set GPIO Pin as Output
__inline void gpio_set_pin_input(uint8_t); 				// IO0DIR IO1DIR Set GPIO Pin as Input
__inline bool gpio_check_pin_high(uint8_t); 			// IO0PIN IO1PIN Check a GPIO pin for High
__inline bool gpio_check_pin_low(uint8_t); 				// IO0PIN IO1PIN Check a GPIO pin for low
__inline void gpio_set_pin_high(uint8_t); 				// IO0SET IO1SET Set a GPIO pin to High
__inline void gpio_set_pin_low(uint8_t); 				// IO0CLR IO1CLR Clear a GPIO pin to Low
__inline void gpio_toggle_pin(uint8_t); 				// Toggle Pin Irrespective Of Its Previous State

//------------------------------------------------------------------------------


__inline void gpio_set_pin_function(uint8_t pin,uint8_t function)
  {
      if(pin <= 15)
        {
            pin = pin*2;
            PINSEL0 =(function<<pin);
        }
      else if(pin > 15 && pin <= 31)
        {
            pin = pin - 16;
            pin = pin*2;
            PINSEL1 =(function<<pin);
        }
      else if(pin > 31 && pin <= 48)
        {
            pin = pin - 32;
            pin = pin*2;
            PINSEL2 =(function<<pin);
        }
      else
        {
        }
  }

__inline void gpio_set_pin_output(uint8_t pin)
  {
      if(pin <= 31)
        {
            //gpio_set_pin_function(pin,0);
            IO0DIR |=(1<<pin);
        }
      else if(pin > 31 && pin <= 48)
        {
            //gpio_set_pin_function(pin,0);
            pin = pin - 16;
            IO1DIR |=(1<<pin);
        }
      else
        {
        }
  }

__inline void gpio_set_pin_input(uint8_t pin)
  {
      if(pin <= 31)
        {
            //gpio_set_pin_function(pin,0);
            IO0DIR &=~(1<<pin);
        }
      else if(pin > 31 && pin <= 48)
        {
            //gpio_set_pin_function(pin,0);
            pin = pin - 16;
            IO1DIR &=~(1<<pin);
        }
      else
        {
        }
  }

__inline bool gpio_check_pin_high(uint8_t pin)
  {
      if(pin <= 31)
        {
            gpio_set_pin_input(pin);
            if(IO0PIN&(1<<pin))
                return true;
            else
                return false;

        }
      else if(pin > 31 && pin <= 48)
        {
            gpio_set_pin_input(pin);
            pin = pin - 16;
            if(IO1PIN&(1<<pin))
                return true;
            else
                return false;
        }
      else
        {
        }
  }

__inline bool gpio_check_pin_low(uint8_t pin)
  {
      if(pin <= 31)
        {
            gpio_set_pin_input(pin);
            if(IO0PIN&(1<<pin))
                return false;
            else
                return true;
        }
      else if(pin > 31 && pin <= 48)
        {
            gpio_set_pin_input(pin);
            pin = pin - 16;
            if(IO1PIN&(1<<pin))
                return false;
            else
                return true;
        }
      else
        {
        }
  }

__inline void gpio_set_pin_high(uint8_t pin)
  {
      if(pin <= 31)
        {
            gpio_set_pin_output(pin);
            IO0SET |=(1<<pin);
        }
      else if(pin > 31 && pin <= 48)
        {
            gpio_set_pin_output(pin);
            pin = pin - 16;
            IO1SET |=(1<<pin);
        }
      else
        {
        }
  }

__inline void gpio_set_pin_low(uint8_t pin)
  {
      if(pin <= 31)
        {
            gpio_set_pin_output(pin);
            IO0CLR |=(1<<pin);
        }
      else if(pin > 31 && pin <= 48)
        {
            gpio_set_pin_output(pin);
            pin = pin - 16;
            IO1CLR |=(1<<pin);
        }
      else
        {
        }
  }

__inline void gpio_toggle_pin(uint8_t pin)
  {
      if(pin <= 31)
        {
            gpio_set_pin_output(pin);
            if(IO0SET&1<<pin)
                IO0CLR |= 1<<pin;
            else
                IO0SET |= 1<<pin;
        }
      else if(pin > 31 && pin <= 48)
        {
            gpio_set_pin_output(pin);
            pin = pin - 16;
            if(IO1SET&1<<pin)
                IO1CLR |= 1<<pin;
            else
                IO1SET |= 1<<pin;
        }
      else
        {
        }
  }
  #endif
 