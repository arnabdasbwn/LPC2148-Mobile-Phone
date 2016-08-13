#ifndef __lpc2148_keypad_H
#define __lpc2148_keypad_H

#include <lpc214x.h>
#include <inttypes.h>
#include <ctype.h>

#define R1 (1<<16)
#define R2 (1<<17)
#define R3 (1<<18)
#define R4 (1<<19)
#define R5 (1<<20)
#define R6 (1<<21)
#define C1 (1<<22)
#define C2 (1<<23)
#define C3 (1<<24)

#define ROWPIN PINSEL2	//define MCU port connected to LCD ROW pins
#define COLPIN PINSEL2	//define MCU port connected to LCD COLLUMN pins

#define ROWIN IO1PIN	//define MCU port connected to LCD ROW pins
#define COLIN IO1PIN	//define MCU port connected to LCD COLLUMN pins

#define ROWSET IO1SET	//define MCU port connected to LCD ROW pins
#define COLSET IO1SET	//define MCU port connected to LCD COLLUMN pins

#define ROWCLR IO1CLR	//define MCU port connected to LCD ROW pins
#define COLCLR IO1CLR	//define MCU port connected to LCD COLLUMN pins

#define ROWDIR IO1DIR	//define MCU direction register to ROW data pins
#define COLDIR IO1DIR	//define MCU direction register to COLLUMN  pins

void keyInit(void);
void delay1();
void delay2();
void setHoldTime(uint16_t);
void setDebounceTime(uint16_t);
unsigned char waitForKey(uint16_t);
unsigned char waitForKeyAlways (void);
unsigned char waitForButton(uint8_t);
unsigned char getKey(void);

uint16_t HoldTime = 100;
uint16_t DebounceTime = 100;
uint8_t Position =0;

unsigned char button[10][3]=
   {' ' , ' ' , ' ' , //1 
	'a' , 'b' , 'c' , //2
	'd' , 'e' , 'f' , //3
	'g' , 'h' , 'i' , //4
	'j' , 'k' , 'l' , //5
	'm' , 'n' , 'o' , //6
	'p' , 'q' , 'r' , //7
	's' , 't' , 'u' , //8
	'v' , 'w' , 'x' , //9
	'y' , 'z' , ' '  };//0

void keyInit()
  {
      ROWPIN&=~R1&~R2&~R3&~R4&~R5&~R6;
      COLPIN&=~C1&~C2&~C3;

      ROWDIR&=~R1&~R2&~R3&~R4&~R5&~R6;
      COLDIR|=C1|C2|C3;

      ROWCLR|=R1|R2|R3|R4|R5|R6;
      COLCLR|=C1|C2|C3;

  }

void setHoldTime(uint16_t k)
  {
      HoldTime = k;
  }

void setDebounceTime(uint16_t k)
  {
      DebounceTime = k;
  }


void delay1()
  {
      timer0_delay_ms(HoldTime);
  }
void delay2()
  {
       timer0_delay_ms(DebounceTime);
  }
unsigned char waitForButton(uint8_t pos)
  {
	  int count = 0;
	  unsigned char previous; 
	  unsigned char key;
	  unsigned char final;
	  key = waitForKey(400);
	  previous = key;
      final = key;
	  
	  if( key == 'B' || key == '(' || key == ')' || key == '<'|| key == '>')
		  return key;
	  else if(key != 'x')
	  {
		   pcd8544_set_cursor(pos*6,1);
		   pcd8544_print_char(key,BLACK);
	  }
	  
	  while(count<3 && isdigit (key))
	  {	  
		  key = waitForKey(300);
		  if(previous == key && isdigit (key))
		  {
			  
			  //LCDcursorLeft(1);
              //LCDchar(' ');
              //LCDcursorLeft(1);
			  final = button[key-0x30][count];
			  count ++;
			  //LCDchar(final);
			  pcd8544_set_cursor(pos*6,1);
		      pcd8544_print_char(final,BLACK);
		  }
		  else if(key != 'x' && ( key == 'B' ||  key == ')' ))
		  {
			  return key;
		  }
		  else if(key == '(' || key == 'K')
			  return final;
		  else
			  return final;
		  
	  }
	  return final;	  
  
  }
