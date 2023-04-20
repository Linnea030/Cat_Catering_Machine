/*
 * DDminiproject.c
 *
 * Created: 2021/11/27 22:03:13
 * Author : Linnea
 */ 

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTB3
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB2
#define F_CPU 16000000UL

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <time.h>


#define BAUDRATE 9600// 9600 bps
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)//baud prescaller

//---------------------------------------LCD sub function
unsigned char rec = '0';//set receive char

void USART_init(void){
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); //set the baud rate of USART
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);//set baud rate of USART
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //enable transmit of USART
	UCSR0C = (3<<UCSZ00); //use 8-bit (default) of USART
}

void USART_send(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0))); // check that if data is sent
	UDR0 = data; // if sent, load new data to transmit
}

unsigned char USART_receive(void){
	while(!(UCSR0A & (1<<RXC0))); // Wait to receive the data
	return UDR0; // Read data from UDR data register
}

int get_length(char str[])//find length of string
{
	char *q = str;
	int cnt = 0;
	while (*q++ != '\0')
	{
		cnt=cnt+1;
	}
	return cnt;
}

void USART_putstring(char *str)
{
	int c;
	c=get_length(str);//get length of string
	for (int i=0; i<c; i++){
		if (str[i] != 0){
			USART_send(str[i]); //display string
		}
	}
}

void transform(char *str){//get string from terminal
	int i = 0;
	char s=0;//use s to control the loop
	while(s!=0x0d&&s!=0x0a){
		char s = USART_receive();//receive data
		if(s==0x0d||s==0x0a){//0x0d=13 means enter, 0x0a=10 means /r /n
			str[i++] =0;//stop transform
			return;//get string
		}
		else{
			str[i++] =s;//transform
		}
	}
}
//---------------------------------------LCD sub function

//---------------------------------------motor sub function
void delaym()//Use timer1 to prevent error caused by key jitter
{
	TCNT1=0xe17a; //load TCNT1
	TCCR1A=0x00;//TCCR1A=0 use timer1 normal mode
	TCCR1B=0x04; //Timer1, normal mode, 1024 prescaler
	while((TIFR1 & 0x1)==0); //wait for setting flag
	TCCR1B=0;//stop timer1
	TIFR1=0x01; //initialize timer1
}

void initADC(void) {
	ADMUX |= (1 << REFS0) | (1 << MUX0); //reference voltage on AV_CC, and use ADC1
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //ADC clock prescaler / 8
	ADCSRA |= (1 << ADEN); //enables the ADC
}
void initADC1(void) {
	ADMUX |= (1 << REFS0) | (0 << MUX0); //reference voltage on AV_CC, and use ADC0
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //ADC clock prescaler / 8
	ADCSRA |= (1 << ADEN); //enables the ADC
}
void direction(void)	//change direction by using PORTD4 and PORTD5
{
	if ((PINB&0x20)==0x20)//if PortD5=1, PortD4=0
	{
		PORTB &=~(1<<5);//make PortD5=0, PortD4=1
		PORTB|=1<<4;
	}
	else if ((PINB&0x10)==0x10)//if PortD5=0, PortD4=1
	{
		PORTB&=~(1<<4);//make PortD5=1, PortD4=0
		PORTB|=1<<5;
	}
		delaym();//delay 0.5s to prevent error caused by key jitter
}
//---------------------------------------motor sub function
//---------------------------------------servo sub function
void delays1()//use timer0 to delay 1s
{
	int i=0;//let i=0 to make a loop
	while(i!=61){
		TCNT0=0x00; //load tcnt0
		TCCR0A=0x00;//timer0
		TCCR0B=0x05; //timer0, 1024 prescaler
		while((TIFR0 & 0x1)==0); //wait
		TCCR0B=0;//stop timer0
		TIFR0=0x01; //clear tov0 flag
		i=i+1;
	}
}
void delays2()//use timer0 to delay 2s
{
	int i=0;//let i=0 to make a loop
	while(i!=122){
		TCNT0=0x00; //load tcnt0
		TCCR0A=0x00;//timer0
		TCCR0B=0x05; //timer0, 1024 prescaler
		while((TIFR0 & 0x1)==0); //wait
		TCCR0B=0;//stop timer0
		TIFR0=0x01; //clear tov0 flag
		i=i+1;
	}
}
void delays3()//use timer0 to delay 3s
{
	int i=0;//let i=0 to make a loop
	while(i!=183){
		TCNT0=0x00; //load tcnt0
		TCCR0A=0x00;//timer0
		TCCR0B=0x05; //timer0, 1024 prescaler
		while((TIFR0 & 0x1)==0); //wait
		TCCR0B=0;//stop timer0
		TIFR0=0x01; //clear tov0 flag
		i=i+1;
	}
}
void delays4()//use timer0 to delay 4s
{
	int i=0;//let i=0 to make a loop
	while(i!=244){
		TCNT0=0x00; //load tcnt0
		TCCR0A=0x00;//timer0
		TCCR0B=0x05; //timer0, 1024 prescaler
		while((TIFR0 & 0x1)==0); //wait
		TCCR0B=0;//stop timer0
		TIFR0=0x01; //clear tov0 flag
		i=i+1;
	}
}
void delays()//use timer0 to delay 2s
{
	int i=0;//let i=0 to make a loop
	while(i!=50){
		TCNT0=0x00; //load tcnt0
		TCCR0A=0x00;//timer0
		TCCR0B=0x05; //timer0, 1024 prescaler
		while((TIFR0 & 0x1)==0); //wait
		TCCR0B=0;//stop timer0
		TIFR0=0x01; //clear tov0 flag
		i=i+1;
	}
}
//---------------------------------------servo sub function

