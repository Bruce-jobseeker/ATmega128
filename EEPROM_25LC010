#include <avr/sfr_defs.h>
#include "EEPROM_25LC010.h"

void SPI_Init(void)
{
	DDRB |= (1 << SPI_SS);
	PORTB |= (1 << SPI_SS);
	DDRB |= (1 << SPI_MOSI);
	DDRB &= ~(1 << SPI_MISO);
	DDRB |= (1 << SPI_SCK);
	
	SPCR |= (1 << MSTR);
	SPCR |= (1 << SPE);
}

void EEPROM_changeByte(uint8_t byte)
{
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)));
}

void EEPROM_sendAddress(uint8_t address)
{
	EEPROM_changeByte(address);
}

uint8_t EEPROM_readByte(uint8_t address)
{
	EEPROM_Select();
	EEPROM_changeByte(EEPROM_READ);
	EEPROM_sendAddress(address);
	EEPROM_changeByte(0);
	EEPROM_DeSelect();
	
	return SPDR;
}

void EEPROM_writeEnable(void)
{
	EEPROM_Select();
	EEPROM_changeByte(EEPROM_WREN);
	EEPROM_DeSelect();
}

void EEPROM_writeByte(uint8_t address, uint8_t data)
{
	EEPROM_writeEnable();
	
	EEPROM_Select();
	EEPROM_changeByte(EEPROM_WRITE);
	EEPROM_sendAddress(address);
	EEPROM_changeByte(data);
	EEPROM_DeSelect();
	
	while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS));
}

void EEPROM_write_int1(int address, int value)
{
	uint8_t *p1 = (uint8_t *)(&value);
	EEPROM_writeByte(address, *p1);
	EEPROM_writeByte(address+1, *(p1+1));
}

void EEPROM_write_int2(int address, int value)
{
	typedef union{
		int val;
		uint8_t bytes[2];
		} EEPROMint;
	
	EEPROMint a;
	a.val = value;
		
	EEPROM_writeByte(address, a.bytes[0]);
	EEPROM_writeByte(address+1, a.bytes[1]);
}

int EEPROM_read_int(int address)
{
	int result = EEPROM_readByte(address) + (EEPROM_readByte(address+1) << 8);
	
	return result;
}


uint8_t EEPROM_readStatus(void)
{
	EEPROM_Select();
	EEPROM_changeByte(EEPROM_RDSR);
	EEPROM_changeByte(0);
	EEPROM_DeSelect();
	
	return SPDR;
}

void EEPROM_eraseAll(void)
{
	uint8_t i;
	uint16_t pageAddress = 0;
	
	while(pageAddress < EEPROM_TOTAL_BYTE){
		EEPROM_writeEnable();
		EEPROM_Select();
		EEPROM_changeByte(EEPROM_WRITE);
		EEPROM_sendAddress(pageAddress);
		for(i=0 ; i<EEPROM_PAGE_SIZE ; i++)
		EEPROM_changeByte(0);
		EEPROM_DeSelect();
		
		pageAddress += EEPROM_PAGE_SIZE;
		while(EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS));
	}
}
