#include <avr/io.h>
#include <stdio.h>

void UART1_init(void)
{
	UBRR1H = 0x00;
	UBRR1L = 207;
	UCSR1A |= _BV(U2X1);
	UCSR1C |= 0x06;
	
	UCSR1B |= _BV(RXEN1);
	UCSR1B |= _BV(TXEN1);
}

void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)) );
	UDR1 = data;
}

unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1 << RXC1)) );
	return UDR1;
}

void UART1_print_string(char *str)
{
	for( int i = 0; str[i] ; i++)
	UART1_transmit(str[i]);
}

void UART1_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	
	sprintf(numString, "%d", n);
	
	UART1_print_string(numString);
}

void UART1_print_float(float f)
{
	char numString[20] = "0";
	
	sprintf(numString, "%f", f);
	
	UART1_print_string(numString);
}
