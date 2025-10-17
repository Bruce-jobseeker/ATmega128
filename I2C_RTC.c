#include <avr/sfr_defs.h>
#include "I2C_RTC.h"

void I2C_init(void)
{
	DDRD |= (1 << I2C_SCL);
	DDRD |= (1 << I2C_SDA);
	
	TWBR = 32;
}

void I2C_start(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	
	while( !(TWCR & (1 << TWINT)) );
}

void I2C_transmit(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );
}

uint8_t I2C_receive_ACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );
	
	return TWDR;
}

uint8_t I2C_receive_NACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	while( !(TWCR & (1 << TWINT)) );
	
	return TWDR;
}

void I2C_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}
