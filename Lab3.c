/*
 * GccApplication1.c
 *
 * Created: 9/26/2024 1:49:51 PM
 * Author : pathey1590
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUDRATE ((F_CPU / (BAUD * 16UL) - 1))

void uart_Init(void){
	
	UBRR0H = (BAUDRATE>>8);	//Set high baud rate register
	UBRR0L = BAUDRATE;		//Set low baud rate register

	UCSR0B = (1 << TXEN0);		//Turn on transmitter by enabling TXEN0

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	//set frame format: 8 data bits, 1 stop bit
}

void uart_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}


int main(void)
{
	
	uart_Init();
	
	PORTD |= (1<<4|1<<5|1<<6|1<<7);		//set rows to high
	PORTB |= (1<<0|1<<1|1<<2|1<<3);		//set columns to high
	
	DDRD |= (1<<DDD4 | (1 << DDD5) | (1 << DDD6) | (1 << DDD7));	//set ports wired to columns to input
	DDRB &= ~(1<<DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB3);		//set ports wired to rows to output
	
	char keyPad[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};
	
	
	
    /* Replace with your application code */
    while (1) 
    {
		for(uint8_t i = 0; i < 4; i += 1){
			PORTD &= ~(1<< (4 + i));	//set row[i+4] = 0 to check row 'i' (D uses 4-7 not 0-3)
			for(uint8_t j = 0; j < 4; j += 1){
				if (!(PINB & (1 << j))){
					uart_Transmit(keyPad[i][j]);
					_delay_ms(200);
					//UDR0 = '\0';
					while(!(PINB & (1 << j))){}
				}
			}
			PORTD |= (1 << (4 + i));	//set row back to high
		}
    }
}