unsigned char waitForKeyAlways (void)
  {
      unsigned char key;
      ROWCLR=R1|R2|R3|R4|R5|R6;
      COLCLR=C1|C2|C3;

      while(1)
        {				
            COLCLR=C1;
            COLSET=C2|C3;                     // first column = 0

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='K';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key='(';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='1';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='4';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='7';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='*';
                        delay2();
                        return key;
                    }
              }

            COLCLR=C2;
            COLSET=C1|C3;                     //second column = 1

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='>';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key='<';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='2';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='5';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='8';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='0';
                        delay2();
                        return key;
                    }
              }

            COLCLR=C3;
            COLSET=C1|C2;                     //third column = 2

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='B';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key=')';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='3';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='6';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='9';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='#';
                        delay2();
                        return key;
                    }
              }
        }

  }
unsigned char getKey (void)
  {
      unsigned char key;
      ROWCLR=R1|R2|R3|R4|R5|R6;
      COLCLR=C1|C2|C3;

      COLCLR=C1;
      COLSET=C2|C3;                     // first column = 0

      if(!(ROWIN&R1))
        {
            delay1();
            if(!(ROWIN&R1))
              {
                  key='K';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R2))
        {
            delay1();
            if(!(ROWIN&R2))
              {
                  key='(';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R3))
        {
            delay1();
            if(!(ROWIN&R3))
              {
                  key='1';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R4))
        {
            delay1();
            if(!(ROWIN&R4))
              {
                  key='4';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R5))
        {
            delay1();
            if(!(ROWIN&R5))
              {
                  key='7';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R6))
        {
            delay1();
            if(!(ROWIN&R6))
              {
                  key='*';
                  delay2();
                  return key;
              }
        }

      COLCLR=C2;
      COLSET=C1|C3;                     //second column = 1

      if(!(ROWIN&R1))
        {
            delay1();
            if(!(ROWIN&R1))
              {
                  key='>';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R2))
        {
            delay1();
            if(!(ROWIN&R2))
              {
                  key='<';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R3))
        {
            delay1();
            if(!(ROWIN&R3))
              {
                  key='2';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R4))
        {
            delay1();
            if(!(ROWIN&R4))
              {
                  key='5';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R5))
        {
            delay1();
            if(!(ROWIN&R5))
              {
                  key='8';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R6))
        {
            delay1();
            if(!(ROWIN&R6))
              {
                  key='0';
                  delay2();
                  return key;
              }
        }

      COLCLR=C3;
      COLSET=C1|C2;                     //third column = 2

      if(!(ROWIN&R1))
        {
            delay1();
            if(!(ROWIN&R1))
              {
                  key='B';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R2))
        {
            delay1();
            if(!(ROWIN&R2))
              {
                  key=')';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R3))
        {
            delay1();
            if(!(ROWIN&R3))
              {
                  key='3';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R4))
        {
            delay1();
            if(!(ROWIN&R4))
              {
                  key='6';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R5))
        {
            delay1();
            if(!(ROWIN&R5))
              {
                  key='9';
                  delay2();
                  return key;
              }
        }
      else if(!(ROWIN&R6))
        {
            delay1();
            if(!(ROWIN&R6))
              {
                  key='#';
                  delay2();
                  return key;
              }
        }

      return '?';
  }
unsigned char waitForKey (uint16_t delay)
  {
      unsigned char key;
	  uint16_t time2;
	  uint16_t time = timer0_get_ms();
      ROWCLR=R1|R2|R3|R4|R5|R6;
      COLCLR=C1|C2|C3;
	  time = time + delay;
      while(1)
        {
			time2 = timer0_get_ms();
			
			if(time2 >= time)
				return 'x';
				
            COLCLR=C1;
            COLSET=C2|C3;                     // first column = 0

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='K';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key='(';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='1';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='4';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='7';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='*';
                        delay2();
                        return key;
                    }
              }

            COLCLR=C2;
            COLSET=C1|C3;                     //second column = 1

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='>';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key='<';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='2';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='5';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='8';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='0';
                        delay2();
                        return key;
                    }
              }

            COLCLR=C3;
            COLSET=C1|C2;                     //third column = 2

            if(!(ROWIN&R1))
              {
                  delay1();
                  if(!(ROWIN&R1))
                    {
                        key='B';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R2))
              {
                  delay1();
                  if(!(ROWIN&R2))
                    {
                        key=')';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R3))
              {
                  delay1();
                  if(!(ROWIN&R3))
                    {
                        key='3';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R4))
              {
                  delay1();
                  if(!(ROWIN&R4))
                    {
                        key='6';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R5))
              {
                  delay1();
                  if(!(ROWIN&R5))
                    {
                        key='9';
                        delay2();
                        return key;
                    }
              }
            else if(!(ROWIN&R6))
              {
                  delay1();
                  if(!(ROWIN&R6))
                    {
                        key='#';
                        delay2();
                        return key;
                    }
              }
        }

  }
  #endif
  