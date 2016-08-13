#include <lpc214x.h>
#include <ctype.h>
#include "lpc2148_clock.h"
#include "lpc2148_rtc.h"
#include "lpc2148_gpio.h"
#include "lpc2148_spi0.h"
#include "lpc2148_pcd8544.h"
#include "lpc2148_timer0.h"
#include "lpc2148_keypad.h"
#include "lpc2148_uart0.h"

 void mobile_menu_open(void);
 void mobile_menu_call(unsigned char);
 void mobile_menu_call_receive(unsigned char);
 void mobile_menu_message(void);
 void mobile_menu_message_receive(void);
 void mobile_menu_clock(void);
 void mobile_menu_setting(void);
 
 unsigned char loc[2];
 
int main()
{
	 unsigned char key;
	 int m,n,pos;
	 uint8_t sec1,sec2,min1,min2;
	 uint8_t  phone[13];
	 phone[0] = '+';
	 
	 clock_init();
	 rtc_init();
	 //rtc_set_default_time();
	 timer0_init();	
	 spi0_init();
	 uart0_init();
     pcd8544_init();
	 keyInit();
	
	 //Boot Sequence
	 	
	 pcd8544_backlight_state(ON);
     pcd8544_clear_screen(WHITE);
     pcd8544_print_image(bmp1);
     timer0_delay_ms(2000);
	
	 pcd8544_print_image(bmp2);
     timer0_delay_ms(1500);
	 
	 pcd8544_print_image(bmp3);
     timer0_delay_ms(1500);
	 
	 pcd8544_print_image(happy_bmp);
	 pcd8544_print_string(0,5,"MENU",BLACK);
	 pcd8544_print_string(68,5,"<-",BLACK);
	 
	 uart0_tx_string("AT\r");
	 timer0_delay_ms(10);
     uart0_tx_string("AT+CMGF=1\r");	 
	 
     while(1)
     {
		 key = waitForKey(100);
		 
		 if(key == 'K')
			 mobile_menu_open();
		 else if(isdigit (key))
			 mobile_menu_call(key);
		 else {}
			 
	 if(msg_flag == 1)
	 {	 //U0IER = 0x00;
		 pcd8544_print_string(0,0,"MSG",BLACK);
		 for(m=1;m<UART0_RX_BUFFER_SIZE;m++)
		  {
			  if(uart0_rx_buffer[m-1] == 'S' && uart0_rx_buffer[m] == 'M')
			  {
				  loc[0]=uart0_rx_buffer[m+3];
				  loc[1]=uart0_rx_buffer[m+4];
				  pcd8544_print_string(0,1,loc,BLACK);
				  for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
							uart0_rx_buffer[n] = ' ';
				  break;
			  }
		  }
		 // U0IER |= 1<<0;
	 }

	 if(call_flag == 1)
	 {
		U0IER = 0x00;
		  for(m=0;m<UART0_RX_BUFFER_SIZE;m++)
		  {
			  if(uart0_rx_buffer[m] == '9')
			  {
				  pos=m;
				  break;
			  }
		  }
		  
		  for(n=1;n<13;n++)
		  {
			 phone[n] = uart0_rx_buffer[pos];
			 pos++;
		  }
		   pcd8544_clear_screen(WHITE);
		   pcd8544_print_string(0,2,"RECEIVING >>",BLACK);
		   pcd8544_print_string(0,3,phone,BLACK);
		   pcd8544_print_string(0,5,"PICK",BLACK);
		   pcd8544_print_string(54,5,">END<",BLACK);
		  

		  
		  while(1)
		  {

		      key = waitForKeyAlways();
		  
			 if(key==')' || key == 'B')
				{
					uart0_tx_string("ATH\r");
					pcd8544_print_image(happy_bmp);
					pcd8544_print_string(0,5,"MENU",BLACK);
					pcd8544_print_string(68,5,"<-",BLACK);
					for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
							uart0_rx_buffer[n] = ' ';
					pos=0;
					call_flag = 0;
					U0IER |= 1<<0;
					break;
				}
				else if (key=='K' || key == '(')
				{
					uart0_tx_string("ATA\r");
					pcd8544_clear_screen(WHITE);
					pcd8544_print_string(0,2,"IN CALL >> ",BLACK);
					pcd8544_print_string(0,3,phone,BLACK);
					pcd8544_print_string(54,5,">END<",BLACK);
					for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
							uart0_rx_buffer[n] = ' ';
					call_flag = 0;
					pos=0;
					U0IER |= 1<<0;
					break;
					
				}
			}
		  		  sec1 = timer0_get_s();
				  min1 = timer0_get_min();
				while(1)
				{
				     sec2 = timer0_get_s() - sec1;
			         min2 = timer0_get_min() - min1;
					 print_int(4,4,min2,BLACK);
					 pcd8544_print_string(10,4,":",BLACK);
					 print_int(16,4,sec2,BLACK);
					
					key = waitForKey(200);
					
					if(key==')' || key == 'B')
					{
						uart0_tx_string("ATH\r");
						pcd8544_print_image(happy_bmp);
						pcd8544_print_string(0,5,"MENU",BLACK);
						pcd8544_print_string(68,5,"<-",BLACK);
						for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
							uart0_rx_buffer[n] = ' ';
						call_flag = 0;
						pos=0;
						U0IER |= 1<<0;
						break;
					}
				}
		 call_flag = 0;
		 U0IER |= 1<<0;
				}

	};
}
void mobile_menu_open()
{
	int select = 0;
	unsigned char key;
	pcd8544_print_image(phone_bmp);
	pcd8544_print_string(0,5,"OK",BLACK);
	pcd8544_print_string(68,5,"<-",BLACK);
	while(1)
	{
		key=waitForKeyAlways();
		if(key == 'K')
		{
			if(select == 0)
			{
				pcd8544_clear_screen(WHITE);
				pcd8544_print_string(0,5,"CALL",BLACK);
				pcd8544_print_string(54,5,"ERASE",BLACK);
				pcd8544_print_string(4,1,"ENTER Ph. No.",BLACK);
				mobile_menu_call(waitForKeyAlways());
				pcd8544_print_image(happy_bmp);
				pcd8544_print_string(0,5,"MENU",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
				return;
			}
			else if(select == 1)
			{
				mobile_menu_message();
				pcd8544_print_image(happy_bmp);
				pcd8544_print_string(0,5,"MENU",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
				return;
			}
			else if(select == 2)
				mobile_menu_message_receive();
			else if(select == 3)
				mobile_menu_clock();
			else if(select == 4)
				mobile_menu_setting();
				
		}
		else if(key == '>')
		{
			select++;
			if(select > 4)
				select=0;
			
			if(select == 0)
			{
				pcd8544_print_image(phone_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 1)
			{
				pcd8544_print_image(outbox_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 2)
			{
				pcd8544_print_image(inbox_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 3)
			{
				pcd8544_print_image(clock_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 4)
			{
				pcd8544_print_image(setting_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
				
		}
		else if(key == '<')
		{
			select--;
			if(select<0)
				select = 4;
			
			if(select == 0)
			{
				pcd8544_print_image(phone_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 1)
			{
				pcd8544_print_image(outbox_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 2)
			{
				pcd8544_print_image(inbox_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 3)
			{
				pcd8544_print_image(clock_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
			else if(select == 4)
			{
				pcd8544_print_image(setting_bmp);
				pcd8544_print_string(0,5,"OK",BLACK);
				pcd8544_print_string(68,5,"<-",BLACK);
			}
		}
		else if(key == ')' || key == 'B')
		{
		pcd8544_print_image(happy_bmp);
		pcd8544_print_string(0,5,"MENU",BLACK);
		pcd8544_print_string(68,5,"<-",BLACK);
		return;
		}
			
	}
}
void mobile_menu_call(unsigned char ch)
{
	  int i=4;
      unsigned char phoneno[13] = {' '};
      unsigned char key;
	  if(ch == ')' || ch == '(' || ch == 'K' || ch == '<' || ch == '>' || ch == 'B')
		  return;
	  phoneno[0]='+';
	  phoneno[1]='9';
	  phoneno[2]='1';
	  phoneno[3]=ch;
	  pcd8544_clear_screen(WHITE);
	  pcd8544_print_string(0,5,"CALL",BLACK);
	  pcd8544_print_string(54,5,"ERASE",BLACK);
	  pcd8544_print_string(4,1,"ENTER Ph. No.",BLACK);
	  pcd8544_print_string(3,3,phoneno,BLACK);
            while( i>2 && i <=13 )
              {
                  key=waitForKeyAlways();

                  if(isdigit(key) && i<13 )
                    {
                        phoneno[i]=key;
						pcd8544_print_string(3,3,phoneno,BLACK);
                        i++;
                    }
                  else if(key=='B' && i >3)
                    {
                        i--;
						phoneno[i]=' ';
						pcd8544_print_string(3,3,phoneno,BLACK);
                    }
                  else if(key==')')
                    {
						pcd8544_print_image(happy_bmp);
						pcd8544_print_string(0,5,"MENU",BLACK);
						pcd8544_print_string(68,5,"<-",BLACK);
						return;
                    }
                  else if(key=='(' || key == 'K')
                    {
                        break;
                    }
              }
			  
            // USE GSM Module To Call
	
			  uart0_tx_string("ATD");
			  uart0_tx_string(phoneno);
			  uart0_tx_string(";\r");
			  pcd8544_print_string(3,4,"CALLING >>>",BLACK);
			  pcd8544_print_string(0,5,"    ",BLACK);
			  pcd8544_print_string(54,5,">END<",BLACK);
            
			  while(waitForKeyAlways()!=')' );//  && check for NO Carrier
			  
			  uart0_tx_string("ATH\r");
			  
			  //SEND ATH Command to Hang UP
			  
		pcd8544_print_image(happy_bmp);
		pcd8544_print_string(0,5,"MENU",BLACK);
		pcd8544_print_string(68,5,"<-",BLACK);

}
 void mobile_menu_message(void)
 {
	  int i=3;
      unsigned char phoneno[13] = {' '};
	  unsigned char message[60] = {' '};
      unsigned char key;
	  phoneno[0]='+';
	  phoneno[1]='9';
	  phoneno[2]='1';
	  pcd8544_clear_screen(WHITE);
	  pcd8544_print_string(0,5,"MSG",BLACK);
	  pcd8544_print_string(54,5,"ERASE",BLACK);
	  pcd8544_print_string(4,1,"ENTER Ph. No.",BLACK);
	  pcd8544_print_string(3,3,phoneno,BLACK);
            while( i>2 && i <=13 )
              {
                  key=waitForKeyAlways();
					
                  if(isdigit(key) && i<13 )
                    {
                        phoneno[i]=key;
						pcd8544_print_string(3,3,phoneno,BLACK);
                        i++;
                    }
                  else if(key=='B' && i >2)
                    {
                        i--;
						phoneno[i]=' ';
						pcd8544_print_string(3,3,phoneno,BLACK);
                    }
                  else if(key==')')
                    {
						pcd8544_print_image(happy_bmp);
						pcd8544_print_string(0,5,"MENU",BLACK);
						pcd8544_print_string(68,5,"<-",BLACK);
						return;
                    }
                  else if(key=='(' || key == 'K')
                    {
                        break;
                    }
              }
			  
			pcd8544_clear_screen(WHITE);
			pcd8544_print_string(0,5,"MSG",BLACK);
			pcd8544_print_string(54,5,"ERASE",BLACK);
			pcd8544_print_string(0,0,"TYPE",BLACK);
			  
			  i=0;
			   while( i>=0 && i <60 )
              {
                  key=waitForButton(i);
					
				  if(key=='(' || key == 'K')
                    {
                        break;
                    }
					else if(key==')')
                    {
						pcd8544_print_image(happy_bmp);
						pcd8544_print_string(0,5,"MENU",BLACK);
						pcd8544_print_string(68,5,"<-",BLACK);
						return;
                    } 
					else if(key=='B' && i > 0)
                    {
                        i--;
						message[i]=' ';
						pcd8544_print_string(0,1,message,BLACK);
                    }
                  else if(key != 'x' && i<120 )
                    {
                        message[i]=key;
						pcd8544_print_string(0,1,message,BLACK);
                        i++;
                    }
                  
              }
			  
             // USE GSM Module To MSG
			  
			  uart0_tx_string("AT+CMGS=\"");
			  uart0_tx_string(phoneno);
			  uart0_tx_string("\"\r");
			  timer0_delay_ms(200);
			  uart0_tx_string(message);
			  uart0_tx_char(26);
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(3,3,phoneno,BLACK);
			  pcd8544_print_string(3,4,"SENDING",BLACK);
			 			  
			  timer0_delay_ms(3000);
			  
		      pcd8544_clear_screen(WHITE);
		      pcd8544_print_string(0,5,"MENU",BLACK);
		      pcd8544_print_string(68,5,"<-",BLACK);
 }
 void mobile_menu_message_receive(void)
 {
	 unsigned char key;
	 int i=0;
	 int m,n,pos1=0,pos2=0;
	 unsigned char  phone[13];
	 unsigned char  message[40];
	 unsigned char read[256];
	 phone[0] = '+';
	 //loc[0]=0x30;
	 //loc[1]=0x31;
     pcd8544_clear_screen(WHITE);
     pcd8544_print_string(0,5,"VIEW",BLACK);
     pcd8544_print_string(54,5,"ERASE",BLACK);
     pcd8544_print_string(4,1,"ENTER MSG. No.",BLACK);
     //pcd8544_print_string(3,3,loc,BLACK);
            while( i>=0 && i <=1)
              {
                  key=waitForKeyAlways();

                  if(isdigit(key) && i<=1 )
                    {
                        loc[i]=key;
						pcd8544_print_string(3,3,loc,BLACK);
                        i++;
                    }
                  else if(key=='B' && i >0)
                    {
                        i--;
						loc[i]=' ';
						pcd8544_print_string(3,3,loc,BLACK);
                    }
                  else if(key==')')
                    {
						pcd8544_print_image(happy_bmp);
						pcd8544_print_string(0,5,"MENU",BLACK);
						pcd8544_print_string(68,5,"<-",BLACK);
						return;
                    }
                  else if(key=='(' || key == 'K')
                    {
                        break;
                    }
              }
	 pcd8544_clear_screen(WHITE);
	 pcd8544_print_string(0,0,"   ",BLACK);
	 pcd8544_print_string(54,5,"BACK",BLACK);
	 pcd8544_print_string(0,5,loc,BLACK);
	 
	 for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
			uart0_rx_buffer[n] = ' ';
	 
	 uart0_tx_string("AT+CMGR=");
	 uart0_tx_string(loc);
	 uart0_tx_char('\r');
	 timer0_delay_ms(200);
	 //U0IER = 0x00;
	 
     n=0; 
	 while(n<256)
	 {
		 if(isprint(uart0_rx_buffer[n]) && (!isspace(uart0_rx_buffer[n])))
		 {
			 if((pos2+1)!=n)
			 {
				 read[pos1] = ' ';
				 pos1++;
			 }
			 read[pos1]=uart0_rx_buffer[n];
			 pos2=n;
			 pos1++;
			 timer0_delay_ms(2);
		 }
		 n++;
	 }	
		n=0;
		while(n<254)
	  {
		 if(read[n] == '+' && read[n+1] == '9' && read[n+2] == '1')
		 {
			 pos1 = n+1;
			 break;
		 }
		 n++;
	 }
	
	 for(n=1;n<13;n++)
	 {
		 phone[n]=read[pos1];
		 pos1++;
	 }
	 
	 n=0;
		while(n<254)
	  {
		 if(read[n] == '+' && read[n+1] == '2' && read[n+2] == '2')
		 {
			 pos2 = n+5;
			 break;
		 }
		 n++;
	 }
		 n=0;
		while(n<254)
	  {
		 if(read[n] == 'O' && read[n+1] == 'K' )
		 {
			 pos1 = n-1;
			 break;
		 }
		 n++;
	 }
	 for(n=1;n<40;n++)
	 {
		 message[n]=read[pos2];
		 pos2++;
		 if(pos2-1==pos1)
			 break;
	 }
	  
	  pcd8544_print_string(0,1,phone,BLACK);
	  pcd8544_print_string(0,2,message,BLACK);

		  while(1)
		  {
		    key = waitForKeyAlways();
		  
			 if(key==')' || key == 'B')
				{
					pcd8544_print_image(happy_bmp);
					pcd8544_print_string(0,5,"MENU",BLACK);
					pcd8544_print_string(68,5,"<-",BLACK);
					for(n=0;n<UART0_RX_BUFFER_SIZE;n++)
							uart0_rx_buffer[n] = ' ';
					msg_flag = 0;
					loc[0]=0;
					loc[1]=0;
					break;
				}
			}
		 loc[0]=0;
		 loc[1]=0;
		 msg_flag = 0;
		 return;
}

 void mobile_menu_clock(void)
 {
	 unsigned char key;
	 pcd8544_clear_screen(WHITE);
	 pcd8544_print_string(0,5,"SET",BLACK);
	 pcd8544_print_string(68,5,"<-",BLACK);
	 pcd8544_print_string(0,2,"=> ",BLACK);
	 pcd8544_print_string(0,3,"=> ",BLACK);
	 
      while(1)
      {
		  print_int(18,2,rtc_get_hour(),WHITE);
		  pcd8544_set_cursor(30,2);
		  pcd8544_print_char(':',BLACK);
		  print_int(35,2,rtc_get_min(),WHITE);
		  pcd8544_set_cursor(47,2);
		  pcd8544_print_char(':',BLACK);
		  print_int(52,2,rtc_get_sec(),WHITE);
					  
		  print_int(18,3,rtc_get_dom(),WHITE);
		  pcd8544_set_cursor(30,3);
		  pcd8544_print_char('/',BLACK);
		  print_int(36,3,rtc_get_month(),WHITE);
		  pcd8544_set_cursor(49,3);
		  pcd8544_print_char('/',BLACK);
		  print_int(55,3,rtc_get_year(),WHITE);
					  
		  key=waitForKey(250);
					  
		  if(key == ')' || key == 'B')
			  return;
		  
		  else if(key == 'K')
		  {
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,2,"=> ",BLACK);
			  pcd8544_print_string(18,3,"--",BLACK);
			  
			  while(1)
			  {
				  print_int(18,2,rtc_get_hour(),WHITE);
				  pcd8544_set_cursor(30,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(35,2,rtc_get_min(),WHITE);
				  pcd8544_set_cursor(47,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(52,2,rtc_get_sec(),WHITE);
				  key=waitForKey(250);
				  
				  if ( key == '>')
				  {
					  rtc_set_hour(rtc_get_hour()+1);
					  if(rtc_get_hour()>23)
						  rtc_set_hour(0);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_hour(rtc_get_hour()-1);
					  if(rtc_get_hour()<0)
						  rtc_set_hour(23);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,2,"=> ",BLACK);
			  pcd8544_print_string(35,3,"--",BLACK);
			  timer0_delay_ms(200);
			  while(1)
			  {
				  print_int(18,2,rtc_get_hour(),WHITE);
				  pcd8544_set_cursor(30,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(35,2,rtc_get_min(),WHITE);
				  pcd8544_set_cursor(47,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(52,2,rtc_get_sec(),WHITE);
				  key=waitForKey(250);
				  
				  if ( key == '>')
				  {
					  rtc_set_min(rtc_get_min()+1);
					  if(rtc_get_min()>59)
						  rtc_set_min(0);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_min(rtc_get_min()-1);
					  if(rtc_get_min()<0)
						  rtc_set_min(59);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,2,"=> ",BLACK);
			  pcd8544_print_string(52,3,"--",BLACK);
			  timer0_delay_ms(200);
			  while(1)
			  {
				  print_int(18,2,rtc_get_hour(),WHITE);
				  pcd8544_set_cursor(30,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(35,2,rtc_get_min(),WHITE);
				  pcd8544_set_cursor(47,2);
				  pcd8544_print_char(':',BLACK);
				  print_int(52,2,rtc_get_sec(),WHITE);
				  key=waitForKey(250);
				  
				  if ( key == '>')
				  {
					  rtc_set_sec(rtc_get_sec()+1);
					  if(rtc_get_sec()>59)
						  rtc_set_sec(0);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_sec(rtc_get_sec()-1);
					  if(rtc_get_sec()<0)
						  rtc_set_sec(59);
					  print_int(18,2,rtc_get_hour(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(33,2,rtc_get_min(),WHITE);
					  pcd8544_print_char(':',BLACK);
					  print_int(48,2,rtc_get_sec(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
			  /////////////////////////////////////////////////////////////
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,3,"=> ",BLACK);
			  pcd8544_print_string(18,2,"--",BLACK);
			  timer0_delay_ms(200);
			  while(1)
			  {
				  print_int(18,3,rtc_get_dom(),WHITE);
				  pcd8544_set_cursor(30,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(36,3,rtc_get_month(),WHITE);
				  pcd8544_set_cursor(49,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(55,3,rtc_get_year(),WHITE);
				  key=waitForKey(250);
				  if ( key == '>')
				  {
					  rtc_set_dom(rtc_get_dom()+1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_dom(rtc_get_dom()-1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,3,"=> ",BLACK);
			  pcd8544_print_string(36,2,"--",BLACK);
			  timer0_delay_ms(200);
			  while(1)
			  {
				  print_int(18,3,rtc_get_dom(),WHITE);
				  pcd8544_set_cursor(30,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(36,3,rtc_get_month(),WHITE);
				  pcd8544_set_cursor(49,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(55,3,rtc_get_year(),WHITE);
				  key=waitForKey(250);
				  if ( key == '>')
				  {
					  rtc_set_month(rtc_get_month()+1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_month(rtc_get_month()-1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
			  pcd8544_clear_screen(WHITE);
			  pcd8544_print_string(0,5,"SET",BLACK);
			  pcd8544_print_string(68,5,"<-",BLACK);
			  pcd8544_print_string(0,3,"=> ",BLACK);
			  pcd8544_print_string(55,2,"----",BLACK);
			  timer0_delay_ms(200);
			  while(1)
			  {
				  print_int(18,3,rtc_get_dom(),WHITE);
				  pcd8544_set_cursor(30,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(36,3,rtc_get_month(),WHITE);
				  pcd8544_set_cursor(49,3);
				  pcd8544_print_char('/',BLACK);
				  print_int(55,3,rtc_get_year(),WHITE);
				  key=waitForKey(250);
				  if ( key == '>')
				  {
					  rtc_set_year(rtc_get_year()+1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
					  
				  }
				  else if (key == '<')
				  {
					  rtc_set_year(rtc_get_year()-1);
					  print_int(18,3,rtc_get_dom(),WHITE);
					  pcd8544_set_cursor(30,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(36,3,rtc_get_month(),WHITE);
					  pcd8544_set_cursor(49,3);
					  pcd8544_print_char('/',BLACK);
					  print_int(55,3,rtc_get_year(),WHITE);
				   }
				  else if(key == ')' || key == 'B')
					return;
				  else if( key == 'K' )
					break;
			  }
		  pcd8544_clear_screen(WHITE);
		  pcd8544_print_string(0,5,"SET",BLACK);
		  pcd8544_print_string(68,5,"<-",BLACK);
		  pcd8544_print_string(0,2,"=> ",BLACK);
	      pcd8544_print_string(0,3,"=> ",BLACK);
		  }
	  }
 
 }
 void mobile_menu_setting(void)
 {}