int main()
{

DDRD=0xFF;// Set all output
DDRB=0xFF;// Set all output
PORTB = 0x00;// Set all low
PORTD = 0x00;// Set all low
TCCR0A=0xc3;// Use fast PWM mode with inverting mode
TCCR0B=0x05;// use timer0 with 1024 prescaler
OCR0A=0xFF;//OCR0A=0xFF=255
		
Lcd4_Init();//initialization of LCD using 8-bit mode
Lcd4_Clear();         //clean LCD
USART_init();// Initialize USART
char string[255]="Wait ";//The first letters to appear
Lcd4_Set_Cursor(1,0);//set cursor at row 1 and column 0
Lcd4_Write_String("ModeD!");// display string on LCD
USART_putstring("Welcome to use\n");//display on terminal

	while (1)//wait here
	{	
		USART_init();//USART initialization	
		rec=USART_receive();
		Lcd4_Init();//initialization of LCD using 4-bit mode
		
		if(rec == 'A'){//ModeA
			Lcd4_Clear();         //clean LCD
			Lcd4_Init();//initialization of LCD using 4-bit mode
			Lcd4_Set_Cursor(1,3);//set cursor at row 1 and column 3
			Lcd4_Write_String("ModeA!");// display string on LCD
			USART_putstring("In modeA\n");//display on terminal
			
			DDRD &=~(1<<2);// Set PD2 input
			DDRD &=~(1<<3);// Set PD3 input
			DDRC|=(1<<3)|(1<<4)|(1<<5);// Set PC3 PC4 PC5 output
			PORTB|=(1<<5);//PB5=1 to control direction of motor
			PORTB|=(1<<2);//PB2=1
			
			TCCR0A=0x83;// Use fast PWM mode with inverting mode
			TCCR0B=0x05;// use timer0 with 1024 prescaler
			uint16_t pValue;
			int value;
			initADC();//function initADC()
			int c=0;//initialize c
			while(1){
					if((PIND&0x08)==0x08){// Quit
					Lcd4_Clear();         //clean LCD
					USART_putstring("Quit\n");//display on terminal
					PORTC &=~(1<<4);//PC3 PC4 PC5 as 0
					PORTC &=~(1<<5);
					PORTC &=~(1<<3);
					OCR0A = 0;//OCR0A=0 stop motor
					Lcd4_Set_Cursor(1,0);//set cursor at row 1 and column 0
					Lcd4_Write_String("Welcome!");// display string on LCD
					break;
					}
					ADCSRA |= (1 << ADSC); //art ADC conversion
					while((ADCSRA & (1 << ADSC))); //wait until ADSC bit is clear, i.e., ADC conversion is done
					uint8_t theLowADC = ADCL;//the low value is in ADCL register
					pValue = ADCH << 8 | theLowADC;//the high value is in ADCH register
					value = pValue/4;//make value in the range of about 0-255
					if (value > 255) {
						OCR0A = 255;//if OCR0A overflow make it equal to 255
					}
					else if (value < 0)
					{
						OCR0A = 0;//if OCR0A is negative make it equal to 0
					}
					else {
						OCR0A = value;//else make it equal to ADC value
					}
					if((PIND&0x04)!=0x04){
						direction();
					}
					delaym();//delay 0.5s to prevent error caused by key jitter
					
					//srand ();
					c=1+rand()%6;//get random number
					switch (c)//select laser case
					{
						
						case 0:{
							PORTC|=(1<<4);
							PORTC &=~(1<<5);
							PORTC &=~(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 1:{
							PORTC&=~(1<<4);
							PORTC|=(1<<5);
							PORTC &=~(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 2:{
							PORTC&=~(1<<4);
							PORTC&=~(1<<5);
							PORTC|=(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 3:{
							PORTC&=~(1<<4);
							PORTC|=(1<<5);
							PORTC|=(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 4:{
							PORTC|=(1<<4);
							PORTC|=(1<<5);
							PORTC|=(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 5:{
							PORTC|=(1<<4);
							PORTC &=~(1<<5);
							PORTC|=(1<<3);
							delaym();
							break;//jump out of switch
						}
						case 6:{
							PORTC|=(1<<4);
							PORTC|=(1<<5);
							PORTC &=~(1<<3);
							delaym();
							break;//jump out of switch
						}
					}
				}
			}
			
		if(rec == 'B'){//ModeB
			DDRD &=~(1<<2);//PD2 PD3 input
			DDRD &=~(1<<3);
			PORTB|=(1<<2);//PB2 high

			Lcd4_Clear();         //clean LCD
			Lcd4_Init();//initialization of LCD using 4-bit mode
			Lcd4_Set_Cursor(1,3);//set cursor at row 1 and column 3
			Lcd4_Write_String("ModeB!");// display string on LCD
			USART_putstring("In modeB\n");//display on terminal
			delays1();//delay for 2s
			Lcd4_Clear();         //clean LCD
			Lcd4_Write_String("Set time");// display string on LCD
			TCCR1A |= (1 << WGM11) | (1 << COM1A1);//set fast pwm 14 mode with non-inverting mode
			TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS11);//set pre-scale of 8 cs11
			ICR1 = 40000; //set icr1max = 40000 icr1 is the top defining pwm period
			int c=0;
			while(1){//duty cycle need offset to correct
			if((PIND&0x04)!=0x04){//set time
				c=c+1;
				if(c>4){
					c=0;
				}
			_delay_ms(50);//Avoid error
			}
			switch (c){
				case 0:{
					Lcd4_Clear();         //clean LCD
					Lcd4_Write_String("Set time");// display string on LCD
					_delay_ms(500);
					break;//jump out of switch
				}
				case 1:{
					Lcd4_Clear();         //clean LCD
					Lcd4_Write_String("Set one hour");// display string on LCD
					_delay_ms(500);
					break;//jump out of switch
				}
				case 2:{
					Lcd4_Clear();         //clean LCD
					Lcd4_Write_String("Set two hours");// display string on LCD
					_delay_ms(500);
					break;//jump out of switch
				}
				case 3:{
					Lcd4_Clear();         //clean LCD
					Lcd4_Write_String("Set three hours");// display string on LCD
					_delay_ms(500);
					break;//jump out of switch
				}
				case 4:{
					Lcd4_Clear();         //clean LCD
					Lcd4_Write_String("Set four hours");// display string on LCD
					_delay_ms(500);
					break;//jump out of switch
				}
			}
			}
				while((PIND&0x08)!=0x08){
				switch (c)
				{
					case 0:{
						OCR1A = 1000;//-90 degree
						break;//jump out of switch
					}
					case 1:{
						//Lcd4_Clear();         //clean LCD
						//Lcd4_Write_String("Set one hour");// display string on LCD
						OCR1A = 1000;//-90 degree
						delays1();//delay for 1s
						OCR1A = 5000;//90 degree
						delays();//delay for 1s
						OCR1A = 1000;//-90 degree
						delays1();//delay for 1s
						break;//jump out of switch
					}
					case 2:{
						OCR1A = 1000;//-90 degree
						delays2();//delay for 2s
						OCR1A = 5000;//90 degree
						delays();//delay for 1s
						OCR1A = 1000;//-90 degree
						delays2();//delay for 2s
						break;//jump out of switch
					}
					case 3:{
						OCR1A = 1000;//-90 degree
						delays3();//delay for 3s
						OCR1A = 5000;//90 degree
						delays();//delay for 1s
						OCR1A = 1000;//-90 degree
						delays3();//delay for 3s
						break;//jump out of switch
					}
					case 4:{
						OCR1A = 1000;//-90 degree
						delays4();//delay for 4s
						OCR1A = 5000;//]90 degree
						delays();//delay for 1s
						OCR1A = 1000;//-90 degree
						delays4();//delay for 4s
						break;//jump out of switch
					}
				}
			}
		}
		}
		if(rec == 'C'){//ModeC
			DDRD &=~(1<<2);//PD2 PD3 input
			DDRD &=~(1<<3);
			PORTB|=(1<<2);//PB2 high
			Lcd4_Clear();         //clean LCD
			Lcd4_Set_Cursor(1,3);//set cursor at row 1 and column 3
			Lcd4_Write_String("ModeC!");// display string on LCD
			USART_putstring("In modeC\n");//display on terminal
			delays1();
			EIMSK |= (1 << INT0); //enable external interrupt 0
			sei(); //enable interrupts
			while (1){
				Lcd4_Clear();         //clean LCD
				UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0));//stop USART send and receive
				Lcd4_Init();//initialization of LCD using 8-bit mode
				Lcd4_Set_Cursor(1,3);//set cursor at row 1 and column 3
				Lcd4_Write_String(string);// display string on LCD
				USART_init();//USART initialization
				transform(string);// get string from terminal one by one
				USART_putstring("Received note\n");//display on terminal
				delays1();//delay for 1s
				USART_putstring("Please change note\n");//display on terminal
			}
		}
		if(rec == 'D'){//ModeD
			DDRD &=~(1<<2);//PD2 PD3 input
			DDRD &=~(1<<3);
			uint16_t pValue;
			int value;
			initADC1();//function initADC()
			DDRB =0xFF;//output
			DDRD =0xFF;//output
			DDRC =0x00;//input
			PORTD &= ~(1<<3);//PD3
			Lcd4_Init();			//Initialize LCD
			char data[5];
			while (1)//wait here
			{
				Lcd4_Clear();			//Clear LCD
				char* s1="Humidity =";// set Humidity
				Lcd4_Set_Cursor(1,0);	//set cursor at row 1 and column 0
				Lcd4_Write_String(s1);// 
				PORTD|=(1<<3);//PD3 high
				ADCSRA |= (1 << ADSC); //art ADC conversion
				while((ADCSRA & (1 << ADSC))); //wait until ADSC bit is clear, i.e., ADC conversion is done
				uint8_t theLowADC = ADCL;//the low value is in ADCL register
				pValue = ADCH << 8 | theLowADC;//the high value is in ADCH register
				value = pValue;//make value in the range of about 0-1023
				PORTD &= ~(1<<3);//PD3=0
				itoa(value,data,10);//change data type to char
				Lcd4_Set_Cursor(1,11);//set cursor at row 1 and column 11
				Lcd4_Write_String(data);//display data

				if (value > 600) {
				}
				else if (value < 600)
				{
				Lcd4_Set_Cursor(2,0);//set cursor at row 2 and column 0
				Lcd4_Write_String("wrong");//display wrong
				}
				if((PIND&0x08)==0x08){//Quit
					USART_putstring("Quit\n");//display on terminal
					Lcd4_Clear();//Clear LCD
					Lcd4_Set_Cursor(1,0);//set cursor at row 1 and column 0
					Lcd4_Write_String("Welcome!");// display string on LCD
					break;
				}
				delays1();
			}
		}
	return 0;
}

ISR (INT0_vect) //ISR for external interrupt 0
{
						Lcd4_Clear();//Clear LCD
						USART_putstring("Quit\n");//display on terminal
						Lcd4_Set_Cursor(1,0);//set cursor at row 1 and column 0
						Lcd4_Write_String("Welcome!");// display string on LCD
						delays1();//delay 1s
}
